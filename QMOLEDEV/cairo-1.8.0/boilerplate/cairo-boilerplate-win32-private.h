/* -*- Mode: c; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 8; -*- */
/*
 * Copyright © 2004,2006 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Red Hat, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. Red Hat, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * RED HAT, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RED HAT, INC. BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Carl D. Worth <cworth@cworth.org>
 */

#ifndef _CAIRO_BOILERPLATE_WIN32_PRIVATE_H_
#define _CAIRO_BOILERPLATE_WIN32_PRIVATE_H_

cairo_surface_t *
_cairo_boilerplate_win32_create_surface (const char			 *name,
					 cairo_content_t		  content,
					 int				  width,
					 int				  height,
					 int				  max_width,
					 int				  max_height,
					 cairo_boilerplate_mode_t	  mode,
					 int                              id,
					 void				**closure);

cairo_surface_t *
_cairo_boilerplate_win32_printing_create_surface (const char		  *name,
						  cairo_content_t	   content,
						  int			   width,
						  int			   height,
						  int			   max_width,
						  int			   max_height,
						  cairo_boilerplate_mode_t mode,
						  int			   id,
						  void			 **closure);

void
_cairo_boilerplate_win32_printing_cleanup (void *closure);

cairo_status_t
_cairo_boilerplate_win32_printing_surface_write_to_png (cairo_surface_t *surface,
							const char      *filename);

cairo_surface_t *
_cairo_boilerplate_win32_printing_get_image_surface (cairo_surface_t *surface,
						     int width,
						     int height);

#endif
