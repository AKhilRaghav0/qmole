/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3bar - an xcb-based status- and ws-bar for i3
 * © 2010-2011 Axel Wagner and contributors (see also: LICENSE)
 *
 * outputs.c: Maintaining the output-list
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <i3/ipc.h>
#include <yajl/yajl_parse.h>
#include <yajl/yajl_version.h>

#include "common.h"

/* A datatype to pass through the callbacks to save the state */
struct outputs_json_params {
    struct outputs_head *outputs;
    i3_output           *outputs_walk;
    char                *cur_key;
    char                *json;
    bool                in_rect;
};

/*
 * Parse a null-value (current_workspace)
 *
 */
static int outputs_null_cb(void *params_) {
    struct outputs_json_params *params = (struct outputs_json_params*) params_;

    FREE(params->cur_key);

    return 1;
}

/*
 * Parse a boolean value (active)
 *
 */
static int outputs_boolean_cb(void *params_, int val) {
    struct outputs_json_params *params = (struct outputs_json_params*) params_;

    if (strcmp(params->cur_key, "active")) {
        return 0;
    }

    params->outputs_walk->active = val;

    FREE(params->cur_key);

    return 1;
}

/*
 * Parse an integer (current_workspace or the rect)
 *
 */
#if YAJL_MAJOR >= 2
static int outputs_integer_cb(void *params_, long long val) {
#else
static int outputs_integer_cb(void *params_, long val) {
#endif
    struct outputs_json_params *params = (struct outputs_json_params*) params_;

    if (!strcmp(params->cur_key, "current_workspace")) {
        params->outputs_walk->ws = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "x")) {
        params->outputs_walk->rect.x = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "y")) {
        params->outputs_walk->rect.y = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "width")) {
        params->outputs_walk->rect.w = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "height")) {
        params->outputs_walk->rect.h = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    return 0;
}

/*
 * Parse a string (name)
 *
 */
#if YAJL_MAJOR >= 2
static int outputs_string_cb(void *params_, const unsigned char *val, size_t len) {
#else
static int outputs_string_cb(void *params_, const unsigned char *val, unsigned int len) {
#endif
    struct outputs_json_params *params = (struct outputs_json_params*) params_;

    if (!strcmp(params->cur_key, "current_workspace")) {
        char *copy = smalloc(sizeof(const unsigned char) * (len + 1));
        strncpy(copy, (const char*) val, len);
        copy[len] = '\0';

        char *end;
        errno = 0;
        long parsed_num = strtol(copy, &end, 10);
        if (errno == 0 &&
            (end && *end == '\0'))
            params->outputs_walk->ws = parsed_num;
        free(copy);
        FREE(params->cur_key);
        return 1;
    }

    if (strcmp(params->cur_key, "name")) {
        return 0;
    }

    char *name = smalloc(sizeof(const unsigned char) * (len + 1));
    strncpy(name, (const char*) val, len);
    name[len] = '\0';

    params->outputs_walk->name = name;

    FREE(params->cur_key);

    return 1;
}

/*
 * We hit the start of a json-map (rect or a new output)
 *
 */
static int outputs_start_map_cb(void *params_) {
    struct outputs_json_params *params = (struct outputs_json_params*) params_;
    i3_output *new_output = NULL;

    if (params->cur_key == NULL) {
        new_output = smalloc(sizeof(i3_output));
        new_output->name = NULL;
        new_output->ws = 0,
        memset(&new_output->rect, 0, sizeof(rect));
        new_output->bar = XCB_NONE;

        new_output->workspaces = smalloc(sizeof(struct ws_head));
        TAILQ_INIT(new_output->workspaces);

        new_output->trayclients = smalloc(sizeof(struct tc_head));
        TAILQ_INIT(new_output->trayclients);

        params->outputs_walk = new_output;

        return 1;
    }

    if (!strcmp(params->cur_key, "rect")) {
        params->in_rect = true;
    }

    return 1;
}

/*
 * We hit the end of a map (rect or a new output)
 *
 */
static int outputs_end_map_cb(void *params_) {
    struct outputs_json_params *params = (struct outputs_json_params*) params_;
    if (params->in_rect) {
        params->in_rect = false;
        /* Ignore the end of a rect */
        return 1;
    }

    /* See if we actually handle that output */
    if (config.num_outputs > 0) {
        bool handle_output = false;
        for (int c = 0; c < config.num_outputs; c++) {
            if (strcasecmp(params->outputs_walk->name, config.outputs[c]) != 0)
                continue;

            handle_output = true;
            break;
        }
        if (!handle_output) {
            DLOG("Ignoring output \"%s\", not configured to handle it.\n",
                 params->outputs_walk->name);
            FREE(params->outputs_walk->name);
            FREE(params->outputs_walk->workspaces);
            FREE(params->outputs_walk->trayclients);
            FREE(params->outputs_walk);
            FREE(params->cur_key);
            return 1;
        }
    }

    i3_output *target = get_output_by_name(params->outputs_walk->name);

    if (target == NULL) {
        SLIST_INSERT_HEAD(outputs, params->outputs_walk, slist);
    } else {
        target->active = params->outputs_walk->active;
        target->ws = params->outputs_walk->ws;
        target->rect = params->outputs_walk->rect;
    }
    return 1;
}

/*
 * Parse a key.
 *
 * Essentially we just save it in the parsing-state
 *
 */
#if YAJL_MAJOR >= 2
static int outputs_map_key_cb(void *params_, const unsigned char *keyVal, size_t keyLen) {
#else
static int outputs_map_key_cb(void *params_, const unsigned char *keyVal, unsigned keyLen) {
#endif
    struct outputs_json_params *params = (struct outputs_json_params*) params_;
    FREE(params->cur_key);

    params->cur_key = smalloc(sizeof(unsigned char) * (keyLen + 1));
    strncpy(params->cur_key, (const char*) keyVal, keyLen);
    params->cur_key[keyLen] = '\0';

    return 1;
}

/* A datastructure to pass all these callbacks to yajl */
yajl_callbacks outputs_callbacks = {
    &outputs_null_cb,
    &outputs_boolean_cb,
    &outputs_integer_cb,
    NULL,
    NULL,
    &outputs_string_cb,
    &outputs_start_map_cb,
    &outputs_map_key_cb,
    &outputs_end_map_cb,
    NULL,
    NULL
};

/*
 * Initiate the output-list
 *
 */
void init_outputs() {
    outputs = smalloc(sizeof(struct outputs_head));
    SLIST_INIT(outputs);
}

/*
 * Start parsing the received json-string
 *
 */
void parse_outputs_json(char *json) {
    struct outputs_json_params params;

    params.outputs_walk = NULL;
    params.cur_key = NULL;
    params.json = json;
    params.in_rect = false;

    yajl_handle handle;
    yajl_status state;
#if YAJL_MAJOR < 2
    yajl_parser_config parse_conf = { 0, 0 };

    handle = yajl_alloc(&outputs_callbacks, &parse_conf, NULL, (void*) &params);
#else
    handle = yajl_alloc(&outputs_callbacks, NULL, (void*) &params);
#endif

    state = yajl_parse(handle, (const unsigned char*) json, strlen(json));

    /* FIXME: Propper errorhandling for JSON-parsing */
    switch (state) {
        case yajl_status_ok:
            break;
        case yajl_status_client_canceled:
#if YAJL_MAJOR < 2
        case yajl_status_insufficient_data:
#endif
        case yajl_status_error:
            ELOG("Could not parse outputs-reply!\n");
            exit(EXIT_FAILURE);
            break;
    }

    yajl_free(handle);
}

/*
 * Returns the output with the given name
 *
 */
i3_output *get_output_by_name(char *name) {
    i3_output *walk;
    if (name == NULL) {
        return NULL;
    }
    SLIST_FOREACH(walk, outputs, slist) {
        if (!strcmp(walk->name, name)) {
            break;
        }
    }

    return walk;
}
