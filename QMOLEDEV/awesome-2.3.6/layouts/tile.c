/*
 * tile.c - tile layout
 *
 * Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
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

#include <stdio.h>

#include "screen.h"
#include "awesome.h"
#include "tag.h"
#include "layout.h"
#include "client.h"
#include "layouts/tile.h"
#include "common/util.h"

extern AwesomeConf globalconf;

void
uicb_tag_setnmaster(int screen, char * arg)
{
    Tag **curtags = tags_get_current(screen);
    Layout *curlay = curtags[0]->layout;
    Client *c;
    int n;

    if(!arg || (curlay->arrange != layout_tile
                && curlay->arrange != layout_tileleft
                && curlay->arrange != layout_tilebottom
                && curlay->arrange != layout_tiletop))
        return;

    if((curtags[0]->nmaster = (int) compute_new_value_from_arg(arg, (double) curtags[0]->nmaster)) < 0)
        curtags[0]->nmaster = 0;

    for(n = 0, c = globalconf.clients; c; c = c->next)
        if(IS_TILED(c, screen))
            n++;

    if(curtags[0]->nmaster > n - 1)
        curtags[0]->nmaster = n;

    p_delete(&curtags);

    globalconf.screens[screen].need_arrange = True;
}

void
uicb_tag_setncol(int screen, char * arg)
{
    Tag **curtags = tags_get_current(screen);
    Layout *curlay = curtags[0]->layout;
    Client *c;
    int n;

    if(!arg || (curlay->arrange != layout_tile
                && curlay->arrange != layout_tileleft
                && curlay->arrange != layout_tilebottom
                && curlay->arrange != layout_tiletop))
        return;


    if((curtags[0]->ncol = (int) compute_new_value_from_arg(arg, (double) curtags[0]->ncol)) < 1)
        curtags[0]->ncol = 1;

    for(n = 0, c = globalconf.clients; c; c = c->next)
        if(IS_TILED(c, screen))
            n++;

    if(curtags[0]->ncol > n - 1)
    	curtags[0]->ncol = n - 1;

    p_delete(&curtags);

    globalconf.screens[screen].need_arrange = True;
}

void
uicb_tag_setmwfact(int screen, char *arg)
{
    char *newarg;
    Tag **curtags = tags_get_current(screen);
    Layout *curlay = curtags[0]->layout;

    if(!arg || (curlay->arrange != layout_tile
                && curlay->arrange != layout_tileleft
                && curlay->arrange != layout_tilebottom
                && curlay->arrange != layout_tiletop))
        return;

    newarg = a_strdup(arg);
    if(curlay->arrange == layout_tileleft || curlay->arrange == layout_tiletop)
    {
        if(newarg[0] == '+')
            newarg[0] = '-';
        else if(arg[0] == '-')
            newarg[0] = '+';
    }

    curtags[0]->mwfact = compute_new_value_from_arg(newarg, curtags[0]->mwfact);

    if(curtags[0]->mwfact < globalconf.screens[screen].mwfact_lower_limit)
        curtags[0]->mwfact = globalconf.screens[screen].mwfact_lower_limit;
    else if(curtags[0]->mwfact > globalconf.screens[screen].mwfact_upper_limit)
        curtags[0]->mwfact = globalconf.screens[screen].mwfact_upper_limit;

    p_delete(&newarg);
    p_delete(&curtags);
    globalconf.screens[screen].need_arrange = True;
}

static void
_tile(int screen, const Position position)
{
    /* windows area geometry */
    int wah = 0, waw = 0, wax = 0, way = 0;
    /* master size */
    unsigned int mw = 0, mh = 0;
    int n, i, masterwin = 0, otherwin = 0;
    int real_ncol = 1, win_by_col = 1, current_col = 0;
    area_t area, geometry = { 0, 0, 0, 0, NULL, NULL };
    Client *c;
    Tag **curtags = tags_get_current(screen);

    area = screen_get_area(screen,
                           globalconf.screens[screen].statusbar,
                            &globalconf.screens[screen].padding);

    for(n = 0, c = globalconf.clients; c; c = c->next)
        if(IS_TILED(c, screen))
            n++;

    wah = area.height;
    waw = area.width;
    wax = area.x;
    way = area.y;

    masterwin = MIN(n, curtags[0]->nmaster);

    otherwin = n - masterwin;

    if(otherwin < 0)
        otherwin = 0;

    if(curtags[0]->nmaster)
        switch(position)
        {
          case Right:
          case Left:
            mh = masterwin ? wah / masterwin : wah;
            mw = otherwin ? waw * curtags[0]->mwfact : waw;
            break;
          default:
            mh = otherwin ? wah * curtags[0]->mwfact : wah;
            mw = masterwin ? waw / masterwin : waw;
            break;
        }
    else
        mh = mw = 0;

    real_ncol = curtags[0]->ncol > 0 ? MIN(otherwin, curtags[0]->ncol) : MIN(otherwin, 1);

    for(i = 0, c = globalconf.clients; c; c = c->next)
    {
        if(!IS_TILED(c, screen))
            continue;

        if(i < curtags[0]->nmaster)
        {
            switch(position)
            {
              case Right:
                geometry.y = way + i * mh;
                geometry.x = wax;
                break;
              case Left:
                geometry.y = way + i * mh;
                geometry.x = wax + (waw - mw);
                break;
              case Top:
                geometry.x = wax + i * mw;
                geometry.y = way + (wah - mh);
                break;
              case Bottom:
              default:
                geometry.x = wax + i * mw;
                geometry.y = way;
                break;
                break;
            }

            geometry.width = mw - 2 * c->border;
            geometry.height =  mh - 2 * c->border;

            client_resize(c, geometry, globalconf.screens[screen].resize_hints);
        }
        else
        {
            if(real_ncol)
                win_by_col = otherwin / real_ncol;

            if((i - curtags[0]->nmaster) && (i - curtags[0]->nmaster) % win_by_col == 0 && current_col < real_ncol - 1)
                current_col++;

            if(current_col == real_ncol - 1)
                win_by_col += otherwin % real_ncol;

            if(position == Right || position == Left)
            {
                if(otherwin <= real_ncol)
                    geometry.height = wah - 2 * c->border;
                else
                    geometry.height = (wah / win_by_col) - 2 * c->border;

                geometry.width = (waw - mw) / real_ncol - 2 * c->border;

                if(i == curtags[0]->nmaster || otherwin <= real_ncol || (i - curtags[0]->nmaster) % win_by_col == 0)
                    geometry.y = way;
                else
                    geometry.y = way + ((i - curtags[0]->nmaster) % win_by_col) * (geometry.height + 2 * c->border);

                geometry.x = wax + current_col * (geometry.width + 2 * c->border);

                if(position == Right)
                    geometry.x += mw;
            }
            else
            {
                if(otherwin <= real_ncol)
                    geometry.width = waw - 2 * c->border;
                else
                    geometry.width = (waw / win_by_col) - 2 * c->border;

                geometry.height = (wah - mh) / real_ncol - 2 * c->border;

                if(i == curtags[0]->nmaster || otherwin <= real_ncol || (i - curtags[0]->nmaster) % win_by_col == 0)
                    geometry.x = wax;
                else
                    geometry.x = wax + ((i - curtags[0]->nmaster) % win_by_col) * (geometry.width + 2 * c->border);

                geometry.y = way + current_col * (geometry.height + 2 * c->border);

                if(position == Bottom)
                    geometry.y += mh;
            }
            client_resize(c, geometry, globalconf.screens[screen].resize_hints);
        }
        i++;
    }

    p_delete(&curtags);
}

void
layout_tile(int screen)
{
    _tile(screen, Right);
}

void
layout_tileleft(int screen)
{
    _tile(screen, Left);
}

void
layout_tilebottom(int screen)
{
    _tile(screen, Bottom);
}

void
layout_tiletop(int screen)
{
    _tile(screen, Top);
}

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
