/*
 * gnome-keyring
 *
 * Copyright (C) 2010 Stefan Walter
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef __GCR_ICONS_H__
#define __GCR_ICONS_H__

#include "gcr-internal.h"

G_BEGIN_DECLS

#define GCR_ICON_CERTIFICATE    "application-certificate"
#define GCR_ICON_KEY            "gcr-key"
#define GCR_ICON_KEY_PAIR       "gcr-key-pair"

void         _gcr_icons_register         (void);

G_END_DECLS

#endif /* __GCR_TOKEN_MANAGER_H__ */
