/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © 2009-2011 Michael Stapelberg and contributors (see also: LICENSE)
 *
 * resize.c: Interactive resizing.
 *
 */
#include "all.h"

extern xcb_connection_t *conn;

/*
 * This is an ugly data structure which we need because there is no standard
 * way of having nested functions (only available as a gcc extension at the
 * moment, clang doesn’t support it) or blocks (only available as a clang
 * extension and only on Mac OS X systems at the moment).
 *
 */
struct callback_params {
    orientation_t orientation;
    Con *output;
    xcb_window_t helpwin;
    uint32_t *new_position;
};

DRAGGING_CB(resize_callback) {
    const struct callback_params *params = extra;
    Con *output = params->output;
    DLOG("new x = %d, y = %d\n", new_x, new_y);
    if (params->orientation == HORIZ) {
        /* Check if the new coordinates are within screen boundaries */
        if (new_x > (output->rect.x + output->rect.width - 25) ||
            new_x < (output->rect.x + 25))
            return;

        *(params->new_position) = new_x;
        xcb_configure_window(conn, params->helpwin, XCB_CONFIG_WINDOW_X, params->new_position);
    } else {
        if (new_y > (output->rect.y + output->rect.height - 25) ||
            new_y < (output->rect.y + 25))
            return;

        *(params->new_position) = new_y;
        xcb_configure_window(conn, params->helpwin, XCB_CONFIG_WINDOW_Y, params->new_position);
    }

    xcb_flush(conn);
}

int resize_graphical_handler(Con *first, Con *second, orientation_t orientation, const xcb_button_press_event_t *event) {
    DLOG("resize handler\n");

    uint32_t new_position;

    /* TODO: previously, we were getting a rect containing all screens. why? */
    Con *output = con_get_output(first);
    DLOG("x = %d, width = %d\n", output->rect.x, output->rect.width);

    uint32_t mask = 0;
    uint32_t values[2];

    mask = XCB_CW_OVERRIDE_REDIRECT;
    values[0] = 1;

    /* Open a new window, the resizebar. Grab the pointer and move the window around
       as the user moves the pointer. */
    xcb_window_t grabwin = create_window(conn, output->rect, XCB_WINDOW_CLASS_INPUT_ONLY, XCURSOR_CURSOR_POINTER, true, mask, values);

    Rect helprect;
    if (orientation == HORIZ) {
        helprect.x = event->root_x;
        helprect.y = output->rect.y;
        helprect.width = 2;
        helprect.height = output->rect.height;
        new_position = event->root_x;
    } else {
        helprect.x = output->rect.x;
        helprect.y = event->root_y;
        helprect.width = output->rect.width;
        helprect.height = 2;
        new_position = event->root_y;
    }

    mask = XCB_CW_BACK_PIXEL;
    values[0] = config.client.focused.border;

    mask |= XCB_CW_OVERRIDE_REDIRECT;
    values[1] = 1;

    xcb_window_t helpwin = create_window(conn, helprect, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                         (orientation == HORIZ ?
                                          XCURSOR_CURSOR_RESIZE_HORIZONTAL :
                                          XCURSOR_CURSOR_RESIZE_VERTICAL), true, mask, values);

    xcb_circulate_window(conn, XCB_CIRCULATE_RAISE_LOWEST, helpwin);

    xcb_flush(conn);

    const struct callback_params params = { orientation, output, helpwin, &new_position };

    drag_pointer(NULL, event, grabwin, BORDER_TOP, resize_callback, &params);

    xcb_destroy_window(conn, helpwin);
    xcb_destroy_window(conn, grabwin);
    xcb_flush(conn);

    int pixels;
    if (orientation == HORIZ)
        pixels = (new_position - event->root_x);
    else pixels = (new_position - event->root_y);

    DLOG("Done, pixels = %d\n", pixels);

    // if we got thus far, the containers must have
    // percentages associated with them
    assert(first->percent > 0.0);
    assert(second->percent > 0.0);

    // calculate the new percentage for the first container
    double new_percent, difference;
    double percent = first->percent;
    DLOG("percent = %f\n", percent);
    int original = (orientation == HORIZ ? first->rect.width : first->rect.height);
    DLOG("original = %d\n", original);
    new_percent = (original + pixels) * (percent / original);
    difference = percent - new_percent;
    DLOG("difference = %f\n", difference);
    DLOG("new percent = %f\n", new_percent);
    first->percent = new_percent;

    // calculate the new percentage for the second container
    double s_percent = second->percent;
    second->percent = s_percent + difference;
    DLOG("second->percent = %f\n", second->percent);

    // now we must make sure that the sum of the percentages remain 1.0
    con_fix_percent(first->parent);

    return 0;
}
