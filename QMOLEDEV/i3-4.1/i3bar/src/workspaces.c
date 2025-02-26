/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3bar - an xcb-based status- and ws-bar for i3
 * © 2010-2011 Axel Wagner and contributors (see also: LICENSE)
 *
 * workspaces.c: Maintaining the workspace-lists
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <yajl/yajl_parse.h>
#include <yajl/yajl_version.h>

#include "common.h"

/* A datatype to pass through the callbacks to save the state */
struct workspaces_json_params {
    struct ws_head *workspaces;
    i3_ws          *workspaces_walk;
    char           *cur_key;
    char           *json;
};

/*
 * Parse a boolean value (visible, focused, urgent)
 *
 */
static int workspaces_boolean_cb(void *params_, int val) {
    struct workspaces_json_params *params = (struct workspaces_json_params*) params_;

    if (!strcmp(params->cur_key, "visible")) {
        params->workspaces_walk->visible = val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "focused")) {
        params->workspaces_walk->focused = val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "urgent")) {
        params->workspaces_walk->urgent = val;
        FREE(params->cur_key);
        return 1;
    }

    FREE(params->cur_key);

    return 0;
}

/*
 * Parse an integer (num or the rect)
 *
 */
#if YAJL_MAJOR >= 2
static int workspaces_integer_cb(void *params_, long long val) {
#else
static int workspaces_integer_cb(void *params_, long val) {
#endif
    struct workspaces_json_params *params = (struct workspaces_json_params*) params_;

    if (!strcmp(params->cur_key, "num")) {
        params->workspaces_walk->num = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "x")) {
        params->workspaces_walk->rect.x = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "y")) {
        params->workspaces_walk->rect.y = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "width")) {
        params->workspaces_walk->rect.w = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    if (!strcmp(params->cur_key, "height")) {
        params->workspaces_walk->rect.h = (int) val;
        FREE(params->cur_key);
        return 1;
    }

    FREE(params->cur_key);
    return 0;
}

/*
 * Parse a string (name, output)
 *
 */
#if YAJL_MAJOR >= 2
static int workspaces_string_cb(void *params_, const unsigned char *val, size_t len) {
#else
static int workspaces_string_cb(void *params_, const unsigned char *val, unsigned int len) {
#endif
        struct workspaces_json_params *params = (struct workspaces_json_params*) params_;

        char *output_name;

        if (!strcmp(params->cur_key, "name")) {
            /* Save the name */
            params->workspaces_walk->name = smalloc(sizeof(const unsigned char) * (len + 1));
            strncpy(params->workspaces_walk->name, (const char*) val, len);
            params->workspaces_walk->name[len] = '\0';

            /* Convert the name to ucs2, save its length in glyphs and calculate its rendered width */
            int ucs2_len;
            xcb_char2b_t *ucs2_name = (xcb_char2b_t*) convert_utf8_to_ucs2(params->workspaces_walk->name, &ucs2_len);
            params->workspaces_walk->ucs2_name = ucs2_name;
            params->workspaces_walk->name_glyphs = ucs2_len;
            params->workspaces_walk->name_width =
                predict_text_extents(params->workspaces_walk->ucs2_name,
                params->workspaces_walk->name_glyphs);

            DLOG("Got Workspace %s, name_width: %d, glyphs: %d\n",
                 params->workspaces_walk->name,
                 params->workspaces_walk->name_width,
                 params->workspaces_walk->name_glyphs);
            FREE(params->cur_key);

            return 1;
        }

        if (!strcmp(params->cur_key, "output")) {
            /* We add the ws to the TAILQ of the output, it belongs to */
            output_name = smalloc(sizeof(const unsigned char) * (len + 1));
            strncpy(output_name, (const char*) val, len);
            output_name[len] = '\0';
            i3_output *target = get_output_by_name(output_name);
            if (target) {
                params->workspaces_walk->output = target;

                TAILQ_INSERT_TAIL(params->workspaces_walk->output->workspaces,
                                  params->workspaces_walk,
                                  tailq);
            }

            FREE(output_name);
            return 1;
        }

        return 0;
}

/*
 * We hit the start of a json-map (rect or a new output)
 *
 */
static int workspaces_start_map_cb(void *params_) {
    struct workspaces_json_params *params = (struct workspaces_json_params*) params_;

    i3_ws *new_workspace = NULL;

    if (params->cur_key == NULL) {
        new_workspace = smalloc(sizeof(i3_ws));
        new_workspace->num = -1;
        new_workspace->name = NULL;
        new_workspace->visible = 0;
        new_workspace->focused = 0;
        new_workspace->urgent = 0;
        memset(&new_workspace->rect, 0, sizeof(rect));
        new_workspace->output = NULL;

        params->workspaces_walk = new_workspace;
        return 1;
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
static int workspaces_map_key_cb(void *params_, const unsigned char *keyVal, size_t keyLen) {
#else
static int workspaces_map_key_cb(void *params_, const unsigned char *keyVal, unsigned int keyLen) {
#endif
    struct workspaces_json_params *params = (struct workspaces_json_params*) params_;
    FREE(params->cur_key);

    params->cur_key = smalloc(sizeof(unsigned char) * (keyLen + 1));
    strncpy(params->cur_key, (const char*) keyVal, keyLen);
    params->cur_key[keyLen] = '\0';

    return 1;
}

/* A datastructure to pass all these callbacks to yajl */
yajl_callbacks workspaces_callbacks = {
    NULL,
    &workspaces_boolean_cb,
    &workspaces_integer_cb,
    NULL,
    NULL,
    &workspaces_string_cb,
    &workspaces_start_map_cb,
    &workspaces_map_key_cb,
    NULL,
    NULL,
    NULL
};

/*
 * Start parsing the received json-string
 *
 */
void parse_workspaces_json(char *json) {
    /* FIXME: Fasciliate stream-processing, i.e. allow starting to interpret
     * JSON in chunks */
    struct workspaces_json_params params;

    free_workspaces();

    params.workspaces_walk = NULL;
    params.cur_key = NULL;
    params.json = json;

    yajl_handle handle;
    yajl_status state;
#if YAJL_MAJOR < 2
    yajl_parser_config parse_conf = { 0, 0 };

    handle = yajl_alloc(&workspaces_callbacks, &parse_conf, NULL, (void*) &params);
#else
    handle = yajl_alloc(&workspaces_callbacks, NULL, (void*) &params);
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
            ELOG("Could not parse workspaces-reply!\n");
            exit(EXIT_FAILURE);
            break;
    }

    yajl_free(handle);

    FREE(params.cur_key);
}

/*
 * free() all workspace data-structures. Does not free() the heads of the tailqueues.
 *
 */
void free_workspaces() {
    i3_output *outputs_walk;
    if (outputs == NULL) {
        return;
    }
    i3_ws     *ws_walk;

    SLIST_FOREACH(outputs_walk, outputs, slist) {
        if (outputs_walk->workspaces != NULL && !TAILQ_EMPTY(outputs_walk->workspaces)) {
            TAILQ_FOREACH(ws_walk, outputs_walk->workspaces, tailq) {
                FREE(ws_walk->name);
                FREE(ws_walk->ucs2_name);
            }
            FREE_TAILQ(outputs_walk->workspaces, i3_ws);
        }
    }
}
