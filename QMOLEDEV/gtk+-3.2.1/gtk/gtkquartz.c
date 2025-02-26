/* gtkquartz.c: Utility functions used by the Quartz port
 *
 * Copyright (C) 2006 Imendio AB
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include "gtkquartz.h"
#include "gtkselectionprivate.h"
#include <gdk/quartz/gdkquartz.h>

NSImage *
_gtk_quartz_create_image_from_pixbuf (GdkPixbuf *pixbuf)
{
  CGColorSpaceRef colorspace;
  CGDataProviderRef data_provider;
  CGContextRef context;
  CGImageRef image;
  void *data;
  int rowstride, pixbuf_width, pixbuf_height;
  gboolean has_alpha;
  NSImage *nsimage;

  pixbuf_width = gdk_pixbuf_get_width (pixbuf);
  pixbuf_height = gdk_pixbuf_get_height (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  has_alpha = gdk_pixbuf_get_has_alpha (pixbuf);

  data = gdk_pixbuf_get_pixels (pixbuf);

  colorspace = CGColorSpaceCreateDeviceRGB ();
  data_provider = CGDataProviderCreateWithData (NULL, data, pixbuf_height * rowstride, NULL);

  image = CGImageCreate (pixbuf_width, pixbuf_height, 8,
			 has_alpha ? 32 : 24, rowstride, 
			 colorspace, 
			 has_alpha ? kCGImageAlphaLast : 0,
			 data_provider, NULL, FALSE, 
			 kCGRenderingIntentDefault);

  CGDataProviderRelease (data_provider);
  CGColorSpaceRelease (colorspace);

  nsimage = [[NSImage alloc] initWithSize:NSMakeSize (pixbuf_width, pixbuf_height)];
  [nsimage lockFocus];

  context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
  CGContextDrawImage (context, CGRectMake (0, 0, pixbuf_width, pixbuf_height), image);
 
  [nsimage unlockFocus];

  CGImageRelease (image);

  return nsimage;
}

static NSString *
target_to_pasteboard_type (const char *target)
{
  if (strcmp (target, "UTF8_STRING") == 0)
    return NSStringPboardType;
  else if (strcmp (target, "image/tiff") == 0)
    return NSTIFFPboardType;
  else if (strcmp (target, "application/x-color") == 0)
    return NSColorPboardType;
  else if (strcmp (target, "text/uri-list") == 0)
    return NSURLPboardType;
  else
    return [NSString stringWithUTF8String:target];
}

NSSet *
_gtk_quartz_target_list_to_pasteboard_types (GtkTargetList *target_list)
{
  NSMutableSet *set = [[NSMutableSet alloc] init];
  GList *list;

  for (list = target_list->list; list; list = list->next)
    {
      GtkTargetPair *pair = list->data;
      gchar *target = gdk_atom_name (pair->target);
      [set addObject:target_to_pasteboard_type (target)];
      g_free (target);
    }

  return set;
}

NSSet *
_gtk_quartz_target_entries_to_pasteboard_types (const GtkTargetEntry *targets,
						guint                 n_targets)
{
  NSMutableSet *set = [[NSMutableSet alloc] init];
  int i;

  for (i = 0; i < n_targets; i++)
    {
      [set addObject:target_to_pasteboard_type (targets[i].target)];
    }

  return set;
}

GdkAtom 
_gtk_quartz_pasteboard_type_to_atom (NSString *type)
{
  if ([type isEqualToString:NSStringPboardType])
    return gdk_atom_intern_static_string ("UTF8_STRING");
  else if ([type isEqualToString:NSTIFFPboardType])
    return gdk_atom_intern_static_string ("image/tiff");
  else if ([type isEqualToString:NSColorPboardType])
    return gdk_atom_intern_static_string ("application/x-color");
  else if ([type isEqualToString:NSURLPboardType])
    return gdk_atom_intern_static_string ("text/uri-list");
  else
    return gdk_atom_intern ([type UTF8String], FALSE);  
}

GList *
_gtk_quartz_pasteboard_types_to_atom_list (NSArray *array)
{
  GList *result = NULL;
  int i;
  int count;

  count = [array count];

  for (i = 0; i < count; i++) 
    {
      GdkAtom atom = _gtk_quartz_pasteboard_type_to_atom ([array objectAtIndex:i]);

      result = g_list_prepend (result, GDK_ATOM_TO_POINTER (atom));
    }

  return result;
}

GtkSelectionData *
_gtk_quartz_get_selection_data_from_pasteboard (NSPasteboard *pasteboard,
						GdkAtom       target,
						GdkAtom       selection)
{
  GtkSelectionData *selection_data = NULL;

  selection_data = g_slice_new0 (GtkSelectionData);
  selection_data->selection = selection;
  selection_data->target = target;
  if (!selection_data->display)
    selection_data->display = gdk_display_get_default ();
  if (target == gdk_atom_intern_static_string ("UTF8_STRING"))
    {
      NSString *s = [pasteboard stringForType:NSStringPboardType];

      if (s)
	{
          const char *utf8_string = [s UTF8String];

          gtk_selection_data_set (selection_data,
                                  target, 8,
                                  (guchar *)utf8_string, strlen (utf8_string));
	}
    }
  else if (target == gdk_atom_intern_static_string ("application/x-color"))
    {
      NSColor *nscolor = [[NSColor colorFromPasteboard:pasteboard]
                          colorUsingColorSpaceName:NSDeviceRGBColorSpace];
      
      guint16 color[4];
      
      selection_data->target = target;

      color[0] = 0xffff * [nscolor redComponent];
      color[1] = 0xffff * [nscolor greenComponent];
      color[2] = 0xffff * [nscolor blueComponent];
      color[3] = 0xffff * [nscolor alphaComponent];

      gtk_selection_data_set (selection_data, target, 16, (guchar *)color, 8);
    }
  else if (target == gdk_atom_intern_static_string ("text/uri-list"))
    {
      if ([[pasteboard types] containsObject:NSFilenamesPboardType])
        {
           gchar **uris;
           NSArray *files = [pasteboard propertyListForType:NSFilenamesPboardType];
           int n_files = [files count];
           int i;

           selection_data->target = gdk_atom_intern_static_string ("text/uri-list");

           uris = (gchar **) g_malloc (sizeof (gchar*) * (n_files + 1));
           for (i = 0; i < n_files; ++i)
             {
               NSString* uriString = [files objectAtIndex:i];
               uriString = [@"file://" stringByAppendingString:uriString];
               uriString = [uriString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
               uris[i] = (gchar *) [uriString cStringUsingEncoding:NSUTF8StringEncoding];
             }
           uris[i] = NULL;

           gtk_selection_data_set_uris (selection_data, uris);
           g_free (uris);
         }
      else if ([[pasteboard types] containsObject:NSURLPboardType])
        {
          gchar *uris[2];
          NSURL *url = [NSURL URLFromPasteboard:pasteboard];

          selection_data->target = gdk_atom_intern_static_string ("text/uri-list");

          uris[0] = (gchar *) [[url description] UTF8String];

          uris[1] = NULL;
          gtk_selection_data_set_uris (selection_data, uris);
        }
    }
  else
    {
      NSData *data;
      gchar *name;

      name = gdk_atom_name (target);

      if (strcmp (name, "image/tiff") == 0)
	data = [pasteboard dataForType:NSTIFFPboardType];
      else
	data = [pasteboard dataForType:[NSString stringWithUTF8String:name]];

      g_free (name);

      if (data)
	{
	  gtk_selection_data_set (selection_data,
                                  target, 8,
                                  [data bytes], [data length]);
	}
    }

  return selection_data;
}

void
_gtk_quartz_set_selection_data_for_pasteboard (NSPasteboard     *pasteboard,
					       GtkSelectionData *selection_data)
{
  NSString *type;
  gchar *target;
  GdkDisplay *display;
  gint format;
  const guchar *data;
  NSUInteger length;

  target = gdk_atom_name (gtk_selection_data_get_target (selection_data));
  display = gtk_selection_data_get_display (selection_data);
  format = gtk_selection_data_get_format (selection_data);
  data = gtk_selection_data_get_data (selection_data);
  length = gtk_selection_data_get_length (selection_data);

  type = target_to_pasteboard_type (target);
  g_free (target);

  if ([type isEqualTo:NSStringPboardType]) 
    [pasteboard setString:[NSString stringWithUTF8String:(const char *)data]
                  forType:type];
  else if ([type isEqualTo:NSColorPboardType])
    {
      guint16 *color = (guint16 *)data;
      float red, green, blue, alpha;
      NSColor *nscolor;

      red   = (float)color[0] / 0xffff;
      green = (float)color[1] / 0xffff;
      blue  = (float)color[2] / 0xffff;
      alpha = (float)color[3] / 0xffff;

      nscolor = [NSColor colorWithDeviceRed:red green:green blue:blue alpha:alpha];
      [nscolor writeToPasteboard:pasteboard];
    }
  else if ([type isEqualTo:NSURLPboardType])
    {
      gchar **uris;

      uris = gtk_selection_data_get_uris (selection_data);
      if (uris != NULL)
        {
          NSURL *url;

          url = [NSURL URLWithString:[NSString stringWithUTF8String:uris[0]]];
          [url writeToPasteboard:pasteboard];
        }
      g_strfreev (uris);
    }
  else
    [pasteboard setData:[NSData dataWithBytesNoCopy:(void *)data
                                             length:length
                                       freeWhenDone:NO]
                                            forType:type];
}

/*
 * Bundle-based functions for various directories. These almost work
 * even when the application isn't in a bundle, becuase mainBundle
 * paths point to the bin directory in that case. It's a simple matter
 * to test for that and remove the last element.
 */

static gchar *
get_bundle_path()
{
  gchar *base, *path;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  gchar *resource_path = g_strdup([[[NSBundle mainBundle] resourcePath] UTF8String]);
  [pool drain];
  base = g_path_get_basename(resource_path);
  if (strcmp(base, "bin") == 0) 
    path = g_path_get_dirname(resource_path);
  else
    path = strdup(resource_path);
  g_free(resource_path);
  g_free(base);
  return path;
}

const gchar *
_gtk_get_datadir (void)
{
  gchar *resource_dir = get_bundle_path();
  gchar *retval = g_build_filename(resource_dir, "share", NULL);
  g_free(resource_dir);
  return retval;
}

const gchar *
_gtk_get_libdir (void)
{
  gchar *resource_dir = get_bundle_path();
  gchar *retval = g_build_filename(resource_dir, "lib", NULL);
  g_free(resource_dir);
  return retval;
}

const gchar *
_gtk_get_localedir (void)
{

  gchar *resource_dir = get_bundle_path();
  gchar *retval = g_build_filename(resource_dir, "share", "locale", NULL);
  g_free(resource_dir);
  return retval;
}

const gchar *
_gtk_get_sysconfdir (void)
{
  gchar *resource_dir = get_bundle_path();
  gchar *retval = g_build_filename(resource_dir, "etc", NULL);
  g_free(resource_dir);
  return retval;
}

const gchar *
_gtk_get_data_prefix (void)
{
  return get_bundle_path();
}

