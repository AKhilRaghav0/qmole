/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include "config.h"

#include "gdkinternals.h"
#include "gdkdisplayprivate.h"

#include <string.h>
#include <math.h>


/**
 * SECTION:events
 * @Short_description: Functions for handling events from the window system
 * @Title: Events
 * @See_also: <link linkend="gdk-Event-Structures">Event Structures</link>
 *
 * This section describes functions dealing with events from the window
 * system.
 *
 * In GTK+ applications the events are handled automatically in
 * gtk_main_do_event() and passed on to the appropriate widgets, so these
 * functions are rarely needed. Though some of the fields in the
 * <link linkend="gdk-Event-Structures">Event Structures</link> are useful.
 */


typedef struct _GdkIOClosure GdkIOClosure;

struct _GdkIOClosure
{
  GDestroyNotify notify;
  gpointer data;
};

/* Private variable declarations
 */

static GdkEventFunc   _gdk_event_func = NULL;    /* Callback for events */
static gpointer       _gdk_event_data = NULL;
static GDestroyNotify _gdk_event_notify = NULL;

void
_gdk_event_emit (GdkEvent *event)
{
  if (_gdk_event_func)
    (*_gdk_event_func) (event, _gdk_event_data);
}

/*********************************************
 * Functions for maintaining the event queue *
 *********************************************/

/**
 * _gdk_event_queue_find_first:
 * @display: a #GdkDisplay
 * 
 * Find the first event on the queue that is not still
 * being filled in.
 * 
 * Return value: Pointer to the list node for that event, or NULL.
 **/
GList*
_gdk_event_queue_find_first (GdkDisplay *display)
{
  GList *tmp_list = display->queued_events;

  while (tmp_list)
    {
      GdkEventPrivate *event = tmp_list->data;
      if (!(event->flags & GDK_EVENT_PENDING))
	return tmp_list;

      tmp_list = g_list_next (tmp_list);
    }

  return NULL;
}

/**
 * _gdk_event_queue_prepend:
 * @display: a #GdkDisplay
 * @event: Event to prepend.
 *
 * Prepends an event before the head of the event queue.
 *
 * Returns: the newly prepended list node.
 **/
GList*
_gdk_event_queue_prepend (GdkDisplay *display,
			  GdkEvent   *event)
{
  display->queued_events = g_list_prepend (display->queued_events, event);
  if (!display->queued_tail)
    display->queued_tail = display->queued_events;
  return display->queued_events;
}

/**
 * _gdk_event_queue_append:
 * @display: a #GdkDisplay
 * @event: Event to append.
 * 
 * Appends an event onto the tail of the event queue.
 *
 * Returns: the newly appended list node.
 **/
GList *
_gdk_event_queue_append (GdkDisplay *display,
			 GdkEvent   *event)
{
  display->queued_tail = g_list_append (display->queued_tail, event);
  
  if (!display->queued_events)
    display->queued_events = display->queued_tail;
  else
    display->queued_tail = display->queued_tail->next;

  return display->queued_tail;
}

/**
 * _gdk_event_queue_insert_after:
 * @display: a #GdkDisplay
 * @sibling: Append after this event.
 * @event: Event to append.
 *
 * Appends an event after the specified event, or if it isn't in
 * the queue, onto the tail of the event queue.
 *
 * Returns: the newly appended list node.
 *
 * Since: 2.16
 */
GList*
_gdk_event_queue_insert_after (GdkDisplay *display,
                               GdkEvent   *sibling,
                               GdkEvent   *event)
{
  GList *prev = g_list_find (display->queued_events, sibling);
  if (prev && prev->next)
    {
      display->queued_events = g_list_insert_before (display->queued_events, prev->next, event);
      return prev->next;
    }
  else
    return _gdk_event_queue_append (display, event);
}

/**
 * _gdk_event_queue_insert_after:
 * @display: a #GdkDisplay
 * @sibling: Append after this event.
 * @event: Event to append.
 *
 * Appends an event before the specified event, or if it isn't in
 * the queue, onto the tail of the event queue.
 *
 * Returns: the newly appended list node.
 *
 * Since: 2.16
 */
GList*
_gdk_event_queue_insert_before (GdkDisplay *display,
				GdkEvent   *sibling,
				GdkEvent   *event)
{
  GList *next = g_list_find (display->queued_events, sibling);
  if (next)
    {
      display->queued_events = g_list_insert_before (display->queued_events, next, event);
      return next->prev;
    }
  else
    return _gdk_event_queue_append (display, event);
}


/**
 * _gdk_event_queue_remove_link:
 * @display: a #GdkDisplay
 * @node: node to remove
 * 
 * Removes a specified list node from the event queue.
 **/
void
_gdk_event_queue_remove_link (GdkDisplay *display,
			      GList      *node)
{
  if (node->prev)
    node->prev->next = node->next;
  else
    display->queued_events = node->next;
  
  if (node->next)
    node->next->prev = node->prev;
  else
    display->queued_tail = node->prev;
}

/**
 * _gdk_event_unqueue:
 * @display: a #GdkDisplay
 * 
 * Removes and returns the first event from the event
 * queue that is not still being filled in.
 * 
 * Return value: the event, or %NULL. Ownership is transferred
 * to the caller.
 **/
GdkEvent*
_gdk_event_unqueue (GdkDisplay *display)
{
  GdkEvent *event = NULL;
  GList *tmp_list;

  tmp_list = _gdk_event_queue_find_first (display);

  if (tmp_list)
    {
      event = tmp_list->data;
      _gdk_event_queue_remove_link (display, tmp_list);
      g_list_free_1 (tmp_list);
    }

  return event;
}

/**
 * gdk_event_handler_set:
 * @func: the function to call to handle events from GDK.
 * @data: user data to pass to the function. 
 * @notify: the function to call when the handler function is removed, i.e. when
 *          gdk_event_handler_set() is called with another event handler.
 * 
 * Sets the function to call to handle all events from GDK.
 *
 * Note that GTK+ uses this to install its own event handler, so it is
 * usually not useful for GTK+ applications. (Although an application
 * can call this function then call gtk_main_do_event() to pass
 * events to GTK+.)
 **/
void 
gdk_event_handler_set (GdkEventFunc   func,
		       gpointer       data,
		       GDestroyNotify notify)
{
  if (_gdk_event_notify)
    (*_gdk_event_notify) (_gdk_event_data);

  _gdk_event_func = func;
  _gdk_event_data = data;
  _gdk_event_notify = notify;
}

/**
 * gdk_events_pending:
 *
 * Checks if any events are ready to be processed for any display.
 *
 * Return value: %TRUE if any events are pending.
 */
gboolean
gdk_events_pending (void)
{
  GSList *list, *l;
  gboolean pending;

  pending = FALSE;
  list = gdk_display_manager_list_displays (gdk_display_manager_get ());
  for (l = list; l; l = l->next)
    {
      if (_gdk_event_queue_find_first (l->data))
        {
          pending = TRUE;
          goto out;
        }
    }

  for (l = list; l; l = l->next)
    {
      if (gdk_display_has_pending (l->data))
        {
          pending = TRUE;
          goto out;
        }
    }

 out:
  g_slist_free (list);

  return pending;
}

/**
 * gdk_event_get:
 * 
 * Checks all open displays for a #GdkEvent to process,to be processed
 * on, fetching events from the windowing system if necessary.
 * See gdk_display_get_event().
 * 
 * Return value: the next #GdkEvent to be processed, or %NULL if no events
 * are pending. The returned #GdkEvent should be freed with gdk_event_free().
 **/
GdkEvent*
gdk_event_get (void)
{
  GSList *list, *l;
  GdkEvent *event;

  event = NULL;
  list = gdk_display_manager_list_displays (gdk_display_manager_get ());
  for (l = list; l; l = l->next)
    {
      event = gdk_display_get_event (l->data);
      if (event)
        break;
    }

  g_slist_free (list);

  return event;
}

/**
 * gdk_event_peek:
 *
 * If there is an event waiting in the event queue of some open
 * display, returns a copy of it. See gdk_display_peek_event().
 * 
 * Return value: a copy of the first #GdkEvent on some event queue, or %NULL if no
 * events are in any queues. The returned #GdkEvent should be freed with
 * gdk_event_free().
 **/
GdkEvent*
gdk_event_peek (void)
{
  GSList *list, *l;
  GdkEvent *event;

  event = NULL;
  list = gdk_display_manager_list_displays (gdk_display_manager_get ());
  for (l = list; l; l = l->next)
    {
      event = gdk_display_peek_event (l->data);
      if (event)
        break;
    }

  g_slist_free (list);

  return event;
}

/**
 * gdk_event_put:
 * @event: a #GdkEvent.
 *
 * Appends a copy of the given event onto the front of the event
 * queue for event->any.window's display, or the default event
 * queue if event->any.window is %NULL. See gdk_display_put_event().
 **/
void
gdk_event_put (const GdkEvent *event)
{
  GdkDisplay *display;
  
  g_return_if_fail (event != NULL);

  if (event->any.window)
    display = gdk_window_get_display (event->any.window);
  else
    {
      GDK_NOTE (MULTIHEAD,
		g_message ("Falling back to default display for gdk_event_put()"));
      display = gdk_display_get_default ();
    }

  gdk_display_put_event (display, event);
}

static GHashTable *event_hash = NULL;

/**
 * gdk_event_new:
 * @type: a #GdkEventType 
 * 
 * Creates a new event of the given type. All fields are set to 0.
 * 
 * Return value: a newly-allocated #GdkEvent. The returned #GdkEvent 
 * should be freed with gdk_event_free().
 *
 * Since: 2.2
 **/
GdkEvent*
gdk_event_new (GdkEventType type)
{
  GdkEventPrivate *new_private;
  GdkEvent *new_event;
  
  if (!event_hash)
    event_hash = g_hash_table_new (g_direct_hash, NULL);

  new_private = g_slice_new0 (GdkEventPrivate);
  
  new_private->flags = 0;
  new_private->screen = NULL;

  g_hash_table_insert (event_hash, new_private, GUINT_TO_POINTER (1));

  new_event = (GdkEvent *) new_private;

  new_event->any.type = type;

  /*
   * Bytewise 0 initialization is reasonable for most of the 
   * current event types. Explicitely initialize double fields
   * since I trust bytewise 0 == 0. less than for integers
   * or pointers.
   */
  switch (type)
    {
    case GDK_MOTION_NOTIFY:
      new_event->motion.x = 0.;
      new_event->motion.y = 0.;
      new_event->motion.x_root = 0.;
      new_event->motion.y_root = 0.;
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      new_event->button.x = 0.;
      new_event->button.y = 0.;
      new_event->button.x_root = 0.;
      new_event->button.y_root = 0.;
      break;
    case GDK_SCROLL:
      new_event->scroll.x = 0.;
      new_event->scroll.y = 0.;
      new_event->scroll.x_root = 0.;
      new_event->scroll.y_root = 0.;
      break;
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      new_event->crossing.x = 0.;
      new_event->crossing.y = 0.;
      new_event->crossing.x_root = 0.;
      new_event->crossing.y_root = 0.;
      break;
    default:
      break;
    }
  
  return new_event;
}

static gboolean
gdk_event_is_allocated (const GdkEvent *event)
{
  if (event_hash)
    return g_hash_table_lookup (event_hash, event) != NULL;

  return FALSE;
}
 
/**
 * gdk_event_copy:
 * @event: a #GdkEvent
 * 
 * Copies a #GdkEvent, copying or incrementing the reference count of the
 * resources associated with it (e.g. #GdkWindow's and strings).
 * 
 * Return value: a copy of @event. The returned #GdkEvent should be freed with
 * gdk_event_free().
 **/
GdkEvent*
gdk_event_copy (const GdkEvent *event)
{
  GdkEventPrivate *new_private;
  GdkEvent *new_event;

  g_return_val_if_fail (event != NULL, NULL);

  new_event = gdk_event_new (GDK_NOTHING);
  new_private = (GdkEventPrivate *)new_event;

  *new_event = *event;
  if (new_event->any.window)
    g_object_ref (new_event->any.window);

  if (gdk_event_is_allocated (event))
    {
      GdkEventPrivate *private = (GdkEventPrivate *)event;

      new_private->screen = private->screen;
      new_private->device = private->device;
      new_private->source_device = private->source_device;
    }

  switch (event->any.type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      new_event->key.string = g_strdup (event->key.string);
      break;

    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      if (event->crossing.subwindow != NULL)
        g_object_ref (event->crossing.subwindow);
      break;

    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DRAG_STATUS:
    case GDK_DROP_START:
    case GDK_DROP_FINISHED:
      g_object_ref (event->dnd.context);
      break;

    case GDK_EXPOSE:
    case GDK_DAMAGE:
      if (event->expose.region)
        new_event->expose.region = cairo_region_copy (event->expose.region);
      break;

    case GDK_SETTING:
      new_event->setting.name = g_strdup (new_event->setting.name);
      break;

    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      if (event->button.axes)
        new_event->button.axes = g_memdup (event->button.axes,
                                           sizeof (gdouble) * gdk_device_get_n_axes (event->button.device));
      break;

    case GDK_MOTION_NOTIFY:
      if (event->motion.axes)
        new_event->motion.axes = g_memdup (event->motion.axes,
                                           sizeof (gdouble) * gdk_device_get_n_axes (event->motion.device));
      break;

    case GDK_OWNER_CHANGE:
      new_event->owner_change.owner = event->owner_change.owner;
      if (new_event->owner_change.owner)
        g_object_ref (new_event->owner_change.owner);
      break;

    case GDK_SELECTION_CLEAR:
    case GDK_SELECTION_NOTIFY:
    case GDK_SELECTION_REQUEST:
      new_event->selection.requestor = event->selection.requestor;
      if (new_event->selection.requestor)
        g_object_unref (new_event->selection.requestor);
      break;

    default:
      break;
    }

  if (gdk_event_is_allocated (event))
    _gdk_display_event_data_copy (gdk_display_get_default (), event, new_event);

  return new_event;
}

/**
 * gdk_event_free:
 * @event:  a #GdkEvent.
 * 
 * Frees a #GdkEvent, freeing or decrementing any resources associated with it.
 * Note that this function should only be called with events returned from
 * functions such as gdk_event_peek(), gdk_event_get(), gdk_event_copy()
 * and gdk_event_new().
 **/
void
gdk_event_free (GdkEvent *event)
{
  GdkDisplay *display;

  g_return_if_fail (event != NULL);

  if (event->any.window)
    g_object_unref (event->any.window);
  
  switch (event->any.type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      g_free (event->key.string);
      break;
      
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      if (event->crossing.subwindow != NULL)
	g_object_unref (event->crossing.subwindow);
      break;
      
    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DRAG_STATUS:
    case GDK_DROP_START:
    case GDK_DROP_FINISHED:
      if (event->dnd.context != NULL)
        g_object_unref (event->dnd.context);
      break;

    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      g_free (event->button.axes);
      break;
      
    case GDK_EXPOSE:
    case GDK_DAMAGE:
      if (event->expose.region)
	cairo_region_destroy (event->expose.region);
      break;
      
    case GDK_MOTION_NOTIFY:
      g_free (event->motion.axes);
      break;
      
    case GDK_SETTING:
      g_free (event->setting.name);
      break;
      
    case GDK_OWNER_CHANGE:
      if (event->owner_change.owner)
        g_object_unref (event->owner_change.owner);
      break;

    case GDK_SELECTION_CLEAR:
    case GDK_SELECTION_NOTIFY:
    case GDK_SELECTION_REQUEST:
      if (event->selection.requestor)
        g_object_unref (event->selection.requestor);
      break;

    default:
      break;
    }

  display = gdk_display_get_default ();
  if (display)
    _gdk_display_event_data_free (display, event);

  g_hash_table_remove (event_hash, event);
  g_slice_free (GdkEventPrivate, (GdkEventPrivate*) event);
}

/**
 * gdk_event_get_time:
 * @event: a #GdkEvent
 * 
 * Returns the time stamp from @event, if there is one; otherwise
 * returns #GDK_CURRENT_TIME. If @event is %NULL, returns #GDK_CURRENT_TIME.
 * 
 * Return value: time stamp field from @event
 **/
guint32
gdk_event_get_time (const GdkEvent *event)
{
  if (event)
    switch (event->type)
      {
      case GDK_MOTION_NOTIFY:
	return event->motion.time;
      case GDK_BUTTON_PRESS:
      case GDK_2BUTTON_PRESS:
      case GDK_3BUTTON_PRESS:
      case GDK_BUTTON_RELEASE:
	return event->button.time;
      case GDK_SCROLL:
        return event->scroll.time;
      case GDK_KEY_PRESS:
      case GDK_KEY_RELEASE:
	return event->key.time;
      case GDK_ENTER_NOTIFY:
      case GDK_LEAVE_NOTIFY:
	return event->crossing.time;
      case GDK_PROPERTY_NOTIFY:
	return event->property.time;
      case GDK_SELECTION_CLEAR:
      case GDK_SELECTION_REQUEST:
      case GDK_SELECTION_NOTIFY:
	return event->selection.time;
      case GDK_PROXIMITY_IN:
      case GDK_PROXIMITY_OUT:
	return event->proximity.time;
      case GDK_DRAG_ENTER:
      case GDK_DRAG_LEAVE:
      case GDK_DRAG_MOTION:
      case GDK_DRAG_STATUS:
      case GDK_DROP_START:
      case GDK_DROP_FINISHED:
	return event->dnd.time;
      case GDK_CLIENT_EVENT:
      case GDK_VISIBILITY_NOTIFY:
      case GDK_CONFIGURE:
      case GDK_FOCUS_CHANGE:
      case GDK_NOTHING:
      case GDK_DAMAGE:
      case GDK_DELETE:
      case GDK_DESTROY:
      case GDK_EXPOSE:
      case GDK_MAP:
      case GDK_UNMAP:
      case GDK_WINDOW_STATE:
      case GDK_SETTING:
      case GDK_OWNER_CHANGE:
      case GDK_GRAB_BROKEN:
      case GDK_EVENT_LAST:
        /* return current time */
        break;
      }
  
  return GDK_CURRENT_TIME;
}

/**
 * gdk_event_get_state:
 * @event: a #GdkEvent or NULL
 * @state: (out): return location for state
 * 
 * If the event contains a "state" field, puts that field in @state. Otherwise
 * stores an empty state (0). Returns %TRUE if there was a state field
 * in the event. @event may be %NULL, in which case it's treated
 * as if the event had no state field.
 * 
 * Return value: %TRUE if there was a state field in the event 
 **/
gboolean
gdk_event_get_state (const GdkEvent        *event,
                     GdkModifierType       *state)
{
  g_return_val_if_fail (state != NULL, FALSE);
  
  if (event)
    switch (event->type)
      {
      case GDK_MOTION_NOTIFY:
	*state = event->motion.state;
        return TRUE;
      case GDK_BUTTON_PRESS:
      case GDK_2BUTTON_PRESS:
      case GDK_3BUTTON_PRESS:
      case GDK_BUTTON_RELEASE:
        *state =  event->button.state;
        return TRUE;
      case GDK_SCROLL:
	*state =  event->scroll.state;
        return TRUE;
      case GDK_KEY_PRESS:
      case GDK_KEY_RELEASE:
	*state =  event->key.state;
        return TRUE;
      case GDK_ENTER_NOTIFY:
      case GDK_LEAVE_NOTIFY:
	*state =  event->crossing.state;
        return TRUE;
      case GDK_PROPERTY_NOTIFY:
      case GDK_VISIBILITY_NOTIFY:
      case GDK_CLIENT_EVENT:
      case GDK_CONFIGURE:
      case GDK_FOCUS_CHANGE:
      case GDK_SELECTION_CLEAR:
      case GDK_SELECTION_REQUEST:
      case GDK_SELECTION_NOTIFY:
      case GDK_PROXIMITY_IN:
      case GDK_PROXIMITY_OUT:
      case GDK_DAMAGE:
      case GDK_DRAG_ENTER:
      case GDK_DRAG_LEAVE:
      case GDK_DRAG_MOTION:
      case GDK_DRAG_STATUS:
      case GDK_DROP_START:
      case GDK_DROP_FINISHED:
      case GDK_NOTHING:
      case GDK_DELETE:
      case GDK_DESTROY:
      case GDK_EXPOSE:
      case GDK_MAP:
      case GDK_UNMAP:
      case GDK_WINDOW_STATE:
      case GDK_SETTING:
      case GDK_OWNER_CHANGE:
      case GDK_GRAB_BROKEN:
      case GDK_EVENT_LAST:
        /* no state field */
        break;
      }

  *state = 0;
  return FALSE;
}

/**
 * gdk_event_get_coords:
 * @event: a #GdkEvent
 * @x_win: (out): location to put event window x coordinate
 * @y_win: (out): location to put event window y coordinate
 * 
 * Extract the event window relative x/y coordinates from an event.
 * 
 * Return value: %TRUE if the event delivered event window coordinates
 **/
gboolean
gdk_event_get_coords (const GdkEvent *event,
		      gdouble        *x_win,
		      gdouble        *y_win)
{
  gdouble x = 0, y = 0;
  gboolean fetched = TRUE;
  
  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type)
    {
    case GDK_CONFIGURE:
      x = event->configure.x;
      y = event->configure.y;
      break;
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      x = event->crossing.x;
      y = event->crossing.y;
      break;
    case GDK_SCROLL:
      x = event->scroll.x;
      y = event->scroll.y;
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      x = event->button.x;
      y = event->button.y;
      break;
    case GDK_MOTION_NOTIFY:
      x = event->motion.x;
      y = event->motion.y;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (x_win)
    *x_win = x;
  if (y_win)
    *y_win = y;

  return fetched;
}

/**
 * gdk_event_get_root_coords:
 * @event: a #GdkEvent
 * @x_root: (out): location to put root window x coordinate
 * @y_root: (out): location to put root window y coordinate
 * 
 * Extract the root window relative x/y coordinates from an event.
 * 
 * Return value: %TRUE if the event delivered root window coordinates
 **/
gboolean
gdk_event_get_root_coords (const GdkEvent *event,
			   gdouble        *x_root,
			   gdouble        *y_root)
{
  gdouble x = 0, y = 0;
  gboolean fetched = TRUE;
  
  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
      x = event->motion.x_root;
      y = event->motion.y_root;
      break;
    case GDK_SCROLL:
      x = event->scroll.x_root;
      y = event->scroll.y_root;
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      x = event->button.x_root;
      y = event->button.y_root;
      break;
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      x = event->crossing.x_root;
      y = event->crossing.y_root;
      break;
    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DRAG_STATUS:
    case GDK_DROP_START:
    case GDK_DROP_FINISHED:
      x = event->dnd.x_root;
      y = event->dnd.y_root;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (x_root)
    *x_root = x;
  if (y_root)
    *y_root = y;

  return fetched;
}

/**
 * gdk_event_get_button:
 * @event: a #GdkEvent
 * @button: (out): location to store mouse button number
 *
 * Extract the button number from an event.
 *
 * Return value: %TRUE if the event delivered a button number
 *
 * Since: 3.2
 **/
gboolean
gdk_event_get_button (const GdkEvent *event,
                      guint *button)
{
  gboolean fetched = TRUE;
  guint number = 0;

  g_return_val_if_fail (event != NULL, FALSE);
  
  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      number = event->button.button;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (button)
    *button = number;

  return fetched;
}

/**
 * gdk_event_get_click_count:
 * @event: a #GdkEvent
 * @click_count: (out): location to store click count
 *
 * Extracts the click count from an event.
 *
 * Return value: %TRUE if the event delivered a click count
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_click_count (const GdkEvent *event,
                           guint *click_count)
{
  gboolean fetched = TRUE;
  guint number = 0;

  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      number = 1;
      break;
    case GDK_2BUTTON_PRESS:
      number = 2;
      break;
    case GDK_3BUTTON_PRESS:
      number = 3;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (click_count)
    *click_count = number;

  return fetched;
}

/**
 * gdk_event_get_keyval:
 * @event: a #GdkEvent
 * @keyval: (out): location to store the keyval
 *
 * Extracts the keyval from an event.
 *
 * Return value: %TRUE if the event delivered a key symbol
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_keyval (const GdkEvent *event,
                      guint *keyval)
{
  gboolean fetched = TRUE;
  guint number = 0;

  switch (event->type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      number = event->key.keyval;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (keyval)
    *keyval = number;

  return fetched;
}

/**
 * gdk_event_get_keycode:
 * @event: a #GdkEvent
 * @keycode: (out): location to store the keycode
 *
 * Extracts the hardware keycode from an event.
 *
 * Return value: %TRUE if the event delivered a hardware keycode
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_keycode (const GdkEvent *event,
                       guint16 *keycode)
{
  gboolean fetched = TRUE;
  guint16 number = 0;

  switch (event->type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      number = event->key.hardware_keycode;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (keycode)
    *keycode = number;

  return fetched;
}

/**
 * gdk_event_get_scroll_direction:
 * @event: a #GdkEvent
 * @direction: (out): location to store the scroll direction
 *
 * Extracts the scroll direction from an event.
 *
 * Return value: %TRUE if the event delivered a scroll direction
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_scroll_direction (const GdkEvent *event,
                                GdkScrollDirection *direction)
{
  gboolean fetched = TRUE;
  GdkScrollDirection dir = 0;

  switch (event->type)
    {
    case GDK_SCROLL:
      dir = event->scroll.direction;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (direction)
    *direction = dir;

  return fetched;
}

/**
 * gdk_event_get_axis:
 * @event: a #GdkEvent
 * @axis_use: the axis use to look for
 * @value: (out): location to store the value found
 * 
 * Extract the axis value for a particular axis use from
 * an event structure.
 * 
 * Return value: %TRUE if the specified axis was found, otherwise %FALSE
 **/
gboolean
gdk_event_get_axis (const GdkEvent *event,
		    GdkAxisUse      axis_use,
		    gdouble        *value)
{
  gdouble *axes;
  GdkDevice *device;
  
  g_return_val_if_fail (event != NULL, FALSE);
  
  if (axis_use == GDK_AXIS_X || axis_use == GDK_AXIS_Y)
    {
      gdouble x, y;
      
      switch (event->type)
	{
	case GDK_MOTION_NOTIFY:
	  x = event->motion.x;
	  y = event->motion.y;
	  break;
	case GDK_SCROLL:
	  x = event->scroll.x;
	  y = event->scroll.y;
	  break;
	case GDK_BUTTON_PRESS:
	case GDK_BUTTON_RELEASE:
	  x = event->button.x;
	  y = event->button.y;
	  break;
	case GDK_ENTER_NOTIFY:
	case GDK_LEAVE_NOTIFY:
	  x = event->crossing.x;
	  y = event->crossing.y;
	  break;
	  
	default:
	  return FALSE;
	}

      if (axis_use == GDK_AXIS_X && value)
	*value = x;
      if (axis_use == GDK_AXIS_Y && value)
	*value = y;

      return TRUE;
    }
  else if (event->type == GDK_BUTTON_PRESS ||
	   event->type == GDK_BUTTON_RELEASE)
    {
      device = event->button.device;
      axes = event->button.axes;
    }
  else if (event->type == GDK_MOTION_NOTIFY)
    {
      device = event->motion.device;
      axes = event->motion.axes;
    }
  else
    return FALSE;

  return gdk_device_get_axis (device, axes, axis_use, value);
}

/**
 * gdk_event_set_device:
 * @event: a #GdkEvent
 * @device: a #GdkDevice
 *
 * Sets the device for @event to @device. The event must
 * have been allocated by GTK+, for instance, by
 * gdk_event_copy().
 *
 * Since: 3.0
 **/
void
gdk_event_set_device (GdkEvent  *event,
                      GdkDevice *device)
{
  GdkEventPrivate *private;

  g_return_if_fail (gdk_event_is_allocated (event));

  private = (GdkEventPrivate *) event;

  private->device = device;

  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
      event->motion.device = device;
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      event->button.device = device;
      break;
    case GDK_SCROLL:
      event->scroll.device = device;
      break;
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      event->proximity.device = device;
      break;
    default:
      break;
    }
}

/**
 * gdk_event_get_device:
 * @event: a #GdkEvent.
 *
 * If the event contains a "device" field, this function will return
 * it, else it will return %NULL.
 *
 * Returns: (transfer none): a #GdkDevice, or %NULL.
 *
 * Since: 3.0
 **/
GdkDevice *
gdk_event_get_device (const GdkEvent *event)
{
  g_return_val_if_fail (event != NULL, NULL);

  if (gdk_event_is_allocated (event))
    {
      GdkEventPrivate *private = (GdkEventPrivate *) event;

      if (private->device)
        return private->device;
    }

  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
      return event->motion.device;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      return event->button.device;
    case GDK_SCROLL:
      return event->scroll.device;
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      return event->proximity.device;
    default:
      break;
    }

  /* Fallback if event has no device set */
  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
    case GDK_FOCUS_CHANGE:
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DRAG_STATUS:
    case GDK_DROP_START:
    case GDK_DROP_FINISHED:
    case GDK_SCROLL:
    case GDK_GRAB_BROKEN:
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      {
        GdkDisplay *display;
        GdkDeviceManager *device_manager;
        GdkDevice *client_pointer;

        g_warning ("Event with type %d not holding a GdkDevice. "
                   "It is most likely synthesized outside Gdk/GTK+\n",
                   event->type);

        display = gdk_window_get_display (event->any.window);
        device_manager = gdk_display_get_device_manager (display);
        client_pointer = gdk_device_manager_get_client_pointer (device_manager);

        if (event->type == GDK_KEY_PRESS ||
            event->type == GDK_KEY_RELEASE)
          return gdk_device_get_associated_device (client_pointer);
        else
          return client_pointer;
      }
      break;
    default:
      return NULL;
    }
}

/**
 * gdk_event_set_source_device:
 * @event: a #GdkEvent
 * @device: a #GdkDevice
 *
 * Sets the slave device for @event to @device.
 *
 * The event must have been allocated by GTK+,
 * for instance by gdk_event_copy().
 *
 * Since: 3.0
 **/
void
gdk_event_set_source_device (GdkEvent  *event,
                             GdkDevice *device)
{
  GdkEventPrivate *private;

  g_return_if_fail (gdk_event_is_allocated (event));
  g_return_if_fail (GDK_IS_DEVICE (device));

  private = (GdkEventPrivate *) event;

  private->source_device = device;
}

/**
 * gdk_event_get_source_device:
 * @event: a #GdkEvent
 *
 * This function returns the hardware (slave) #GdkDevice that has
 * triggered the event, falling back to the virtual (master) device
 * (as in gdk_event_get_device()) if the event wasn't caused by
 * interaction with a hardware device. This may happen for example
 * in synthesized crossing events after a #GdkWindow updates its
 * geometry or a grab is acquired/released.
 *
 * If the event does not contain a device field, this function will
 * return %NULL.
 *
 * Returns: (transfer none): a #GdkDevice, or %NULL.
 *
 * Since: 3.0
 **/
GdkDevice *
gdk_event_get_source_device (const GdkEvent *event)
{
  GdkEventPrivate *private;

  g_return_val_if_fail (event != NULL, NULL);

  if (!gdk_event_is_allocated (event))
    return NULL;

  private = (GdkEventPrivate *) event;

  if (private->source_device)
    return private->source_device;

  /* Fallback to event device */
  return gdk_event_get_device (event);
}

/**
 * gdk_event_request_motions:
 * @event: a valid #GdkEvent
 *
 * Request more motion notifies if @event is a motion notify hint event.
 *
 * This function should be used instead of gdk_window_get_pointer() to
 * request further motion notifies, because it also works for extension
 * events where motion notifies are provided for devices other than the
 * core pointer. Coordinate extraction, processing and requesting more
 * motion events from a %GDK_MOTION_NOTIFY event usually works like this:
 *
 * |[
 * {
 *   /&ast; motion_event handler &ast;/
 *   x = motion_event->x;
 *   y = motion_event->y;
 *   /&ast; handle (x,y) motion &ast;/
 *   gdk_event_request_motions (motion_event); /&ast; handles is_hint events &ast;/
 * }
 * ]|
 *
 * Since: 2.12
 **/
void
gdk_event_request_motions (const GdkEventMotion *event)
{
  GdkDisplay *display;
  
  g_return_if_fail (event != NULL);
  
  if (event->type == GDK_MOTION_NOTIFY && event->is_hint)
    {
      gdk_device_get_state (event->device, event->window, NULL, NULL);
      
      display = gdk_window_get_display (event->window);
      _gdk_display_enable_motion_hints (display, event->device);
    }
}

static gboolean
gdk_events_get_axis_distances (GdkEvent *event1,
                               GdkEvent *event2,
                               gdouble  *x_distance,
                               gdouble  *y_distance,
                               gdouble  *distance)
{
  gdouble x1, x2, y1, y2;
  gdouble xd, yd;

  if (!gdk_event_get_coords (event1, &x1, &y1) ||
      !gdk_event_get_coords (event2, &x2, &y2))
    return FALSE;

  xd = x2 - x1;
  yd = y2 - y1;

  if (x_distance)
    *x_distance = xd;

  if (y_distance)
    *y_distance = yd;

  if (distance)
    *distance = sqrt ((xd * xd) + (yd * yd));

  return TRUE;
}

/**
 * gdk_events_get_distance:
 * @event1: first #GdkEvent
 * @event2: second #GdkEvent
 * @distance: (out): return location for the distance
 *
 * If both events have X/Y information, the distance between both coordinates
 * (as in a straight line going from @event1 to @event2) will be returned.
 *
 * Returns: %TRUE if the distance could be calculated.
 *
 * Since: 3.0
 **/
gboolean
gdk_events_get_distance (GdkEvent *event1,
                         GdkEvent *event2,
                         gdouble  *distance)
{
  return gdk_events_get_axis_distances (event1, event2,
                                        NULL, NULL,
                                        distance);
}

/**
 * gdk_events_get_angle:
 * @event1: first #GdkEvent
 * @event2: second #GdkEvent
 * @angle: (out): return location for the relative angle between both events
 *
 * If both events contain X/Y information, this function will return %TRUE
 * and return in @angle the relative angle from @event1 to @event2. The rotation
 * direction for positive angles is from the positive X axis towards the positive
 * Y axis.
 *
 * Returns: %TRUE if the angle could be calculated.
 *
 * Since: 3.0
 **/
gboolean
gdk_events_get_angle (GdkEvent *event1,
                      GdkEvent *event2,
                      gdouble  *angle)
{
  gdouble x_distance, y_distance, distance;

  if (!gdk_events_get_axis_distances (event1, event2,
                                      &x_distance, &y_distance,
                                      &distance))
    return FALSE;

  if (angle)
    {
      *angle = atan2 (x_distance, y_distance);

      /* Invert angle */
      *angle = (2 * G_PI) - *angle;

      /* Shift it 90° */
      *angle += G_PI / 2;

      /* And constraint it to 0°-360° */
      *angle = fmod (*angle, 2 * G_PI);
    }

  return TRUE;
}

/**
 * gdk_events_get_center:
 * @event1: first #GdkEvent
 * @event2: second #GdkEvent
 * @x: (out): return location for the X coordinate of the center
 * @y: (out): return location for the Y coordinate of the center
 *
 * If both events contain X/Y information, the center of both coordinates
 * will be returned in @x and @y.
 *
 * Returns: %TRUE if the center could be calculated.
 *
 * Since: 3.0
 **/
gboolean
gdk_events_get_center (GdkEvent *event1,
                       GdkEvent *event2,
                       gdouble  *x,
                       gdouble  *y)
{
  gdouble x1, x2, y1, y2;

  if (!gdk_event_get_coords (event1, &x1, &y1) ||
      !gdk_event_get_coords (event2, &x2, &y2))
    return FALSE;

  if (x)
    *x = (x2 + x1) / 2;

  if (y)
    *y = (y2 + y1) / 2;

  return TRUE;
}

/**
 * gdk_event_set_screen:
 * @event: a #GdkEvent
 * @screen: a #GdkScreen
 * 
 * Sets the screen for @event to @screen. The event must
 * have been allocated by GTK+, for instance, by
 * gdk_event_copy().
 *
 * Since: 2.2
 **/
void
gdk_event_set_screen (GdkEvent  *event,
		      GdkScreen *screen)
{
  GdkEventPrivate *private;
  
  g_return_if_fail (gdk_event_is_allocated (event));

  private = (GdkEventPrivate *)event;
  
  private->screen = screen;
}

/**
 * gdk_event_get_screen:
 * @event: a #GdkEvent
 * 
 * Returns the screen for the event. The screen is
 * typically the screen for <literal>event->any.window</literal>, but
 * for events such as mouse events, it is the screen
 * where the pointer was when the event occurs -
 * that is, the screen which has the root window 
 * to which <literal>event->motion.x_root</literal> and
 * <literal>event->motion.y_root</literal> are relative.
 * 
 * Return value: (transfer none): the screen for the event
 *
 * Since: 2.2
 **/
GdkScreen *
gdk_event_get_screen (const GdkEvent *event)
{
  if (gdk_event_is_allocated (event))
    {
      GdkEventPrivate *private = (GdkEventPrivate *)event;

      if (private->screen)
	return private->screen;
    }

  if (event->any.window)
    return gdk_window_get_screen (event->any.window);

  return NULL;
}

/**
 * gdk_set_show_events:
 * @show_events:  %TRUE to output event debugging information.
 * 
 * Sets whether a trace of received events is output.
 * Note that GTK+ must be compiled with debugging (that is,
 * configured using the <option>--enable-debug</option> option)
 * to use this option.
 **/
void
gdk_set_show_events (gboolean show_events)
{
  if (show_events)
    _gdk_debug_flags |= GDK_DEBUG_EVENTS;
  else
    _gdk_debug_flags &= ~GDK_DEBUG_EVENTS;
}

/**
 * gdk_get_show_events:
 * 
 * Gets whether event debugging output is enabled.
 * 
 * Return value: %TRUE if event debugging output is enabled.
 **/
gboolean
gdk_get_show_events (void)
{
  return (_gdk_debug_flags & GDK_DEBUG_EVENTS) != 0;
}

/* What do we do with G_IO_NVAL?
 */
#define READ_CONDITION (G_IO_IN | G_IO_HUP | G_IO_ERR)
#define WRITE_CONDITION (G_IO_OUT | G_IO_ERR)
#define EXCEPTION_CONDITION (G_IO_PRI)

static void
gdk_synthesize_click (GdkDisplay *display,
                      GdkEvent   *event,
                      gint        nclicks)
{
  GdkEvent *event_copy;

  event_copy = gdk_event_copy (event);
  event_copy->type = (nclicks == 2) ? GDK_2BUTTON_PRESS : GDK_3BUTTON_PRESS;

  _gdk_event_queue_append (display, event_copy);
}

void
_gdk_event_button_generate (GdkDisplay *display,
			    GdkEvent   *event)
{
  GdkMultipleClickInfo *info;

  g_return_if_fail (event->type == GDK_BUTTON_PRESS);

  info = g_hash_table_lookup (display->multiple_click_info, event->button.device);

  if (G_UNLIKELY (!info))
    {
      info = g_new0 (GdkMultipleClickInfo, 1);
      info->button_number[0] = info->button_number[1] = -1;

      g_hash_table_insert (display->multiple_click_info,
                           event->button.device, info);
    }

  if ((event->button.time < (info->button_click_time[1] + 2 * display->double_click_time)) &&
      (event->button.window == info->button_window[1]) &&
      (event->button.button == info->button_number[1]) &&
      (ABS (event->button.x - info->button_x[1]) <= display->double_click_distance) &&
      (ABS (event->button.y - info->button_y[1]) <= display->double_click_distance))
    {
      gdk_synthesize_click (display, event, 3);

      info->button_click_time[1] = 0;
      info->button_click_time[0] = 0;
      info->button_window[1] = NULL;
      info->button_window[0] = NULL;
      info->button_number[1] = -1;
      info->button_number[0] = -1;
      info->button_x[0] = info->button_x[1] = 0;
      info->button_y[0] = info->button_y[1] = 0;
    }
  else if ((event->button.time < (info->button_click_time[0] + display->double_click_time)) &&
	   (event->button.window == info->button_window[0]) &&
	   (event->button.button == info->button_number[0]) &&
	   (ABS (event->button.x - info->button_x[0]) <= display->double_click_distance) &&
	   (ABS (event->button.y - info->button_y[0]) <= display->double_click_distance))
    {
      gdk_synthesize_click (display, event, 2);
      
      info->button_click_time[1] = info->button_click_time[0];
      info->button_click_time[0] = event->button.time;
      info->button_window[1] = info->button_window[0];
      info->button_window[0] = event->button.window;
      info->button_number[1] = info->button_number[0];
      info->button_number[0] = event->button.button;
      info->button_x[1] = info->button_x[0];
      info->button_x[0] = event->button.x;
      info->button_y[1] = info->button_y[0];
      info->button_y[0] = event->button.y;
    }
  else
    {
      info->button_click_time[1] = 0;
      info->button_click_time[0] = event->button.time;
      info->button_window[1] = NULL;
      info->button_window[0] = event->button.window;
      info->button_number[1] = -1;
      info->button_number[0] = event->button.button;
      info->button_x[1] = 0;
      info->button_x[0] = event->button.x;
      info->button_y[1] = 0;
      info->button_y[0] = event->button.y;
    }
}

void
gdk_synthesize_window_state (GdkWindow     *window,
                             GdkWindowState unset_flags,
                             GdkWindowState set_flags)
{
  GdkEvent temp_event;
  GdkWindowState old;
  
  g_return_if_fail (window != NULL);
  
  temp_event.window_state.window = window;
  temp_event.window_state.type = GDK_WINDOW_STATE;
  temp_event.window_state.send_event = FALSE;
  
  old = temp_event.window_state.window->state;
  
  temp_event.window_state.new_window_state = old;
  temp_event.window_state.new_window_state |= set_flags;
  temp_event.window_state.new_window_state &= ~unset_flags;
  temp_event.window_state.changed_mask = temp_event.window_state.new_window_state ^ old;

  if (temp_event.window_state.new_window_state == old)
    return; /* No actual work to do, nothing changed. */

  /* Actually update the field in GdkWindow, this is sort of an odd
   * place to do it, but seems like the safest since it ensures we expose no
   * inconsistent state to the user.
   */
  
  window->state = temp_event.window_state.new_window_state;

  if (temp_event.window_state.changed_mask & GDK_WINDOW_STATE_WITHDRAWN)
    _gdk_window_update_viewable (window);

  /* We only really send the event to toplevels, since
   * all the window states don't apply to non-toplevels.
   * Non-toplevels do use the GDK_WINDOW_STATE_WITHDRAWN flag
   * internally so we needed to update window->state.
   */
  switch (window->window_type)
    {
    case GDK_WINDOW_TOPLEVEL:
    case GDK_WINDOW_TEMP: /* ? */
      gdk_display_put_event (gdk_window_get_display (window), &temp_event);
      break;
      
    case GDK_WINDOW_FOREIGN:
    case GDK_WINDOW_ROOT:
    case GDK_WINDOW_CHILD:
      break;
    }
}

/**
 * gdk_display_set_double_click_time:
 * @display: a #GdkDisplay
 * @msec: double click time in milliseconds (thousandths of a second) 
 * 
 * Sets the double click time (two clicks within this time interval
 * count as a double click and result in a #GDK_2BUTTON_PRESS event).
 * Applications should <emphasis>not</emphasis> set this, it is a global 
 * user-configured setting.
 *
 * Since: 2.2
 **/
void
gdk_display_set_double_click_time (GdkDisplay *display,
				   guint       msec)
{
  display->double_click_time = msec;
}

/**
 * gdk_set_double_click_time:
 * @msec: double click time in milliseconds (thousandths of a second)
 *
 * Set the double click time for the default display. See
 * gdk_display_set_double_click_time(). 
 * See also gdk_display_set_double_click_distance().
 * Applications should <emphasis>not</emphasis> set this, it is a 
 * global user-configured setting.
 **/
void
gdk_set_double_click_time (guint msec)
{
  gdk_display_set_double_click_time (gdk_display_get_default (), msec);
}

/**
 * gdk_display_set_double_click_distance:
 * @display: a #GdkDisplay
 * @distance: distance in pixels
 * 
 * Sets the double click distance (two clicks within this distance
 * count as a double click and result in a #GDK_2BUTTON_PRESS event).
 * See also gdk_display_set_double_click_time().
 * Applications should <emphasis>not</emphasis> set this, it is a global 
 * user-configured setting.
 *
 * Since: 2.4
 **/
void
gdk_display_set_double_click_distance (GdkDisplay *display,
				       guint       distance)
{
  display->double_click_distance = distance;
}

G_DEFINE_BOXED_TYPE (GdkEvent, gdk_event,
                     gdk_event_copy,
                     gdk_event_free)

/**
 * gdk_setting_get:
 * @name: the name of the setting.
 * @value: location to store the value of the setting.
 *
 * Obtains a desktop-wide setting, such as the double-click time,
 * for the default screen. See gdk_screen_get_setting().
 *
 * Returns: %TRUE if the setting existed and a value was stored
 *   in @value, %FALSE otherwise.
 **/
gboolean
gdk_setting_get (const gchar *name,
		 GValue      *value)
{
  return gdk_screen_get_setting (gdk_screen_get_default (), name, value);
}
