/*
 * graph.c - a graph widget
 *
 * Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
 * Copyright © 2007-2008 Marco Candrian <mac@calmar.ws>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "widget.h"
#include "screen.h"
#include "common/util.h"
#include "common/draw.h"
#include "common/configopts.h"

extern AwesomeConf globalconf;

typedef struct
{
    /* general layout */

    char **data_title;                  /** data_title of the data sections */
    float *max;                         /** Represents a full graph */
    int width;                          /** Width of the widget */
    float height;                       /** Height of graph (0-1; 1 = height of statusbar) */
    int box_height;                     /** Height of the innerbox in pixels */
    int size;                           /** Size of lines-array (also innerbox-lenght) */
    XColor bg;                          /** Background color */
    XColor bordercolor;                 /** Border color */
    Position grow;                      /** grow: Left or Right */

    /* markers... */
    int *index;                         /** Index of current (new) value */
    int *max_index;                     /** Index of the actual maximum value */
    float *current_max;                 /** Pointer to current maximum value itself */

    /* all data is stored here */
    int data_items;                     /** Number of data-input items */
    int **lines;                        /** Keeps the calculated values (line-length); */
    float **values;                     /** Actual values */

    /* additional data + a pointer to **lines accordingly */
    int **fillbottom;                   /** Data array pointer (like *lines) */
    int **fillbottom_index;             /** Points to some index[i] */
    int fillbottom_total;               /** Total of them */
    Bool *fillbottom_vertical_grad;     /** Create a vertical color gradient */
    XColor *fillbottom_color;           /** Color of them */
    XColor **fillbottom_pcolor_center;  /** Color at middle of graph */
    XColor **fillbottom_pcolor_end;     /** Color at end of graph */
    int **filltop;                      /** Data array pointer (like *lines) */
    int **filltop_index;                /** Points to some index[i] */
    int filltop_total;                  /** Total of them */
    Bool *filltop_vertical_grad;        /** Create a vertical color gradient */
    XColor *filltop_color;              /** Color of them */
    XColor **filltop_pcolor_center;     /** Color at center of graph */
    XColor **filltop_pcolor_end;        /** Color at end of graph */
    int **drawline;                     /** Data array pointer (like *lines) */
    int **drawline_index;               /** Points to some index[i] */
    int drawline_total;                 /** Total of them */
    Bool *drawline_vertical_grad;       /** Create a vertical color gradient */
    XColor *drawline_color;             /** Color of them */
    XColor **drawline_pcolor_center;    /** Color at middle of graph */
    XColor **drawline_pcolor_end;       /** Color at end of graph */

    int *draw_from;                     /** Preparation/tmp array for draw_graph(); */
    int *draw_to;                       /** Preparation/tmp array for draw_graph(); */

} Data;

static int
graph_draw(Widget *widget, DrawCtx *ctx, int offset,
                 int used __attribute__ ((unused)))
{
    int margin_top;
    int z, y, x, tmp, cur_index, test_index;
    Data *d = widget->data;
    area_t rectangle, pattern_area;

    if(!d->data_items)
        return 0;

    if(!widget->user_supplied_x)
        widget->area.x = widget_calculate_offset(widget->statusbar->width,
                                                 d->width,
                                                 offset,
                                                 widget->alignment);
    if(!widget->user_supplied_y)
        widget->area.y = 0;

    /* box = the graph inside the rectangle */
    if(!(d->box_height))
        d->box_height = (int) (widget->statusbar->height * d->height + 0.5) - 2;

    margin_top = (int)((widget->statusbar->height - (d->box_height + 2)) / 2 + 0.5) + widget->area.y;

    /* draw background */
    rectangle.x = widget->area.x + 1;
    rectangle.y = margin_top + 1;
    rectangle.width = d->size;
    rectangle.height = d->box_height;
    draw_rectangle(ctx, rectangle, 1.0, True, d->bg);

    /* for graph drawing */
    rectangle.y = margin_top + d->box_height + 1; /* bottom left corner as starting point */
    rectangle.width = d->size; /* rectangle.height is not used */

    draw_graph_setup(ctx); /* setup some drawing options */

    /* gradient begin either left or on the right of the rectangle */
    if(d->grow == Right)
        pattern_area.x = rectangle.x + rectangle.width;
    else
        pattern_area.x = rectangle.x;

    if(d->filltop_total)
    {
        pattern_area.y = rectangle.y - rectangle.height;

        /* draw style = top */
        for(z = 0; z < d->filltop_total; z++)
        {
            if(d->filltop_vertical_grad[z])
            {
                pattern_area.width = 0;
                pattern_area.height = rectangle.height;
            }
            else
            {
                pattern_area.height = 0;

                if(d->grow == Right)
                    pattern_area.width = -rectangle.width;
                else
                    pattern_area.width = rectangle.width;
            }

            cur_index = *(d->filltop_index[z]);

            for(y = 0; y < d->size; y++)
            {
                /* draw this filltop data thing [z]. But figure out the part
                 * what shall be visible. Therefore find the next smaller value
                 * on this index (draw_from) - might be 0 (then draw from start) */
                for(tmp = 0, x = 0; x < d->filltop_total; x++)
                {
                    if (x == z) /* no need to compare with itself */
                        continue;

                    /* current index's can be different (widget_tell might shift
                     * some with a different frequenzy), so calculate
                     * offset and compare accordingly finally */
                    test_index = cur_index + (*(d->filltop_index[x]) - *(d->filltop_index[z]));

                    if (test_index < 0)
                        test_index = d->size + test_index; /* text_index is minus, since < 0 */
                    else if(test_index >= d->size)
                        test_index -= d->size;

                    /* ... (test_)index to test for a smaller value found. */

                    /* if such a smaller value (to not overdraw!) is there, store into 'tmp' */
                    if(d->filltop[x][test_index] > tmp && d->filltop[x][test_index] < d->filltop[z][cur_index])
                        tmp = d->filltop[x][test_index];

                }
                /* reverse values (because drawing from top) */
                d->draw_from[cur_index] = d->box_height - tmp; /* i.e. no smaller value -> from top of box */
                d->draw_to[cur_index] = d->box_height - d->filltop[z][cur_index]; /* i.e. on full graph -> 0 = bottom */

                if (--cur_index < 0) /* next index to compare to other values */
                    cur_index = d->size - 1;
            }
            draw_graph(ctx, rectangle , d->draw_from, d->draw_to, *(d->filltop_index[z]), d->grow, pattern_area,
                       &(d->filltop_color[z]), d->filltop_pcolor_center[z], d->filltop_pcolor_end[z]);
        }
    }

    pattern_area.y = rectangle.y;

    if(d->fillbottom_total)
    {
        /* draw style = bottom */
        for(z = 0; z < d->fillbottom_total; z++)
        {
            if(d->fillbottom_vertical_grad[z])
            {
                pattern_area.width = 0;
                pattern_area.height = -rectangle.height;
            }
            else
            {
                pattern_area.height = 0;

                if(d->grow == Right)
                    pattern_area.width = -rectangle.width;
                else
                    pattern_area.width = rectangle.width;
            }

            cur_index = *(d->fillbottom_index[z]);

            for(y = 0; y < d->size; y++)
            {
                for(tmp = 0, x = 0; x < d->fillbottom_total; x++)
                {
                    if (x == z)
                        continue;

                    test_index = cur_index + (*(d->fillbottom_index[x]) - *(d->fillbottom_index[z]));

                    if (test_index < 0)
                        test_index = d->size + test_index;
                    else if(test_index >= d->size)
                        test_index -= d->size;

                    if(d->fillbottom[x][test_index] > tmp && d->fillbottom[x][test_index] < d->fillbottom[z][cur_index])
                        tmp = d->fillbottom[x][test_index];
                }
                d->draw_from[cur_index] = tmp;
                if (--cur_index < 0)
                    cur_index = d->size - 1;
            }
            draw_graph(ctx, rectangle, d->draw_from, d->fillbottom[z], *(d->fillbottom_index[z]), d->grow,
                       pattern_area, &(d->fillbottom_color[z]), d->fillbottom_pcolor_center[z], d->fillbottom_pcolor_end[z]);
        }
    }

    if(d->drawline_total)
    {
        /* draw style = line */
        for(z = 0; z < d->drawline_total; z++)
        {
            if(d->drawline_vertical_grad[z])
            {
                pattern_area.width = 0;
                pattern_area.height = -rectangle.height;
            }
            else
            {
                pattern_area.height = 0;
                if(d->grow == Right)
                    pattern_area.width = -rectangle.width;
                else
                    pattern_area.width = rectangle.width;
            }

            draw_graph_line(ctx, rectangle, d->drawline[z], *(d->drawline_index[z]), d->grow, pattern_area,
                            &(d->drawline_color[z]), d->drawline_pcolor_center[z], d->drawline_pcolor_end[z]);
        }
    }

    /* draw border (after line-drawing, what paints 0-values to the border) */
    rectangle.x = widget->area.x;
    rectangle.y = margin_top;
    rectangle.width = d->size + 2;
    rectangle.height = d->box_height + 2;
    draw_rectangle(ctx, rectangle, 1.0, False, d->bordercolor);

    widget->area.width = d->width;
    widget->area.height = widget->statusbar->height;
    return widget->area.width;
}

static widget_tell_status_t
graph_tell(Widget *widget, char *property, char *command)
{
    Data *d = widget->data;
    int i, u;
    float value;
    char *title, *setting;

    if(!d->data_items)
        return WIDGET_ERROR_CUSTOM; /* error already printed on _new */

    if(command == NULL)
        return WIDGET_ERROR_NOVALUE;

    if(!a_strcmp(property, "data"))
    {
        title = strtok(command, " ");
        if(!(setting = strtok(NULL, " ")))
            return WIDGET_ERROR_NOVALUE;

        for(i = 0; i < d->data_items; i++)
        {
            if(!a_strcmp(title, d->data_title[i]))
            {
                value = MAX(atof(setting), 0);

                if(++d->index[i] >= d->size) /* cycle inside the array */
                    d->index[i] = 0;

                if(d->values[i]) /* scale option is true */
                {
                    d->values[i][d->index[i]] = value;

                    if(value > d->current_max[i]) /* a new maximum value found */
                    {
                        d->max_index[i] = d->index[i];
                        d->current_max[i] = value;

                        /* recalculate */
                        for (u = 0; u < d->size; u++)
                            d->lines[i][u] = (int) (d->values[i][u] * (d->box_height) / d->current_max[i] + 0.5);
                    }
                    /* old max_index reached + current_max > normal, re-check/generate */
                    else if(d->max_index[i] == d->index[i] && d->current_max[i] > d->max[i])
                    {
                        /* find the new max */
                        for (u = 0; u < d->size; u++)
                            if (d->values[i][u] > d->values[i][d->max_index[i]])
                                d->max_index[i] = u;

                        d->current_max[i] = MAX(d->values[i][d->max_index[i]], d->max[i]);

                        /* recalculate */
                        for (u = 0; u < d->size; u++)
                            d->lines[i][u] = (int) (d->values[i][u] * d->box_height / d->current_max[i] + 0.5);
                    }
                    else
                        d->lines[i][d->index[i]] = (int) (value * d->box_height / d->current_max[i] + 0.5);
                }
                else /* scale option is false - limit to d->box_height */
                {
                    if (value < d->current_max[i])
                        d->lines[i][d->index[i]] = (int) (value * d->box_height / d->current_max[i] + 0.5);
                    else
                        d->lines[i][d->index[i]] = d->box_height;
                }
                return WIDGET_NOERROR;
            }
        }
        return WIDGET_ERROR_FORMAT_SECTION;
    }
    else if(!a_strcmp(property, "height"))
        d->height = atof(command);
    else if(!a_strcmp(property, "bg"))
    {
        if(!draw_color_new(globalconf.display,
                           widget->statusbar->phys_screen,
                           command, &d->bg))
            return WIDGET_ERROR_FORMAT_COLOR;
    }
    else if(!a_strcmp(property, "bordercolor"))
    {
        if(!draw_color_new(globalconf.display,
                           widget->statusbar->phys_screen,
                           command, &d->bordercolor))
            return WIDGET_ERROR_FORMAT_COLOR;
    }
    else if(!a_strcmp(property, "grow"))
        switch((d->grow = position_get_from_str(command)))
        {
          case Left:
          case Right:
            break;
          default:
            warn("error changing property %s of widget %s, must be 'left' or 'right'\n",
                 property, widget->name);
            return WIDGET_ERROR_CUSTOM;
        }
    else
        return WIDGET_ERROR;

    return WIDGET_NOERROR;
}

Widget *
graph_new(Statusbar *statusbar, cfg_t *config)
{
    Widget *w;
    Data *d;
    cfg_t *cfg;
    char *color;
    int i;
    char *type;
    XColor tmp_color = { 0, 0, 0, 0, 0, 0 };
    XColor *ptmp_color_center;
    XColor *ptmp_color_end;

    w = p_new(Widget, 1);
    widget_common_new(w, statusbar, config);

    w->draw = graph_draw;
    w->tell = graph_tell;
    w->alignment = cfg_getalignment(config, "align");
    d = w->data = p_new(Data, 1);

    d->width = cfg_getint(config, "width");
    d->height = cfg_getfloat(config, "height");
    d->size = d->width - 2;

    if(!(d->data_items = cfg_size(config, "data")))
    {
        warn("graph widget needs at least one data section\n");
        return w;
    }

    d->grow = cfg_getposition(config, "grow");
    if(d->grow != Left && d->grow != Right)
    {
        warn("graph widget: 'grow' argument must be 'left' or 'right'\n");
        d->data_items = 0; /* disable widget drawing */
        return w;
    }

    d->draw_from = p_new(int, d->size);
    d->draw_to = p_new(int, d->size);

    d->fillbottom = p_new(int *, d->size);
    d->fillbottom_index = p_new(int *, d->size);
    d->filltop = p_new(int *, d->size);
    d->filltop_index = p_new(int *, d->size);
    d->drawline = p_new(int *, d->size);
    d->drawline_index = p_new(int *, d->size);

    d->data_title = p_new(char *, d->data_items);
    d->values = p_new(float *, d->data_items);
    d->lines = p_new(int *, d->data_items);

    d->filltop_color = p_new(XColor, d->data_items);
    d->filltop_pcolor_center = p_new(XColor *, d->data_items);
    d->filltop_pcolor_end = p_new(XColor *, d->data_items);
    d->filltop_vertical_grad = p_new(Bool, d->data_items);
    d->fillbottom_color = p_new(XColor, d->data_items);
    d->fillbottom_pcolor_center = p_new(XColor *, d->data_items);
    d->fillbottom_pcolor_end = p_new(XColor *, d->data_items);
    d->fillbottom_vertical_grad = p_new(Bool, d->data_items);
    d->drawline_color = p_new(XColor, d->data_items);
    d->drawline_pcolor_center = p_new(XColor *, d->data_items);
    d->drawline_pcolor_end = p_new(XColor *, d->data_items);
    d->drawline_vertical_grad = p_new(Bool, d->data_items);

    d->max_index = p_new(int, d->data_items);
    d->index = p_new(int, d->data_items);

    d->current_max = p_new(float, d->data_items);
    d->max = p_new(float, d->data_items);

    for(i = 0; i < d->data_items; i++)
    {
        ptmp_color_center = ptmp_color_end = NULL;

        cfg = cfg_getnsec(config, "data", i);

        d->data_title[i] = a_strdup(cfg_title(cfg));

        if((color = cfg_getstr(cfg, "fg")))
            draw_color_new(globalconf.display, statusbar->phys_screen, color, &tmp_color);
        else
            tmp_color = globalconf.screens[statusbar->screen].styles.normal.fg;

        if((color = cfg_getstr(cfg, "fg_center")))
        {
            ptmp_color_center = p_new(XColor, 1);
            draw_color_new(globalconf.display, statusbar->phys_screen, color, ptmp_color_center);
        }

        if((color = cfg_getstr(cfg, "fg_end")))
        {
            ptmp_color_end = p_new(XColor, 1);
            draw_color_new(globalconf.display, statusbar->phys_screen, color, ptmp_color_end);
        }

        if (cfg_getbool(cfg, "scale"))
            d->values[i] = p_new(float, d->size); /* not null -> scale = true */

        /* prevent: division by zero */
        d->current_max[i] = d->max[i] = cfg_getfloat(cfg, "max");
        d->lines[i] = p_new(int, d->size);

        /* filter each style-typ into it's own array (for easy looping later)*/

        if ((type = cfg_getstr(cfg, "draw_style")))
        {
            if(!a_strncmp(type, "bottom", sizeof("bottom")))
            {
                d->fillbottom[d->fillbottom_total] = d->lines[i];
                d->fillbottom_index[d->fillbottom_total] = &d->index[i];
                d->fillbottom_color[d->fillbottom_total] = tmp_color;
                d->fillbottom_pcolor_center[d->fillbottom_total] = ptmp_color_center;
                d->fillbottom_pcolor_end[d->fillbottom_total] = ptmp_color_end;
                d->fillbottom_vertical_grad[d->fillbottom_total] = cfg_getbool(cfg, "vertical_gradient");
                d->fillbottom_total++;
            }
            else if (!a_strncmp(type, "top", sizeof("top")))
            {
                d->filltop[d->filltop_total] = d->lines[i];
                d->filltop_index[d->filltop_total] = &d->index[i];
                d->filltop_color[d->filltop_total] = tmp_color;
                d->filltop_pcolor_center[d->filltop_total] = ptmp_color_center;
                d->filltop_pcolor_end[d->filltop_total] = ptmp_color_end;
                d->filltop_vertical_grad[d->filltop_total] = cfg_getbool(cfg, "vertical_gradient");
                d->filltop_total++;
            }
            else if (!a_strncmp(type, "line", sizeof("line")))
            {
                d->drawline[d->drawline_total] = d->lines[i];
                d->drawline_index[d->drawline_total] = &d->index[i];
                d->drawline_color[d->drawline_total] = tmp_color;
                d->drawline_pcolor_center[d->drawline_total] = ptmp_color_center;
                d->drawline_pcolor_end[d->drawline_total] = ptmp_color_end;
                d->drawline_vertical_grad[d->drawline_total] = cfg_getbool(cfg, "vertical_gradient");
                d->drawline_total++;
            }
        }
    }

    if((color = cfg_getstr(config, "bg")))
        draw_color_new(globalconf.display, statusbar->phys_screen, color, &d->bg);
    else
        d->bg = globalconf.screens[statusbar->screen].styles.normal.bg;

    if((color = cfg_getstr(config, "bordercolor")))
        draw_color_new(globalconf.display, statusbar->phys_screen, color, &d->bordercolor);
    else
        d->bordercolor = tmp_color;

    return w;
}
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
