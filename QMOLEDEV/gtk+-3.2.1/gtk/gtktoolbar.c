/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * GtkToolbar copyright (C) Federico Mena
 *
 * Copyright (C) 2002 Anders Carlsson <andersca@gnome.org>
 * Copyright (C) 2002 James Henstridge <james@daa.com.au>
 * Copyright (C) 2003, 2004 Soeren Sandmann <sandmann@daimi.au.dk>
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */


#include "config.h"

#include <math.h>
#include <string.h>

#include "gtktoolbar.h"

#include "gtkarrow.h"
#include "gtkbindings.h"
#include "gtkimage.h"
#include "gtklabel.h"
#include "gtkmainprivate.h"
#include "gtkmarshalers.h"
#include "gtkmenu.h"
#include "gtkorientable.h"
#include "gtkradiobutton.h"
#include "gtkradiotoolbutton.h"
#include "gtkseparatormenuitem.h"
#include "gtkseparatortoolitem.h"
#include "gtkstock.h"
#include "gtktoolshell.h"
#include "gtkbox.h"
#include "gtkprivate.h"
#include "gtkintl.h"
#include "gtktypebuiltins.h"


/**
 * SECTION:gtktoolbar
 * @Short_description: Create bars of buttons and other widgets
 * @Title: GtkToolbar
 * @See_also: #GtkToolItem
 *
 * A toolbar is created with a call to gtk_toolbar_new().
 *
 * A toolbar can contain instances of a subclass of #GtkToolItem. To add
 * a #GtkToolItem to the a toolbar, use gtk_toolbar_insert(). To remove
 * an item from the toolbar use gtk_container_remove(). To add a button
 * to the toolbar, add an instance of #GtkToolButton.
 *
 * Toolbar items can be visually grouped by adding instances of
 * #GtkSeparatorToolItem to the toolbar. If the GtkToolbar child property
 * "expand" is #TRUE and the property #GtkSeparatorToolItem:draw is set to
 * #FALSE, the effect is to force all following items to the end of the toolbar.
 *
 * Creating a context menu for the toolbar can be done by connecting to
 * the #GtkToolbar::popup-context-menu signal.
 */


typedef struct _ToolbarContent ToolbarContent;

#define DEFAULT_IPADDING    0

#define DEFAULT_SPACE_SIZE  12
#define DEFAULT_SPACE_STYLE GTK_TOOLBAR_SPACE_LINE
#define SPACE_LINE_DIVISION 10.0
#define SPACE_LINE_START    2.0
#define SPACE_LINE_END      8.0

#define DEFAULT_ICON_SIZE GTK_ICON_SIZE_LARGE_TOOLBAR
#define DEFAULT_TOOLBAR_STYLE GTK_TOOLBAR_BOTH
#define DEFAULT_ANIMATION_STATE TRUE

#define MAX_HOMOGENEOUS_N_CHARS 13 /* Items that are wider than this do not participate
				    * in the homogeneous game. In units of
				    * pango_font_get_estimated_char_width().
				    */
#define SLIDE_SPEED 600.0	   /* How fast the items slide, in pixels per second */
#define ACCEL_THRESHOLD 0.18	   /* After how much time in seconds will items start speeding up */


struct _GtkToolbarPrivate
{
  GtkMenu         *menu;
  GtkSettings     *settings;

  GtkIconSize      icon_size;
  GtkToolbarStyle  style;

  GtkToolItem     *highlight_tool_item;
  GtkWidget       *arrow;
  GtkWidget       *arrow_button;

  GdkWindow       *event_window;

  GList           *content;

  GTimer          *timer;

  GtkWidgetPath   *sibling_path;

  gulong           settings_connection;

  gint             idle_id;
  gint             button_maxw;         /* maximum width of homogeneous children */
  gint             button_maxh;         /* maximum height of homogeneous children */
  gint             max_homogeneous_pixels;
  gint             num_children;

  GtkOrientation   orientation;

  guint            animation : 1;
  guint            icon_size_set : 1;
  guint            is_sliding : 1;
  guint            need_rebuild : 1;  /* whether the overflow menu should be regenerated */
  guint            need_sync : 1;
  guint            show_arrow : 1;
  guint            style_set     : 1;
};

/* Properties */
enum {
  PROP_0,
  PROP_ORIENTATION,
  PROP_TOOLBAR_STYLE,
  PROP_SHOW_ARROW,
  PROP_TOOLTIPS,
  PROP_ICON_SIZE,
  PROP_ICON_SIZE_SET
};

/* Child properties */
enum {
  CHILD_PROP_0,
  CHILD_PROP_EXPAND,
  CHILD_PROP_HOMOGENEOUS
};

/* Signals */
enum {
  ORIENTATION_CHANGED,
  STYLE_CHANGED,
  POPUP_CONTEXT_MENU,
  FOCUS_HOME_OR_END,
  LAST_SIGNAL
};

typedef enum {
  NOT_ALLOCATED,
  NORMAL,
  HIDDEN,
  OVERFLOWN
} ItemState;


static void       gtk_toolbar_set_property         (GObject             *object,
						    guint                prop_id,
						    const GValue        *value,
						    GParamSpec          *pspec);
static void       gtk_toolbar_get_property         (GObject             *object,
						    guint                prop_id,
						    GValue              *value,
						    GParamSpec          *pspec);
static gint       gtk_toolbar_draw                 (GtkWidget           *widget,
                                                    cairo_t             *cr);
static void       gtk_toolbar_realize              (GtkWidget           *widget);
static void       gtk_toolbar_unrealize            (GtkWidget           *widget);
static void       gtk_toolbar_get_preferred_width  (GtkWidget           *widget,
                                                    gint                *minimum,
                                                    gint                *natural);
static void       gtk_toolbar_get_preferred_height (GtkWidget           *widget,
                                                    gint                *minimum,
                                                    gint                *natural);

static void       gtk_toolbar_size_allocate        (GtkWidget           *widget,
						    GtkAllocation       *allocation);
static void       gtk_toolbar_style_updated        (GtkWidget           *widget);
static gboolean   gtk_toolbar_focus                (GtkWidget           *widget,
						    GtkDirectionType     dir);
static void       gtk_toolbar_move_focus           (GtkWidget           *widget,
						    GtkDirectionType     dir);
static void       gtk_toolbar_screen_changed       (GtkWidget           *widget,
						    GdkScreen           *previous_screen);
static void       gtk_toolbar_map                  (GtkWidget           *widget);
static void       gtk_toolbar_unmap                (GtkWidget           *widget);
static void       gtk_toolbar_set_child_property   (GtkContainer        *container,
						    GtkWidget           *child,
						    guint                property_id,
						    const GValue        *value,
						    GParamSpec          *pspec);
static void       gtk_toolbar_get_child_property   (GtkContainer        *container,
						    GtkWidget           *child,
						    guint                property_id,
						    GValue              *value,
						    GParamSpec          *pspec);
static void       gtk_toolbar_finalize             (GObject             *object);
static void       gtk_toolbar_dispose              (GObject             *object);
static void       gtk_toolbar_show_all             (GtkWidget           *widget);
static void       gtk_toolbar_add                  (GtkContainer        *container,
						    GtkWidget           *widget);
static void       gtk_toolbar_remove               (GtkContainer        *container,
						    GtkWidget           *widget);
static void       gtk_toolbar_forall               (GtkContainer        *container,
						    gboolean             include_internals,
						    GtkCallback          callback,
						    gpointer             callback_data);
static GType      gtk_toolbar_child_type           (GtkContainer        *container);
static GtkWidgetPath * gtk_toolbar_get_path_for_child
                                                  (GtkContainer        *container,
                                                   GtkWidget           *child);
static void       gtk_toolbar_invalidate_order    (GtkToolbar           *toolbar);

static void       gtk_toolbar_orientation_changed  (GtkToolbar          *toolbar,
						    GtkOrientation       orientation);
static void       gtk_toolbar_real_style_changed   (GtkToolbar          *toolbar,
						    GtkToolbarStyle      style);
static gboolean   gtk_toolbar_focus_home_or_end    (GtkToolbar          *toolbar,
						    gboolean             focus_home);
static gboolean   gtk_toolbar_button_press         (GtkWidget           *toolbar,
						    GdkEventButton      *event);
static gboolean   gtk_toolbar_arrow_button_press   (GtkWidget           *button,
						    GdkEventButton      *event,
						    GtkToolbar          *toolbar);
static void       gtk_toolbar_arrow_button_clicked (GtkWidget           *button,
						    GtkToolbar          *toolbar);
static void       gtk_toolbar_update_button_relief (GtkToolbar          *toolbar);
static gboolean   gtk_toolbar_popup_menu           (GtkWidget           *toolbar);
static void       gtk_toolbar_reconfigured         (GtkToolbar          *toolbar);

static GtkReliefStyle       get_button_relief    (GtkToolbar *toolbar);
static gint                 get_internal_padding (GtkToolbar *toolbar);
static gint                 get_max_child_expand (GtkToolbar *toolbar);
static GtkShadowType        get_shadow_type      (GtkToolbar *toolbar);

/* methods on ToolbarContent 'class' */
static ToolbarContent *toolbar_content_new_tool_item        (GtkToolbar          *toolbar,
							     GtkToolItem         *item,
							     gboolean             is_placeholder,
							     gint                 pos);
static void            toolbar_content_remove               (ToolbarContent      *content,
							     GtkToolbar          *toolbar);
static void            toolbar_content_free                 (ToolbarContent      *content);
static void            toolbar_content_draw                 (ToolbarContent      *content,
							     GtkContainer        *container,
                                                             cairo_t             *cr);
static gboolean        toolbar_content_visible              (ToolbarContent      *content,
							     GtkToolbar          *toolbar);
static void            toolbar_content_size_request         (ToolbarContent      *content,
							     GtkToolbar          *toolbar,
							     GtkRequisition      *requisition);
static gboolean        toolbar_content_is_homogeneous       (ToolbarContent      *content,
							     GtkToolbar          *toolbar);
static gboolean        toolbar_content_is_placeholder       (ToolbarContent      *content);
static gboolean        toolbar_content_disappearing         (ToolbarContent      *content);
static ItemState       toolbar_content_get_state            (ToolbarContent      *content);
static gboolean        toolbar_content_child_visible        (ToolbarContent      *content);
static void            toolbar_content_get_goal_allocation  (ToolbarContent      *content,
							     GtkAllocation       *allocation);
static void            toolbar_content_get_allocation       (ToolbarContent      *content,
							     GtkAllocation       *allocation);
static void            toolbar_content_set_start_allocation (ToolbarContent      *content,
							     GtkAllocation       *new_start_allocation);
static void            toolbar_content_get_start_allocation (ToolbarContent      *content,
							     GtkAllocation       *start_allocation);
static gboolean        toolbar_content_get_expand           (ToolbarContent      *content);
static void            toolbar_content_set_goal_allocation  (ToolbarContent      *content,
							     GtkAllocation       *allocation);
static void            toolbar_content_set_child_visible    (ToolbarContent      *content,
							     GtkToolbar          *toolbar,
							     gboolean             visible);
static void            toolbar_content_size_allocate        (ToolbarContent      *content,
							     GtkAllocation       *allocation);
static void            toolbar_content_set_state            (ToolbarContent      *content,
							     ItemState            new_state);
static GtkWidget *     toolbar_content_get_widget           (ToolbarContent      *content);
static void            toolbar_content_set_disappearing     (ToolbarContent      *content,
							     gboolean             disappearing);
static void            toolbar_content_set_size_request     (ToolbarContent      *content,
							     gint                 width,
							     gint                 height);
static void            toolbar_content_toolbar_reconfigured (ToolbarContent      *content,
							     GtkToolbar          *toolbar);
static GtkWidget *     toolbar_content_retrieve_menu_item   (ToolbarContent      *content);
static gboolean        toolbar_content_has_proxy_menu_item  (ToolbarContent	 *content);
static gboolean        toolbar_content_is_separator         (ToolbarContent      *content);
static void            toolbar_content_show_all             (ToolbarContent      *content);
static void	       toolbar_content_set_expand	    (ToolbarContent      *content,
							     gboolean		  expand);

static void            toolbar_tool_shell_iface_init        (GtkToolShellIface   *iface);
static GtkIconSize     toolbar_get_icon_size                (GtkToolShell        *shell);
static GtkOrientation  toolbar_get_orientation              (GtkToolShell        *shell);
static GtkToolbarStyle toolbar_get_style                    (GtkToolShell        *shell);
static GtkReliefStyle  toolbar_get_relief_style             (GtkToolShell        *shell);
static void            toolbar_rebuild_menu                 (GtkToolShell        *shell);


G_DEFINE_TYPE_WITH_CODE (GtkToolbar, gtk_toolbar, GTK_TYPE_CONTAINER,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_TOOL_SHELL,
                                                toolbar_tool_shell_iface_init)
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_ORIENTABLE,
                                                NULL))

static guint toolbar_signals[LAST_SIGNAL] = { 0 };


static void
add_arrow_bindings (GtkBindingSet   *binding_set,
		    guint            keysym,
		    GtkDirectionType dir)
{
  guint keypad_keysym = keysym - GDK_KEY_Left + GDK_KEY_KP_Left;
  
  gtk_binding_entry_add_signal (binding_set, keysym, 0,
                                "move-focus", 1,
                                GTK_TYPE_DIRECTION_TYPE, dir);
  gtk_binding_entry_add_signal (binding_set, keypad_keysym, 0,
                                "move-focus", 1,
                                GTK_TYPE_DIRECTION_TYPE, dir);
}

static void
add_ctrl_tab_bindings (GtkBindingSet    *binding_set,
		       GdkModifierType   modifiers,
		       GtkDirectionType  direction)
{
  gtk_binding_entry_add_signal (binding_set,
				GDK_KEY_Tab, GDK_CONTROL_MASK | modifiers,
				"move-focus", 1,
				GTK_TYPE_DIRECTION_TYPE, direction);
  gtk_binding_entry_add_signal (binding_set,
				GDK_KEY_KP_Tab, GDK_CONTROL_MASK | modifiers,
				"move-focus", 1,
				GTK_TYPE_DIRECTION_TYPE, direction);
}

static void
gtk_toolbar_class_init (GtkToolbarClass *klass)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;
  GtkBindingSet *binding_set;
  
  gobject_class = (GObjectClass *)klass;
  widget_class = (GtkWidgetClass *)klass;
  container_class = (GtkContainerClass *)klass;
  
  gobject_class->set_property = gtk_toolbar_set_property;
  gobject_class->get_property = gtk_toolbar_get_property;
  gobject_class->finalize = gtk_toolbar_finalize;
  gobject_class->dispose = gtk_toolbar_dispose;
  
  widget_class->button_press_event = gtk_toolbar_button_press;
  widget_class->draw = gtk_toolbar_draw;
  widget_class->get_preferred_width = gtk_toolbar_get_preferred_width;
  widget_class->get_preferred_height = gtk_toolbar_get_preferred_height;
  widget_class->size_allocate = gtk_toolbar_size_allocate;
  widget_class->style_updated = gtk_toolbar_style_updated;
  widget_class->focus = gtk_toolbar_focus;

  gtk_widget_class_set_accessible_role (widget_class, ATK_ROLE_TOOL_BAR);

  /* need to override the base class function via override_class_handler,
   * because the signal slot is not available in GtkWidgetClass
   */
  g_signal_override_class_handler ("move-focus",
                                   GTK_TYPE_TOOLBAR,
                                   G_CALLBACK (gtk_toolbar_move_focus));

  widget_class->screen_changed = gtk_toolbar_screen_changed;
  widget_class->realize = gtk_toolbar_realize;
  widget_class->unrealize = gtk_toolbar_unrealize;
  widget_class->map = gtk_toolbar_map;
  widget_class->unmap = gtk_toolbar_unmap;
  widget_class->popup_menu = gtk_toolbar_popup_menu;
  widget_class->show_all = gtk_toolbar_show_all;
  
  container_class->add    = gtk_toolbar_add;
  container_class->remove = gtk_toolbar_remove;
  container_class->forall = gtk_toolbar_forall;
  container_class->child_type = gtk_toolbar_child_type;
  container_class->get_child_property = gtk_toolbar_get_child_property;
  container_class->set_child_property = gtk_toolbar_set_child_property;
  container_class->get_path_for_child = gtk_toolbar_get_path_for_child;

  klass->orientation_changed = gtk_toolbar_orientation_changed;
  klass->style_changed = gtk_toolbar_real_style_changed;
  
  /**
   * GtkToolbar::orientation-changed:
   * @toolbar: the object which emitted the signal
   * @orientation: the new #GtkOrientation of the toolbar
   *
   * Emitted when the orientation of the toolbar changes.
   */
  toolbar_signals[ORIENTATION_CHANGED] =
    g_signal_new (I_("orientation-changed"),
		  G_OBJECT_CLASS_TYPE (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GtkToolbarClass, orientation_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__ENUM,
		  G_TYPE_NONE, 1,
		  GTK_TYPE_ORIENTATION);
  /**
   * GtkToolbar::style-changed:
   * @toolbar: The #GtkToolbar which emitted the signal
   * @style: the new #GtkToolbarStyle of the toolbar
   *
   * Emitted when the style of the toolbar changes. 
   */
  toolbar_signals[STYLE_CHANGED] =
    g_signal_new (I_("style-changed"),
		  G_OBJECT_CLASS_TYPE (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GtkToolbarClass, style_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__ENUM,
		  G_TYPE_NONE, 1,
		  GTK_TYPE_TOOLBAR_STYLE);
  /**
   * GtkToolbar::popup-context-menu:
   * @toolbar: the #GtkToolbar which emitted the signal
   * @x: the x coordinate of the point where the menu should appear
   * @y: the y coordinate of the point where the menu should appear
   * @button: the mouse button the user pressed, or -1
   *
   * Emitted when the user right-clicks the toolbar or uses the
   * keybinding to display a popup menu.
   *
   * Application developers should handle this signal if they want
   * to display a context menu on the toolbar. The context-menu should
   * appear at the coordinates given by @x and @y. The mouse button
   * number is given by the @button parameter. If the menu was popped
   * up using the keybaord, @button is -1.
   *
   * Return value: return %TRUE if the signal was handled, %FALSE if not
   */
  toolbar_signals[POPUP_CONTEXT_MENU] =
    g_signal_new (I_("popup-context-menu"),
		  G_OBJECT_CLASS_TYPE (klass),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkToolbarClass, popup_context_menu),
		  _gtk_boolean_handled_accumulator, NULL,
		  _gtk_marshal_BOOLEAN__INT_INT_INT,
		  G_TYPE_BOOLEAN, 3,
		  G_TYPE_INT, G_TYPE_INT,
		  G_TYPE_INT);

  /**
   * GtkToolbar::focus-home-or-end:
   * @toolbar: the #GtkToolbar which emitted the signal
   * @focus_home: %TRUE if the first item should be focused
   *
   * A keybinding signal used internally by GTK+. This signal can't
   * be used in application code
   *
   * Return value: %TRUE if the signal was handled, %FALSE if not
   */
  toolbar_signals[FOCUS_HOME_OR_END] =
    g_signal_new_class_handler (I_("focus-home-or-end"),
                                G_OBJECT_CLASS_TYPE (klass),
                                G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                G_CALLBACK (gtk_toolbar_focus_home_or_end),
                                NULL, NULL,
                                _gtk_marshal_BOOLEAN__BOOLEAN,
                                G_TYPE_BOOLEAN, 1,
                                G_TYPE_BOOLEAN);

  /* properties */
  g_object_class_override_property (gobject_class,
                                    PROP_ORIENTATION,
                                    "orientation");

  g_object_class_install_property (gobject_class,
				   PROP_TOOLBAR_STYLE,
				   g_param_spec_enum ("toolbar-style",
 						      P_("Toolbar Style"),
 						      P_("How to draw the toolbar"),
 						      GTK_TYPE_TOOLBAR_STYLE,
 						      DEFAULT_TOOLBAR_STYLE,
 						      GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
				   PROP_SHOW_ARROW,
				   g_param_spec_boolean ("show-arrow",
							 P_("Show Arrow"),
							 P_("If an arrow should be shown if the toolbar doesn't fit"),
							 TRUE,
							 GTK_PARAM_READWRITE));

  /**
   * GtkToolbar:icon-size:
   *
   * The size of the icons in a toolbar is normally determined by
   * the toolbar-icon-size setting. When this property is set, it 
   * overrides the setting. 
   * 
   * This should only be used for special-purpose toolbars, normal
   * application toolbars should respect the user preferences for the
   * size of icons.
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
				   PROP_ICON_SIZE,
				   g_param_spec_int ("icon-size",
						     P_("Icon size"),
						     P_("Size of icons in this toolbar"),
						     0, G_MAXINT,
						     DEFAULT_ICON_SIZE,
						     GTK_PARAM_READWRITE));  

  /**
   * GtkToolbar:icon-size-set:
   *
   * Is %TRUE if the icon-size property has been set.
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
				   PROP_ICON_SIZE_SET,
				   g_param_spec_boolean ("icon-size-set",
							 P_("Icon size set"),
							 P_("Whether the icon-size property has been set"),
							 FALSE,
							 GTK_PARAM_READWRITE));  

  /* child properties */
  gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_EXPAND,
					      g_param_spec_boolean ("expand", 
								    P_("Expand"), 
								    P_("Whether the item should receive extra space when the toolbar grows"),
								    FALSE,
								    GTK_PARAM_READWRITE));
  
  gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_HOMOGENEOUS,
					      g_param_spec_boolean ("homogeneous", 
								    P_("Homogeneous"), 
								    P_("Whether the item should be the same size as other homogeneous items"),
								    FALSE,
								    GTK_PARAM_READWRITE));
  
  /* style properties */
  gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("space-size",
							     P_("Spacer size"),
							     P_("Size of spacers"),
							     0,
							     G_MAXINT,
                                                             DEFAULT_SPACE_SIZE,
							     GTK_PARAM_READABLE));
  
  gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("internal-padding",
							     P_("Internal padding"),
							     P_("Amount of border space between the toolbar shadow and the buttons"),
							     0,
							     G_MAXINT,
                                                             DEFAULT_IPADDING,
                                                             GTK_PARAM_READABLE));

  gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("max-child-expand",
                                                             P_("Maximum child expand"),
                                                             P_("Maximum amount of space an expandable item will be given"),
                                                             0,
                                                             G_MAXINT,
                                                             G_MAXINT,
                                                             GTK_PARAM_READABLE));

  gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_enum ("space-style",
							      P_("Space style"),
							      P_("Whether spacers are vertical lines or just blank"),
                                                              GTK_TYPE_TOOLBAR_SPACE_STYLE,
                                                              DEFAULT_SPACE_STYLE,
                                                              GTK_PARAM_READABLE));
  
  gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_enum ("button-relief",
							      P_("Button relief"),
							      P_("Type of bevel around toolbar buttons"),
                                                              GTK_TYPE_RELIEF_STYLE,
                                                              GTK_RELIEF_NONE,
                                                              GTK_PARAM_READABLE));
  gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_enum ("shadow-type",
                                                              P_("Shadow type"),
                                                              P_("Style of bevel around the toolbar"),
                                                              GTK_TYPE_SHADOW_TYPE,
                                                              GTK_SHADOW_OUT,
                                                              GTK_PARAM_READABLE));

  binding_set = gtk_binding_set_by_class (klass);
  
  add_arrow_bindings (binding_set, GDK_KEY_Left, GTK_DIR_LEFT);
  add_arrow_bindings (binding_set, GDK_KEY_Right, GTK_DIR_RIGHT);
  add_arrow_bindings (binding_set, GDK_KEY_Up, GTK_DIR_UP);
  add_arrow_bindings (binding_set, GDK_KEY_Down, GTK_DIR_DOWN);
  
  gtk_binding_entry_add_signal (binding_set, GDK_KEY_KP_Home, 0,
                                "focus-home-or-end", 1,
				G_TYPE_BOOLEAN, TRUE);
  gtk_binding_entry_add_signal (binding_set, GDK_KEY_Home, 0,
                                "focus-home-or-end", 1,
				G_TYPE_BOOLEAN, TRUE);
  gtk_binding_entry_add_signal (binding_set, GDK_KEY_KP_End, 0,
                                "focus-home-or-end", 1,
				G_TYPE_BOOLEAN, FALSE);
  gtk_binding_entry_add_signal (binding_set, GDK_KEY_End, 0,
                                "focus-home-or-end", 1,
				G_TYPE_BOOLEAN, FALSE);
  
  add_ctrl_tab_bindings (binding_set, 0, GTK_DIR_TAB_FORWARD);
  add_ctrl_tab_bindings (binding_set, GDK_SHIFT_MASK, GTK_DIR_TAB_BACKWARD);

  g_type_class_add_private (gobject_class, sizeof (GtkToolbarPrivate));
}

static void
toolbar_tool_shell_iface_init (GtkToolShellIface *iface)
{
  iface->get_icon_size    = toolbar_get_icon_size;
  iface->get_orientation  = toolbar_get_orientation;
  iface->get_style        = toolbar_get_style;
  iface->get_relief_style = toolbar_get_relief_style;
  iface->rebuild_menu     = toolbar_rebuild_menu;
}

static void
gtk_toolbar_init (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv;
  GtkStyleContext *context;

  toolbar->priv = G_TYPE_INSTANCE_GET_PRIVATE (toolbar,
                                               GTK_TYPE_TOOLBAR,
                                               GtkToolbarPrivate);
  priv = toolbar->priv;

  gtk_widget_set_can_focus (GTK_WIDGET (toolbar), FALSE);
  gtk_widget_set_has_window (GTK_WIDGET (toolbar), FALSE);

  priv->orientation = GTK_ORIENTATION_HORIZONTAL;
  priv->style = DEFAULT_TOOLBAR_STYLE;
  priv->icon_size = DEFAULT_ICON_SIZE;
  priv->animation = DEFAULT_ANIMATION_STATE;

  priv->arrow_button = gtk_toggle_button_new ();
  g_signal_connect (priv->arrow_button, "button-press-event",
		    G_CALLBACK (gtk_toolbar_arrow_button_press), toolbar);
  g_signal_connect (priv->arrow_button, "clicked",
		    G_CALLBACK (gtk_toolbar_arrow_button_clicked), toolbar);
  gtk_button_set_relief (GTK_BUTTON (priv->arrow_button),
			 get_button_relief (toolbar));

  gtk_button_set_focus_on_click (GTK_BUTTON (priv->arrow_button), FALSE);

  priv->arrow = gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_NONE);
  gtk_widget_set_name (priv->arrow, "gtk-toolbar-arrow");
  gtk_widget_show (priv->arrow);
  gtk_container_add (GTK_CONTAINER (priv->arrow_button), priv->arrow);
  
  gtk_widget_set_parent (priv->arrow_button, GTK_WIDGET (toolbar));
  
  /* which child position a drop will occur at */
  priv->menu = NULL;
  priv->show_arrow = TRUE;
  priv->settings = NULL;
  
  priv->max_homogeneous_pixels = -1;
  
  priv->timer = g_timer_new ();

  context = gtk_widget_get_style_context (GTK_WIDGET (toolbar));
  gtk_style_context_add_class (context, GTK_STYLE_CLASS_TOOLBAR);
}

static void
gtk_toolbar_set_property (GObject      *object,
			  guint         prop_id,
			  const GValue *value,
			  GParamSpec   *pspec)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (object);
  GtkToolbarPrivate *priv = toolbar->priv;

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      g_signal_emit (toolbar, toolbar_signals[ORIENTATION_CHANGED], 0,
                     g_value_get_enum (value));
      break;
    case PROP_TOOLBAR_STYLE:
      gtk_toolbar_set_style (toolbar, g_value_get_enum (value));
      break;
    case PROP_SHOW_ARROW:
      gtk_toolbar_set_show_arrow (toolbar, g_value_get_boolean (value));
      break;
    case PROP_ICON_SIZE:
      gtk_toolbar_set_icon_size (toolbar, g_value_get_int (value));
      break;
    case PROP_ICON_SIZE_SET:
      if (g_value_get_boolean (value))
	priv->icon_size_set = TRUE;
      else
	gtk_toolbar_unset_icon_size (toolbar);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_toolbar_get_property (GObject    *object,
			  guint       prop_id,
			  GValue     *value,
			  GParamSpec *pspec)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (object);
  GtkToolbarPrivate *priv = toolbar->priv;

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      g_value_set_enum (value, priv->orientation);
      break;
    case PROP_TOOLBAR_STYLE:
      g_value_set_enum (value, priv->style);
      break;
    case PROP_SHOW_ARROW:
      g_value_set_boolean (value, priv->show_arrow);
      break;
    case PROP_ICON_SIZE:
      g_value_set_int (value, gtk_toolbar_get_icon_size (toolbar));
      break;
    case PROP_ICON_SIZE_SET:
      g_value_set_boolean (value, priv->icon_size_set);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_toolbar_map (GtkWidget *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;

  GTK_WIDGET_CLASS (gtk_toolbar_parent_class)->map (widget);

  if (priv->event_window)
    gdk_window_show_unraised (priv->event_window);
}

static void
gtk_toolbar_unmap (GtkWidget *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->event_window)
    gdk_window_hide (priv->event_window);
  
  GTK_WIDGET_CLASS (gtk_toolbar_parent_class)->unmap (widget);
}

static void
gtk_toolbar_realize (GtkWidget *widget)
{
  GtkAllocation allocation;
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GdkWindow *window;
  GdkWindowAttr attributes;
  gint attributes_mask;
  guint border_width;

  gtk_widget_set_realized (widget, TRUE);

  gtk_widget_get_allocation (widget, &allocation);
  border_width = gtk_container_get_border_width (GTK_CONTAINER (widget));

  attributes.wclass = GDK_INPUT_ONLY;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = allocation.x + border_width;
  attributes.y = allocation.y + border_width;
  attributes.width = allocation.width - border_width * 2;
  attributes.height = allocation.height - border_width * 2;
  attributes.event_mask = gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_BUTTON_PRESS_MASK |
			    GDK_BUTTON_RELEASE_MASK |
			    GDK_ENTER_NOTIFY_MASK |
			    GDK_LEAVE_NOTIFY_MASK);

  attributes_mask = GDK_WA_X | GDK_WA_Y;

  window = gtk_widget_get_parent_window (widget);
  gtk_widget_set_window (widget, window);
  g_object_ref (window);

  priv->event_window = gdk_window_new (gtk_widget_get_parent_window (widget),
				       &attributes, attributes_mask);
  gdk_window_set_user_data (priv->event_window, toolbar);
}

static void
gtk_toolbar_unrealize (GtkWidget *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->event_window)
    {
      gdk_window_set_user_data (priv->event_window, NULL);
      gdk_window_destroy (priv->event_window);
      priv->event_window = NULL;
    }

  GTK_WIDGET_CLASS (gtk_toolbar_parent_class)->unrealize (widget);
}

static gint
gtk_toolbar_draw (GtkWidget *widget,
                  cairo_t   *cr)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkStyleContext *context;
  GtkStateFlags state;
  GList *list;
  guint border_width;

  border_width = gtk_container_get_border_width (GTK_CONTAINER (widget));
  context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);

  gtk_style_context_save (context);
  gtk_style_context_set_state (context, state);

  gtk_render_background (context, cr, border_width, border_width,
                         gtk_widget_get_allocated_width (widget) - 2 * border_width,
                         gtk_widget_get_allocated_height (widget) - 2 * border_width);
  gtk_render_frame (context, cr, border_width, border_width,
                    gtk_widget_get_allocated_width (widget) - 2 * border_width,
                    gtk_widget_get_allocated_height (widget) - 2 * border_width);

  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      toolbar_content_draw (content, GTK_CONTAINER (widget), cr);
    }
  
  gtk_container_propagate_draw (GTK_CONTAINER (widget),
				priv->arrow_button,
				cr);

  gtk_style_context_restore (context);

  return FALSE;
}

static void
gtk_toolbar_size_request (GtkWidget      *widget,
			  GtkRequisition *requisition)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;
  gint max_child_height;
  gint max_child_width;
  gint max_homogeneous_child_width;
  gint max_homogeneous_child_height;
  gint homogeneous_size;
  gint long_req;
  gint pack_front_size;
  gint ipadding;
  guint border_width;
  GtkRequisition arrow_requisition;
  
  max_homogeneous_child_width = 0;
  max_homogeneous_child_height = 0;
  max_child_width = 0;
  max_child_height = 0;
  for (list = priv->content; list != NULL; list = list->next)
    {
      GtkRequisition requisition;
      ToolbarContent *content = list->data;
      
      if (!toolbar_content_visible (content, toolbar))
	continue;
      
      toolbar_content_size_request (content, toolbar, &requisition);

      max_child_width = MAX (max_child_width, requisition.width);
      max_child_height = MAX (max_child_height, requisition.height);
      
      if (toolbar_content_is_homogeneous (content, toolbar))
	{
	  max_homogeneous_child_width = MAX (max_homogeneous_child_width, requisition.width);
	  max_homogeneous_child_height = MAX (max_homogeneous_child_height, requisition.height);
	}
    }
  
  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    homogeneous_size = max_homogeneous_child_width;
  else
    homogeneous_size = max_homogeneous_child_height;
  
  pack_front_size = 0;
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      guint size;
      
      if (!toolbar_content_visible (content, toolbar))
	continue;

      if (toolbar_content_is_homogeneous (content, toolbar))
	{
	  size = homogeneous_size;
	}
      else
	{
	  GtkRequisition requisition;
	  
	  toolbar_content_size_request (content, toolbar, &requisition);
	  
	  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
	    size = requisition.width;
	  else
	    size = requisition.height;
	}

      pack_front_size += size;
    }
  
  if (priv->show_arrow)
    {
      gtk_widget_get_preferred_size (priv->arrow_button,
                                     &arrow_requisition, NULL);

      if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
	long_req = arrow_requisition.width;
      else
	long_req = arrow_requisition.height;
      
      /* There is no point requesting space for the arrow if that would take
       * up more space than all the items combined
       */
      long_req = MIN (long_req, pack_front_size);
    }
  else
    {
      arrow_requisition.height = 0;
      arrow_requisition.width = 0;
      
      long_req = pack_front_size;
    }
  
  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      requisition->width = long_req;
      requisition->height = MAX (max_child_height, arrow_requisition.height);
    }
  else
    {
      requisition->height = long_req;
      requisition->width = MAX (max_child_width, arrow_requisition.width);
    }
  
  /* Extra spacing */
  ipadding = get_internal_padding (toolbar);

  border_width = gtk_container_get_border_width (GTK_CONTAINER (toolbar));
  requisition->width += 2 * (ipadding + border_width);
  requisition->height += 2 * (ipadding + border_width);
  
  if (get_shadow_type (toolbar) != GTK_SHADOW_NONE)
    {
      GtkStyleContext *context;
      GtkStateFlags state;
      GtkBorder padding;

      context = gtk_widget_get_style_context (widget);
      state = gtk_widget_get_state_flags (widget);
      gtk_style_context_get_padding (context, state, &padding);

      requisition->width += padding.left + padding.right;
      requisition->height += padding.top + padding.bottom;
    }
  
  priv->button_maxw = max_homogeneous_child_width;
  priv->button_maxh = max_homogeneous_child_height;
}

static void
gtk_toolbar_get_preferred_width (GtkWidget *widget,
                                 gint      *minimum,
                                 gint      *natural)
{
  GtkRequisition requisition;

  gtk_toolbar_size_request (widget, &requisition);

  *minimum = *natural = requisition.width;
}

static void
gtk_toolbar_get_preferred_height (GtkWidget *widget,
                                  gint      *minimum,
                                  gint      *natural)
{
  GtkRequisition requisition;

  gtk_toolbar_size_request (widget, &requisition);

  *minimum = *natural = requisition.height;
}

static gint
position (GtkToolbar *toolbar,
          gint        from,
          gint        to,
          gdouble     elapsed)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  gint n_pixels;

  if (!priv->animation)
    return to;

  if (elapsed <= ACCEL_THRESHOLD)
    {
      n_pixels = SLIDE_SPEED * elapsed;
    }
  else
    {
      /* The formula is a second degree polynomial in
       * @elapsed that has the line SLIDE_SPEED * @elapsed
       * as tangent for @elapsed == ACCEL_THRESHOLD.
       * This makes @n_pixels a smooth function of elapsed time.
       */
      n_pixels = (SLIDE_SPEED / ACCEL_THRESHOLD) * elapsed * elapsed -
	SLIDE_SPEED * elapsed + SLIDE_SPEED * ACCEL_THRESHOLD;
    }

  if (to > from)
    return MIN (from + n_pixels, to);
  else
    return MAX (from - n_pixels, to);
}

static void
compute_intermediate_allocation (GtkToolbar          *toolbar,
				 const GtkAllocation *start,
				 const GtkAllocation *goal,
				 GtkAllocation       *intermediate)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  gdouble elapsed = g_timer_elapsed (priv->timer, NULL);

  intermediate->x      = position (toolbar, start->x, goal->x, elapsed);
  intermediate->y      = position (toolbar, start->y, goal->y, elapsed);
  intermediate->width  = position (toolbar, start->x + start->width,
                                   goal->x + goal->width,
                                   elapsed) - intermediate->x;
  intermediate->height = position (toolbar, start->y + start->height,
                                   goal->y + goal->height,
                                   elapsed) - intermediate->y;
}

static void
fixup_allocation_for_rtl (gint           total_size,
			  GtkAllocation *allocation)
{
  allocation->x += (total_size - (2 * allocation->x + allocation->width));
}

static void
fixup_allocation_for_vertical (GtkAllocation *allocation)
{
  gint tmp;
  
  tmp = allocation->x;
  allocation->x = allocation->y;
  allocation->y = tmp;
  
  tmp = allocation->width;
  allocation->width = allocation->height;
  allocation->height = tmp;
}

static gint
get_item_size (GtkToolbar     *toolbar,
	       ToolbarContent *content)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkRequisition requisition;
  
  toolbar_content_size_request (content, toolbar, &requisition);

  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      if (toolbar_content_is_homogeneous (content, toolbar))
	return priv->button_maxw;
      else
	return requisition.width;
    }
  else
    {
      if (toolbar_content_is_homogeneous (content, toolbar))
	return priv->button_maxh;
      else
	return requisition.height;
    }
}

static gboolean
slide_idle_handler (gpointer data)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (data);
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;

  if (priv->need_sync)
    {
      gdk_flush ();
      priv->need_sync = FALSE;
    }
  
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      ItemState state;
      GtkAllocation goal_allocation;
      GtkAllocation allocation;
      gboolean cont;

      state = toolbar_content_get_state (content);
      toolbar_content_get_goal_allocation (content, &goal_allocation);
      toolbar_content_get_allocation (content, &allocation);
      
      cont = FALSE;
      
      if (state == NOT_ALLOCATED)
	{
	  /* an unallocated item means that size allocate has to
	   * called at least once more
	   */
	  cont = TRUE;
	}

      /* An invisible item with a goal allocation of
       * 0 is already at its goal.
       */
      if ((state == NORMAL || state == OVERFLOWN) &&
	  ((goal_allocation.width != 0 &&
	    goal_allocation.height != 0) ||
	   toolbar_content_child_visible (content)))
	{
	  if ((goal_allocation.x != allocation.x ||
	       goal_allocation.y != allocation.y ||
	       goal_allocation.width != allocation.width ||
	       goal_allocation.height != allocation.height))
	    {
	      /* An item is not in its right position yet. Note
	       * that OVERFLOWN items do get an allocation in
	       * gtk_toolbar_size_allocate(). This way you can see
	       * them slide back in when you drag an item off the
	       * toolbar.
	       */
	      cont = TRUE;
	    }
	}

      if (toolbar_content_is_placeholder (content) &&
	  toolbar_content_disappearing (content) &&
	  toolbar_content_child_visible (content))
	{
	  /* A disappearing placeholder is still visible.
	   */
	     
	  cont = TRUE;
	}
      
      if (cont)
	{
	  gtk_widget_queue_resize_no_redraw (GTK_WIDGET (toolbar));
	  
	  return TRUE;
	}
    }
  
  gtk_widget_queue_resize_no_redraw (GTK_WIDGET (toolbar));

  priv->is_sliding = FALSE;
  priv->idle_id = 0;

  return FALSE;
}

static gboolean
rect_within (GtkAllocation *a1,
	     GtkAllocation *a2)
{
  return (a1->x >= a2->x                         &&
	  a1->x + a1->width <= a2->x + a2->width &&
	  a1->y >= a2->y			 &&
	  a1->y + a1->height <= a2->y + a2->height);
}

static void
gtk_toolbar_begin_sliding (GtkToolbar *toolbar)
{
  GtkAllocation allocation;
  GtkWidget *widget = GTK_WIDGET (toolbar);
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkStyleContext *context;
  GtkStateFlags state;
  GtkBorder padding;
  GList *list;
  gint cur_x;
  gint cur_y;
  gint border_width;
  gboolean rtl;
  gboolean vertical;
  
  /* Start the sliding. This function copies the allocation of every
   * item into content->start_allocation. For items that haven't
   * been allocated yet, we calculate their position and save that
   * in start_allocatino along with zero width and zero height.
   *
   * FIXME: It would be nice if we could share this code with
   * the equivalent in gtk_widget_size_allocate().
   */
  priv->is_sliding = TRUE;
  
  if (!priv->idle_id)
    priv->idle_id = gdk_threads_add_idle (slide_idle_handler, toolbar);

  gtk_widget_get_allocation (widget, &allocation);
  context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);
  gtk_style_context_get_padding (context, state, &padding);

  rtl = (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL);
  vertical = (priv->orientation == GTK_ORIENTATION_VERTICAL);
  border_width = get_internal_padding (toolbar) + gtk_container_get_border_width (GTK_CONTAINER (toolbar));
  
  if (rtl)
    {
      cur_x = allocation.width - border_width - padding.right;
      cur_y = allocation.height - border_width - padding.top;
    }
  else
    {
      cur_x = border_width + padding.left;
      cur_y = border_width + padding.top;
    }

  cur_x += allocation.x;
  cur_y += allocation.y;

  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      GtkAllocation new_start_allocation;
      GtkAllocation item_allocation;
      ItemState state;
      
      state = toolbar_content_get_state (content);
      toolbar_content_get_allocation (content, &item_allocation);
      
      if ((state == NORMAL &&
	   rect_within (&item_allocation, &allocation)) ||
	  state == OVERFLOWN)
	{
	  new_start_allocation = item_allocation;
	}
      else
	{
	  new_start_allocation.x = cur_x;
	  new_start_allocation.y = cur_y;
	  
	  if (vertical)
	    {
	      new_start_allocation.width = allocation.width -
                                           2 * border_width -
                                           padding.left - padding.right;
	      new_start_allocation.height = 0;
	    }
	  else
	    {
	      new_start_allocation.width = 0;
	      new_start_allocation.height = allocation.height -
                                            2 * border_width -
                                            padding.top - padding.bottom;
	    }
	}
      
      if (vertical)
	cur_y = new_start_allocation.y + new_start_allocation.height;
      else if (rtl)
	cur_x = new_start_allocation.x;
      else
	cur_x = new_start_allocation.x + new_start_allocation.width;
      
      toolbar_content_set_start_allocation (content, &new_start_allocation);
    }

  /* This resize will run before the first idle handler. This
   * will make sure that items get the right goal allocation
   * so that the idle handler will not immediately return
   * FALSE
   */
  gtk_widget_queue_resize_no_redraw (GTK_WIDGET (toolbar));
  g_timer_reset (priv->timer);
}

static void
gtk_toolbar_stop_sliding (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->is_sliding)
    {
      GList *list;
      
      priv->is_sliding = FALSE;
      
      if (priv->idle_id)
	{
	  g_source_remove (priv->idle_id);
	  priv->idle_id = 0;
	}
      
      list = priv->content;
      while (list)
	{
	  ToolbarContent *content = list->data;
	  list = list->next;

	  if (toolbar_content_is_placeholder (content))
	    {
	      toolbar_content_remove (content, toolbar);
	      toolbar_content_free (content);
	    }
	}
      
      gtk_widget_queue_resize_no_redraw (GTK_WIDGET (toolbar));
    }
}

static void
remove_item (GtkWidget *menu_item,
	     gpointer   data)
{
  gtk_container_remove (GTK_CONTAINER (gtk_widget_get_parent (menu_item)),
                        menu_item);
}

static void
menu_deactivated (GtkWidget  *menu,
		  GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->arrow_button), FALSE);
}

static void
menu_detached (GtkWidget  *widget,
	       GtkMenu    *menu)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;

  priv->menu = NULL;
}

static void
rebuild_menu (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list, *children;

  if (!priv->menu)
    {
      priv->menu = GTK_MENU (gtk_menu_new());
      gtk_menu_attach_to_widget (priv->menu,
				 GTK_WIDGET (toolbar),
				 menu_detached);

      g_signal_connect (priv->menu, "deactivate",
                        G_CALLBACK (menu_deactivated), toolbar);
    }

  gtk_container_foreach (GTK_CONTAINER (priv->menu), remove_item, NULL);
  
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      if (toolbar_content_get_state (content) == OVERFLOWN &&
	  !toolbar_content_is_placeholder (content))
	{
	  GtkWidget *menu_item = toolbar_content_retrieve_menu_item (content);
	  
	  if (menu_item)
	    {
	      g_assert (GTK_IS_MENU_ITEM (menu_item));
	      gtk_menu_shell_append (GTK_MENU_SHELL (priv->menu), menu_item);
	    }
	}
    }

  /* Remove leading and trailing separator items */
  children = gtk_container_get_children (GTK_CONTAINER (priv->menu));
  
  list = children;
  while (list && GTK_IS_SEPARATOR_MENU_ITEM (list->data))
    {
      GtkWidget *child = list->data;
      
      gtk_container_remove (GTK_CONTAINER (priv->menu), child);
      list = list->next;
    }
  g_list_free (children);

  /* Regenerate the list of children so we don't try to remove items twice */
  children = gtk_container_get_children (GTK_CONTAINER (priv->menu));

  list = g_list_last (children);
  while (list && GTK_IS_SEPARATOR_MENU_ITEM (list->data))
    {
      GtkWidget *child = list->data;

      gtk_container_remove (GTK_CONTAINER (priv->menu), child);
      list = list->prev;
    }
  g_list_free (children);

  priv->need_rebuild = FALSE;
}

static void
gtk_toolbar_size_allocate (GtkWidget     *widget,
			   GtkAllocation *allocation)
{
  GtkAllocation widget_allocation;
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkAllocation *allocations;
  ItemState *new_states;
  GtkAllocation arrow_allocation;
  GtkStyleContext *context;
  GtkStateFlags state;
  GtkBorder padding;
  gint arrow_size;
  gint size, pos, short_size;
  GList *list;
  gint i;
  gboolean need_arrow;
  gint n_expand_items;
  gint border_width;
  gint available_size;
  gint n_items;
  gint needed_size;
  GtkRequisition arrow_requisition;
  gboolean overflowing;
  gboolean size_changed;
  GtkAllocation item_area;
  GtkShadowType shadow_type;

  context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);
  gtk_style_context_get_padding (context, state, &padding);

  gtk_widget_get_allocation (widget, &widget_allocation);
  size_changed = FALSE;
  if (widget_allocation.x != allocation->x ||
      widget_allocation.y != allocation->y ||
      widget_allocation.width != allocation->width ||
      widget_allocation.height != allocation->height)
    {
      size_changed = TRUE;
    }

  if (size_changed)
    gtk_toolbar_stop_sliding (toolbar);

  gtk_widget_set_allocation (widget, allocation);

  border_width = gtk_container_get_border_width (GTK_CONTAINER (toolbar));

  if (gtk_widget_get_realized (widget))
    gdk_window_move_resize (priv->event_window,
                            allocation->x + border_width,
                            allocation->y + border_width,
                            allocation->width - border_width * 2,
                            allocation->height - border_width * 2);

  border_width += get_internal_padding (toolbar);

  gtk_widget_get_preferred_size (priv->arrow_button,
                                 &arrow_requisition, NULL);

  shadow_type = get_shadow_type (toolbar);

  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      available_size = size = allocation->width - 2 * border_width;
      short_size = allocation->height - 2 * border_width;
      arrow_size = arrow_requisition.width;

      if (shadow_type != GTK_SHADOW_NONE)
        {
          available_size -= padding.left + padding.right;
          short_size -= padding.top + padding.bottom;
        }
    }
  else
    {
      available_size = size = allocation->height - 2 * border_width;
      short_size = allocation->width - 2 * border_width;
      arrow_size = arrow_requisition.height;

      if (shadow_type != GTK_SHADOW_NONE)
        {
          available_size -= padding.top + padding.bottom;
          short_size -= padding.left + padding.right;
        }
    }

  n_items = g_list_length (priv->content);
  allocations = g_new0 (GtkAllocation, n_items);
  new_states = g_new0 (ItemState, n_items);

  needed_size = 0;
  need_arrow = FALSE;
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;

      if (toolbar_content_visible (content, toolbar))
        {
          needed_size += get_item_size (toolbar, content);

          /* Do we need an arrow?
           *
           * Assume we don't, and see if any non-separator item
           * with a proxy menu item is then going to overflow.
           */
          if (needed_size > available_size &&
              !need_arrow &&
              priv->show_arrow &&
              toolbar_content_has_proxy_menu_item (content) &&
              !toolbar_content_is_separator (content))
            {
              need_arrow = TRUE;
            }
        }
    }

  if (need_arrow)
    size = available_size - arrow_size;
  else
    size = available_size;

  /* calculate widths and states of items */
  overflowing = FALSE;
  for (list = priv->content, i = 0; list != NULL; list = list->next, ++i)
    {
      ToolbarContent *content = list->data;
      gint item_size;

      if (!toolbar_content_visible (content, toolbar))
        {
          new_states[i] = HIDDEN;
          continue;
        }

      item_size = get_item_size (toolbar, content);
      if (item_size <= size && !overflowing)
        {
          size -= item_size;
          allocations[i].width = item_size;
          new_states[i] = NORMAL;
        }
      else
        {
          overflowing = TRUE;
          new_states[i] = OVERFLOWN;
          allocations[i].width = item_size;
        }
    }

  /* calculate width of arrow */
  if (need_arrow)
    {
      arrow_allocation.width = arrow_size;
      arrow_allocation.height = MAX (short_size, 1);
    }

  /* expand expandable items */

  /* We don't expand when there is an overflow menu,
   * because that leads to weird jumps when items get
   * moved to the overflow menu and the expanding
   * items suddenly get a lot of extra space
   */
  if (!overflowing)
    {
      gint max_child_expand;
      n_expand_items = 0;

      for (i = 0, list = priv->content; list != NULL; list = list->next, ++i)
        {
          ToolbarContent *content = list->data;

          if (toolbar_content_get_expand (content) && new_states[i] == NORMAL)
            n_expand_items++;
        }

      max_child_expand = get_max_child_expand (toolbar);
      for (list = priv->content, i = 0; list != NULL; list = list->next, ++i)
        {
          ToolbarContent *content = list->data;

          if (toolbar_content_get_expand (content) && new_states[i] == NORMAL)
            {
              gint extra = size / n_expand_items;
              if (size % n_expand_items != 0)
                extra++;

              if (extra > max_child_expand)
                extra = max_child_expand;

              allocations[i].width += extra;
              size -= extra;
              n_expand_items--;
            }
        }

      g_assert (n_expand_items == 0);
    }

  /* position items */
  pos = border_width;
  for (list = priv->content, i = 0; list != NULL; list = list->next, ++i)
    {
      /* Both NORMAL and OVERFLOWN items get a position.
       * This ensures that sliding will work for OVERFLOWN items too.
       */
      if (new_states[i] == NORMAL || new_states[i] == OVERFLOWN)
        {
          allocations[i].x = pos;
          allocations[i].y = border_width;
          allocations[i].height = short_size;

          pos += allocations[i].width;
        }
    }

  /* position arrow */
  if (need_arrow)
    {
      arrow_allocation.x = available_size - border_width - arrow_allocation.width;
      arrow_allocation.y = border_width;
    }

  item_area.x = border_width;
  item_area.y = border_width;
  item_area.width = available_size - (need_arrow? arrow_size : 0);
  item_area.height = short_size;

  /* fix up allocations in the vertical or RTL cases */
  if (priv->orientation == GTK_ORIENTATION_VERTICAL)
    {
      for (i = 0; i < n_items; ++i)
        fixup_allocation_for_vertical (&(allocations[i]));

      if (need_arrow)
        fixup_allocation_for_vertical (&arrow_allocation);

      fixup_allocation_for_vertical (&item_area);
    }
  else if (gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_RTL)
    {
      for (i = 0; i < n_items; ++i)
        fixup_allocation_for_rtl (available_size, &(allocations[i]));

      if (need_arrow)
        fixup_allocation_for_rtl (available_size, &arrow_allocation);

      fixup_allocation_for_rtl (available_size, &item_area);
    }

  /* translate the items by allocation->(x,y) */
  for (i = 0; i < n_items; ++i)
    {
      allocations[i].x += allocation->x;
      allocations[i].y += allocation->y;

      if (shadow_type != GTK_SHADOW_NONE)
        {
          allocations[i].x += padding.left;
          allocations[i].y += padding.top;
        }
    }

  if (need_arrow)
    {
      arrow_allocation.x += allocation->x;
      arrow_allocation.y += allocation->y;

      if (shadow_type != GTK_SHADOW_NONE)
        {
          arrow_allocation.x += padding.left;
          arrow_allocation.y += padding.top;
        }
    }

  item_area.x += allocation->x;
  item_area.y += allocation->y;
  if (shadow_type != GTK_SHADOW_NONE)
    {
      item_area.x += padding.left;
      item_area.y += padding.top;
    }

  /* did anything change? */
  for (list = priv->content, i = 0; list != NULL; list = list->next, i++)
    {
      ToolbarContent *content = list->data;

      if (toolbar_content_get_state (content) == NORMAL &&
          new_states[i] != NORMAL)
        {
          /* an item disappeared and we didn't change size, so begin sliding */
          if (!size_changed)
            gtk_toolbar_begin_sliding (toolbar);
        }
    }

  /* finally allocate the items */
  if (priv->is_sliding)
    {
      for (list = priv->content, i = 0; list != NULL; list = list->next, i++)
        {
          ToolbarContent *content = list->data;

          toolbar_content_set_goal_allocation (content, &(allocations[i]));
        }
    }

  for (list = priv->content, i = 0; list != NULL; list = list->next, ++i)
    {
      ToolbarContent *content = list->data;

      if (new_states[i] == OVERFLOWN || new_states[i] == NORMAL)
        {
          GtkAllocation alloc;
          GtkAllocation start_allocation = { 0, };
          GtkAllocation goal_allocation;

          if (priv->is_sliding)
            {
              toolbar_content_get_start_allocation (content, &start_allocation);
              toolbar_content_get_goal_allocation (content, &goal_allocation);

              compute_intermediate_allocation (toolbar,
                                               &start_allocation,
                                               &goal_allocation,
                                               &alloc);

              priv->need_sync = TRUE;
            }
          else
            {
              alloc = allocations[i];
            }

          if (alloc.width <= 0 || alloc.height <= 0)
            {
              toolbar_content_set_child_visible (content, toolbar, FALSE);
            }
          else
            {
              if (!rect_within (&alloc, &item_area))
                {
                  toolbar_content_set_child_visible (content, toolbar, FALSE);
                  toolbar_content_size_allocate (content, &alloc);
                }
              else
                {
                  toolbar_content_set_child_visible (content, toolbar, TRUE);
                  toolbar_content_size_allocate (content, &alloc);
                }
            }
        }
      else
        {
          toolbar_content_set_child_visible (content, toolbar, FALSE);
        }

      toolbar_content_set_state (content, new_states[i]);
    }

  if (priv->menu && priv->need_rebuild)
    rebuild_menu (toolbar);

  if (need_arrow)
    {
      gtk_widget_size_allocate (GTK_WIDGET (priv->arrow_button),
                                &arrow_allocation);
      gtk_widget_show (GTK_WIDGET (priv->arrow_button));
    }
  else
    {
      gtk_widget_hide (GTK_WIDGET (priv->arrow_button));

      if (priv->menu && gtk_widget_get_visible (GTK_WIDGET (priv->menu)))
        gtk_menu_shell_deactivate (GTK_MENU_SHELL (priv->menu));
    }

  g_free (allocations);
  g_free (new_states);
}

static void
gtk_toolbar_update_button_relief (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkReliefStyle relief;

  relief = get_button_relief (toolbar);

  if (relief != gtk_button_get_relief (GTK_BUTTON (priv->arrow_button)))
    {
      gtk_toolbar_reconfigured (toolbar);
  
      gtk_button_set_relief (GTK_BUTTON (priv->arrow_button), relief);
    }
}

static void
gtk_toolbar_style_updated (GtkWidget *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;

  GTK_WIDGET_CLASS (gtk_toolbar_parent_class)->style_updated (widget);

  priv->max_homogeneous_pixels = -1;

  if (gtk_widget_get_realized (widget))
    gtk_style_context_set_background (gtk_widget_get_style_context (widget),
                                      gtk_widget_get_window (widget));

  gtk_toolbar_update_button_relief (GTK_TOOLBAR (widget));
}

static GList *
gtk_toolbar_list_children_in_focus_order (GtkToolbar       *toolbar,
					  GtkDirectionType  dir)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *result = NULL;
  GList *list;
  gboolean rtl;
  
  /* generate list of children in reverse logical order */
  
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      GtkWidget *widget;
      
      widget = toolbar_content_get_widget (content);
      
      if (widget)
	result = g_list_prepend (result, widget);
    }
  
  result = g_list_prepend (result, priv->arrow_button);
  
  rtl = (gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_RTL);
  
  /* move in logical order when
   *
   *	- dir is TAB_FORWARD
   *
   *	- in RTL mode and moving left or up
   *
   *    - in LTR mode and moving right or down
   */
  if (dir == GTK_DIR_TAB_FORWARD                                        ||
      (rtl  && (dir == GTK_DIR_UP   || dir == GTK_DIR_LEFT))		||
      (!rtl && (dir == GTK_DIR_DOWN || dir == GTK_DIR_RIGHT)))
    {
      result = g_list_reverse (result);
    }
  
  return result;
}

static gboolean
gtk_toolbar_focus_home_or_end (GtkToolbar *toolbar,
			       gboolean    focus_home)
{
  GList *children, *list;
  GtkDirectionType dir = focus_home? GTK_DIR_RIGHT : GTK_DIR_LEFT;
  
  children = gtk_toolbar_list_children_in_focus_order (toolbar, dir);
  
  if (gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_RTL)
    {
      children = g_list_reverse (children);
      
      dir = (dir == GTK_DIR_RIGHT)? GTK_DIR_LEFT : GTK_DIR_RIGHT;
    }
  
  for (list = children; list != NULL; list = list->next)
    {
      GtkWidget *child = list->data;

      if (gtk_container_get_focus_child (GTK_CONTAINER (toolbar)) == child)
	break;
      
      if (gtk_widget_get_mapped (child) && gtk_widget_child_focus (child, dir))
	break;
    }
  
  g_list_free (children);
  
  return TRUE;
}   

/* Keybinding handler. This function is called when the user presses
 * Ctrl TAB or an arrow key.
 */
static void
gtk_toolbar_move_focus (GtkWidget        *widget,
			GtkDirectionType  dir)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkContainer *container = GTK_CONTAINER (toolbar);
  GtkWidget *focus_child;
  GList *list;
  gboolean try_focus = FALSE;
  GList *children;

  focus_child = gtk_container_get_focus_child (container);

  if (focus_child && gtk_widget_child_focus (focus_child, dir))
    return;
  
  children = gtk_toolbar_list_children_in_focus_order (toolbar, dir);
  
  for (list = children; list != NULL; list = list->next)
    {
      GtkWidget *child = list->data;
      
      if (try_focus && gtk_widget_get_mapped (child) && gtk_widget_child_focus (child, dir))
	break;
      
      if (child == focus_child)
	try_focus = TRUE;
    }
  
  g_list_free (children);
}

/* The focus handler for the toolbar. It called when the user presses
 * TAB or otherwise tries to focus the toolbar.
 */
static gboolean
gtk_toolbar_focus (GtkWidget        *widget,
		   GtkDirectionType  dir)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GList *children, *list;
  gboolean result = FALSE;

  /* if focus is already somewhere inside the toolbar then return FALSE.
   * The only way focus can stay inside the toolbar is when the user presses
   * arrow keys or Ctrl TAB (both of which are handled by the
   * gtk_toolbar_move_focus() keybinding function.
   */
  if (gtk_container_get_focus_child (GTK_CONTAINER (widget)))
    return FALSE;

  children = gtk_toolbar_list_children_in_focus_order (toolbar, dir);

  for (list = children; list != NULL; list = list->next)
    {
      GtkWidget *child = list->data;
      
      if (gtk_widget_get_mapped (child) && gtk_widget_child_focus (child, dir))
	{
	  result = TRUE;
	  break;
	}
    }

  g_list_free (children);

  return result;
}

static GtkSettings *
toolbar_get_settings (GtkToolbar *toolbar)
{
  return toolbar->priv->settings;
}

static void
style_change_notify (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (!priv->style_set)
    {
      /* pretend it was set, then unset, thus reverting to new default */
      priv->style_set = TRUE;
      gtk_toolbar_unset_style (toolbar);
    }
}

static void
icon_size_change_notify (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (!priv->icon_size_set)
    {
      /* pretend it was set, then unset, thus reverting to new default */
      priv->icon_size_set = TRUE;
      gtk_toolbar_unset_icon_size (toolbar);
    }
}

static void
animation_change_notify (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkSettings *settings = toolbar_get_settings (toolbar);
  gboolean animation;

  if (settings)
    g_object_get (settings,
                  "gtk-enable-animations", &animation,
                  NULL);
  else
    animation = DEFAULT_ANIMATION_STATE;

  priv->animation = animation;
}

static void
settings_change_notify (GtkSettings      *settings,
                        const GParamSpec *pspec,
                        GtkToolbar       *toolbar)
{
  if (! strcmp (pspec->name, "gtk-toolbar-style"))
    style_change_notify (toolbar);
  else if (! strcmp (pspec->name, "gtk-toolbar-icon-size"))
    icon_size_change_notify (toolbar);
  else if (! strcmp (pspec->name, "gtk-enable-animations"))
    animation_change_notify (toolbar);
}

static void
gtk_toolbar_screen_changed (GtkWidget *widget,
			    GdkScreen *previous_screen)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkSettings *old_settings = toolbar_get_settings (toolbar);
  GtkSettings *settings;
  
  if (gtk_widget_has_screen (GTK_WIDGET (toolbar)))
    settings = gtk_widget_get_settings (GTK_WIDGET (toolbar));
  else
    settings = NULL;
  
  if (settings == old_settings)
    return;
  
  if (old_settings)
    {
      g_signal_handler_disconnect (old_settings, priv->settings_connection);

      g_object_unref (old_settings);
    }

  if (settings)
    {
      priv->settings_connection =
	g_signal_connect (settings, "notify",
                          G_CALLBACK (settings_change_notify),
                          toolbar);

      priv->settings = g_object_ref (settings);
    }
  else
    priv->settings = NULL;

  style_change_notify (toolbar);
  icon_size_change_notify (toolbar);
  animation_change_notify (toolbar);
}

static int
find_drop_index (GtkToolbar *toolbar,
		 gint        x,
		 gint        y)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *interesting_content;
  GList *list;
  GtkOrientation orientation;
  GtkTextDirection direction;
  gint best_distance = G_MAXINT;
  gint distance;
  gint cursor;
  gint pos;
  ToolbarContent *best_content;
  GtkAllocation allocation;
  
  /* list items we care about wrt. drag and drop */
  interesting_content = NULL;
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      if (toolbar_content_get_state (content) == NORMAL)
	interesting_content = g_list_prepend (interesting_content, content);
    }
  interesting_content = g_list_reverse (interesting_content);
  
  if (!interesting_content)
    return 0;
  
  orientation = priv->orientation;
  direction = gtk_widget_get_direction (GTK_WIDGET (toolbar));
  
  /* distance to first interesting item */
  best_content = interesting_content->data;
  toolbar_content_get_allocation (best_content, &allocation);
  
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      cursor = x;
      
      if (direction == GTK_TEXT_DIR_LTR)
	pos = allocation.x;
      else
	pos = allocation.x + allocation.width;
    }
  else
    {
      cursor = y;
      pos = allocation.y;
    }
  
  best_content = NULL;
  best_distance = ABS (pos - cursor);
  
  /* distance to far end of each item */
  for (list = interesting_content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      toolbar_content_get_allocation (content, &allocation);
      
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
	{
	  if (direction == GTK_TEXT_DIR_LTR)
	    pos = allocation.x + allocation.width;
	  else
	    pos = allocation.x;
	}
      else
	{
	  pos = allocation.y + allocation.height;
	}
      
      distance = ABS (pos - cursor);
      
      if (distance < best_distance)
	{
	  best_distance = distance;
	  best_content = content;
	}
    }
  
  g_list_free (interesting_content);
  
  if (!best_content)
    return 0;
  else
    return g_list_index (priv->content, best_content) + 1;
}

static void
reset_all_placeholders (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;
  
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      if (toolbar_content_is_placeholder (content))
	toolbar_content_set_disappearing (content, TRUE);
    }
}

static gint
physical_to_logical (GtkToolbar *toolbar,
		     gint        physical)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;
  int logical;
  
  g_assert (physical >= 0);
  
  logical = 0;
  for (list = priv->content; list && physical > 0; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      if (!toolbar_content_is_placeholder (content))
	logical++;
      physical--;
    }
  
  g_assert (physical == 0);
  
  return logical;
}

static gint
logical_to_physical (GtkToolbar *toolbar,
		     gint        logical)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;
  gint physical;
  
  g_assert (logical >= 0);
  
  physical = 0;
  for (list = priv->content; list; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      if (!toolbar_content_is_placeholder (content))
	{
	  if (logical == 0)
	    break;
	  logical--;
	}
      
      physical++;
    }
  
  g_assert (logical == 0);
  
  return physical;
}

/**
 * gtk_toolbar_set_drop_highlight_item:
 * @toolbar: a #GtkToolbar
 * @tool_item: (allow-none): a #GtkToolItem, or %NULL to turn of highlighting
 * @index_: a position on @toolbar
 *
 * Highlights @toolbar to give an idea of what it would look like
 * if @item was added to @toolbar at the position indicated by @index_.
 * If @item is %NULL, highlighting is turned off. In that case @index_ 
 * is ignored.
 *
 * The @tool_item passed to this function must not be part of any widget
 * hierarchy. When an item is set as drop highlight item it can not
 * added to any widget hierarchy or used as highlight item for another
 * toolbar.
 * 
 * Since: 2.4
 **/
void
gtk_toolbar_set_drop_highlight_item (GtkToolbar  *toolbar,
				     GtkToolItem *tool_item,
				     gint         index_)
{
  ToolbarContent *content;
  GtkToolbarPrivate *priv;
  gint n_items;
  GtkRequisition requisition;
  GtkRequisition old_requisition;
  gboolean restart_sliding;
  
  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));
  g_return_if_fail (tool_item == NULL || GTK_IS_TOOL_ITEM (tool_item));

  priv = toolbar->priv;

  if (!tool_item)
    {
      if (priv->highlight_tool_item)
	{
	  gtk_widget_unparent (GTK_WIDGET (priv->highlight_tool_item));
	  g_object_unref (priv->highlight_tool_item);
	  priv->highlight_tool_item = NULL;
	}
      
      reset_all_placeholders (toolbar);
      gtk_toolbar_begin_sliding (toolbar);
      return;
    }
  
  n_items = gtk_toolbar_get_n_items (toolbar);
  if (index_ < 0 || index_ > n_items)
    index_ = n_items;
  
  if (tool_item != priv->highlight_tool_item)
    {
      if (priv->highlight_tool_item)
	g_object_unref (priv->highlight_tool_item);
      
      g_object_ref_sink (tool_item);
      
      priv->highlight_tool_item = tool_item;
      
      gtk_widget_set_parent (GTK_WIDGET (priv->highlight_tool_item),
			     GTK_WIDGET (toolbar));
    }
  
  index_ = logical_to_physical (toolbar, index_);
  
  content = g_list_nth_data (priv->content, index_);
  
  if (index_ > 0)
    {
      ToolbarContent *prev_content;
      
      prev_content = g_list_nth_data (priv->content, index_ - 1);
      
      if (prev_content && toolbar_content_is_placeholder (prev_content))
	content = prev_content;
    }
  
  if (!content || !toolbar_content_is_placeholder (content))
    {
      GtkWidget *placeholder;
      
      placeholder = GTK_WIDGET (gtk_separator_tool_item_new ());

      content = toolbar_content_new_tool_item (toolbar,
					       GTK_TOOL_ITEM (placeholder),
					       TRUE, index_);
      gtk_widget_show (placeholder);
    }
  
  g_assert (content);
  g_assert (toolbar_content_is_placeholder (content));

  gtk_widget_get_preferred_size (GTK_WIDGET (priv->highlight_tool_item),
                                 &requisition, NULL);

  toolbar_content_set_expand (content, gtk_tool_item_get_expand (tool_item));
  
  restart_sliding = FALSE;
  toolbar_content_size_request (content, toolbar, &old_requisition);
  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      requisition.height = -1;
      if (requisition.width != old_requisition.width)
	restart_sliding = TRUE;
    }
  else
    {
      requisition.width = -1;
      if (requisition.height != old_requisition.height)
	restart_sliding = TRUE;
    }

  if (toolbar_content_disappearing (content))
    restart_sliding = TRUE;
  
  reset_all_placeholders (toolbar);
  toolbar_content_set_disappearing (content, FALSE);
  
  toolbar_content_set_size_request (content,
				    requisition.width, requisition.height);
  
  if (restart_sliding)
    gtk_toolbar_begin_sliding (toolbar);
}

static void
gtk_toolbar_get_child_property (GtkContainer *container,
				GtkWidget    *child,
				guint         property_id,
				GValue       *value,
				GParamSpec   *pspec)
{
  GtkToolItem *item = GTK_TOOL_ITEM (child);
  
  switch (property_id)
    {
    case CHILD_PROP_HOMOGENEOUS:
      g_value_set_boolean (value, gtk_tool_item_get_homogeneous (item));
      break;
      
    case CHILD_PROP_EXPAND:
      g_value_set_boolean (value, gtk_tool_item_get_expand (item));
      break;
      
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_toolbar_set_child_property (GtkContainer *container,
				GtkWidget    *child,
				guint         property_id,
				const GValue *value,
				GParamSpec   *pspec)
{
  switch (property_id)
    {
    case CHILD_PROP_HOMOGENEOUS:
      gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (child), g_value_get_boolean (value));
      break;
      
    case CHILD_PROP_EXPAND:
      gtk_tool_item_set_expand (GTK_TOOL_ITEM (child), g_value_get_boolean (value));
      break;
      
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_toolbar_show_all (GtkWidget *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (widget);
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;

  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      
      toolbar_content_show_all (content);
    }
  
  gtk_widget_show (widget);
}

static void
gtk_toolbar_add (GtkContainer *container,
		 GtkWidget    *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (container);

  gtk_toolbar_insert (toolbar, GTK_TOOL_ITEM (widget), -1);
}

static void
gtk_toolbar_remove (GtkContainer *container,
		    GtkWidget    *widget)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (container);
  GtkToolbarPrivate *priv = toolbar->priv;
  ToolbarContent *content_to_remove;
  GList *list;

  content_to_remove = NULL;
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      GtkWidget *child;
      
      child = toolbar_content_get_widget (content);
      if (child && child == widget)
	{
	  content_to_remove = content;
	  break;
	}
    }
  
  g_return_if_fail (content_to_remove != NULL);
  
  toolbar_content_remove (content_to_remove, toolbar);
  toolbar_content_free (content_to_remove);
}

static void
gtk_toolbar_forall (GtkContainer *container,
                    gboolean      include_internals,
                    GtkCallback   callback,
                    gpointer      callback_data)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (container);
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;

  g_return_if_fail (callback != NULL);

  list = priv->content;
  while (list)
    {
      ToolbarContent *content = list->data;
      GList *next = list->next;

      if (include_internals || !toolbar_content_is_placeholder (content))
        {
          GtkWidget *child = toolbar_content_get_widget (content);

          if (child)
            callback (child, callback_data);
        }

      list = next;
    }

  if (include_internals && priv->arrow_button)
    callback (priv->arrow_button, callback_data);
}

static GType
gtk_toolbar_child_type (GtkContainer *container)
{
  return GTK_TYPE_TOOL_ITEM;
}

static void
gtk_toolbar_reconfigured (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;
  
  list = priv->content;
  while (list)
    {
      ToolbarContent *content = list->data;
      GList *next = list->next;
      
      toolbar_content_toolbar_reconfigured (content, toolbar);
      
      list = next;
    }
}

static void
gtk_toolbar_orientation_changed (GtkToolbar    *toolbar,
				 GtkOrientation orientation)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->orientation != orientation)
    {
      priv->orientation = orientation;
      
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
	gtk_arrow_set (GTK_ARROW (priv->arrow), GTK_ARROW_DOWN, GTK_SHADOW_NONE);
      else
	gtk_arrow_set (GTK_ARROW (priv->arrow), GTK_ARROW_RIGHT, GTK_SHADOW_NONE);
      
      gtk_toolbar_reconfigured (toolbar);
      
      gtk_widget_queue_resize (GTK_WIDGET (toolbar));
      g_object_notify (G_OBJECT (toolbar), "orientation");
    }
}

static void
gtk_toolbar_real_style_changed (GtkToolbar     *toolbar,
				GtkToolbarStyle style)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->style != style)
    {
      priv->style = style;

      gtk_toolbar_reconfigured (toolbar);
      
      gtk_widget_queue_resize (GTK_WIDGET (toolbar));
      g_object_notify (G_OBJECT (toolbar), "toolbar-style");
    }
}

static void
menu_position_func (GtkMenu  *menu,
		    gint     *x,
		    gint     *y,
		    gboolean *push_in,
		    gpointer  user_data)
{
  GtkAllocation allocation;
  GtkToolbar *toolbar = GTK_TOOLBAR (user_data);
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkRequisition req;
  GtkRequisition menu_req;
  GdkRectangle monitor;
  gint monitor_num;
  GdkScreen *screen;

  gtk_widget_get_preferred_size (priv->arrow_button,
                                 &req, NULL);
  gtk_widget_get_preferred_size (GTK_WIDGET (menu),
                                 &menu_req, NULL);

  screen = gtk_widget_get_screen (GTK_WIDGET (menu));
  monitor_num = gdk_screen_get_monitor_at_window (screen,
                                                  gtk_widget_get_window (priv->arrow_button));
  if (monitor_num < 0)
    monitor_num = 0;
  gdk_screen_get_monitor_geometry (screen, monitor_num, &monitor);

  gtk_widget_get_allocation (priv->arrow_button, &allocation);

  gdk_window_get_origin (gtk_button_get_event_window (GTK_BUTTON (priv->arrow_button)), x, y);
  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      if (gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_LTR) 
	*x += allocation.width - req.width;
      else 
	*x += req.width - menu_req.width;

      if ((*y + allocation.height + menu_req.height) <= monitor.y + monitor.height)
	*y += allocation.height;
      else if ((*y - menu_req.height) >= monitor.y)
	*y -= menu_req.height;
      else if (monitor.y + monitor.height - (*y + allocation.height) > *y)
	*y += allocation.height;
      else
	*y -= menu_req.height;
    }
  else 
    {
      if (gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_LTR) 
	*x += allocation.width;
      else 
	*x -= menu_req.width;

      if (*y + menu_req.height > monitor.y + monitor.height &&
	  *y + allocation.height - monitor.y > monitor.y + monitor.height - *y)
	*y += allocation.height - menu_req.height;
    }

  *push_in = FALSE;
}

static void
show_menu (GtkToolbar     *toolbar,
	   GdkEventButton *event)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  rebuild_menu (toolbar);

  gtk_widget_show_all (GTK_WIDGET (priv->menu));

  gtk_menu_popup (priv->menu, NULL, NULL,
		  menu_position_func, toolbar,
		  event? event->button : 0,
		  event? event->time : gtk_get_current_event_time());
}

static void
gtk_toolbar_arrow_button_clicked (GtkWidget  *button,
				  GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (priv->arrow_button)) &&
      (!priv->menu || !gtk_widget_get_visible (GTK_WIDGET (priv->menu))))
    {
      /* We only get here when the button is clicked with the keyboard,
       * because mouse button presses result in the menu being shown so
       * that priv->menu would be non-NULL and visible.
       */
      show_menu (toolbar, NULL);
      gtk_menu_shell_select_first (GTK_MENU_SHELL (priv->menu), FALSE);
    }
}

static gboolean
gtk_toolbar_arrow_button_press (GtkWidget      *button,
				GdkEventButton *event,
				GtkToolbar     *toolbar)
{
  show_menu (toolbar, event);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  
  return TRUE;
}

static gboolean
gtk_toolbar_button_press (GtkWidget      *toolbar,
    			  GdkEventButton *event)
{
  GtkWidget *window;

  if (_gtk_button_event_triggers_context_menu (event))
    {
      gboolean return_value;

      g_signal_emit (toolbar, toolbar_signals[POPUP_CONTEXT_MENU], 0,
		     (int)event->x_root, (int)event->y_root, event->button,
		     &return_value);

      return return_value;
    }

  window = gtk_widget_get_toplevel (toolbar);

  if (window)
    {
      gboolean window_drag = FALSE;

      gtk_widget_style_get (toolbar,
                            "window-dragging", &window_drag,
                            NULL);

      if (window_drag)
        {
          gtk_window_begin_move_drag (GTK_WINDOW (window),
                                      event->button,
                                      event->x_root,
                                      event->y_root,
                                      event->time);

          return TRUE;
        }
    }

  return FALSE;
}

static gboolean
gtk_toolbar_popup_menu (GtkWidget *toolbar)
{
  gboolean return_value;
  /* This function is the handler for the "popup menu" keybinding,
   * ie., it is called when the user presses Shift F10
   */
  g_signal_emit (toolbar, toolbar_signals[POPUP_CONTEXT_MENU], 0,
		 -1, -1, -1, &return_value);
  
  return return_value;
}

/**
 * gtk_toolbar_new:
 * 
 * Creates a new toolbar. 
 
 * Return Value: the newly-created toolbar.
 **/
GtkWidget *
gtk_toolbar_new (void)
{
  GtkToolbar *toolbar;
  
  toolbar = g_object_new (GTK_TYPE_TOOLBAR, NULL);
  
  return GTK_WIDGET (toolbar);
}

/**
 * gtk_toolbar_insert:
 * @toolbar: a #GtkToolbar
 * @item: a #GtkToolItem
 * @pos: the position of the new item
 *
 * Insert a #GtkToolItem into the toolbar at position @pos. If @pos is
 * 0 the item is prepended to the start of the toolbar. If @pos is
 * negative, the item is appended to the end of the toolbar.
 *
 * Since: 2.4
 **/
void
gtk_toolbar_insert (GtkToolbar  *toolbar,
		    GtkToolItem *item,
		    gint         pos)
{
  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));
  g_return_if_fail (GTK_IS_TOOL_ITEM (item));
  
  if (pos >= 0)
    pos = logical_to_physical (toolbar, pos);

  toolbar_content_new_tool_item (toolbar, item, FALSE, pos);
}

/**
 * gtk_toolbar_get_item_index:
 * @toolbar: a #GtkToolbar
 * @item: a #GtkToolItem that is a child of @toolbar
 * 
 * Returns the position of @item on the toolbar, starting from 0.
 * It is an error if @item is not a child of the toolbar.
 * 
 * Return value: the position of item on the toolbar.
 * 
 * Since: 2.4
 **/
gint
gtk_toolbar_get_item_index (GtkToolbar  *toolbar,
			    GtkToolItem *item)
{
  GtkToolbarPrivate *priv;
  GList *list;
  int n;
  
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), -1);
  g_return_val_if_fail (GTK_IS_TOOL_ITEM (item), -1);
  g_return_val_if_fail (gtk_widget_get_parent (GTK_WIDGET (item)) == GTK_WIDGET (toolbar), -1);

  priv = toolbar->priv;

  n = 0;
  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;
      GtkWidget *widget;
      
      widget = toolbar_content_get_widget (content);
      
      if (item == GTK_TOOL_ITEM (widget))
	break;
      
      ++n;
    }
  
  return physical_to_logical (toolbar, n);
}

/**
 * gtk_toolbar_set_style:
 * @toolbar: a #GtkToolbar.
 * @style: the new style for @toolbar.
 * 
 * Alters the view of @toolbar to display either icons only, text only, or both.
 **/
void
gtk_toolbar_set_style (GtkToolbar      *toolbar,
		       GtkToolbarStyle  style)
{
  GtkToolbarPrivate *priv;

  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));

  priv = toolbar->priv;

  priv->style_set = TRUE;
  g_signal_emit (toolbar, toolbar_signals[STYLE_CHANGED], 0, style);
}

/**
 * gtk_toolbar_get_style:
 * @toolbar: a #GtkToolbar
 *
 * Retrieves whether the toolbar has text, icons, or both . See
 * gtk_toolbar_set_style().
 
 * Return value: the current style of @toolbar
 **/
GtkToolbarStyle
gtk_toolbar_get_style (GtkToolbar *toolbar)
{
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), DEFAULT_TOOLBAR_STYLE);

  return toolbar->priv->style;
}

/**
 * gtk_toolbar_unset_style:
 * @toolbar: a #GtkToolbar
 * 
 * Unsets a toolbar style set with gtk_toolbar_set_style(), so that
 * user preferences will be used to determine the toolbar style.
 **/
void
gtk_toolbar_unset_style (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv;
  GtkToolbarStyle style;
  
  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));

  priv = toolbar->priv;

  if (priv->style_set)
    {
      GtkSettings *settings = toolbar_get_settings (toolbar);
      
      if (settings)
	g_object_get (settings,
		      "gtk-toolbar-style", &style,
		      NULL);
      else
	style = DEFAULT_TOOLBAR_STYLE;

      if (style != priv->style)
	g_signal_emit (toolbar, toolbar_signals[STYLE_CHANGED], 0, style);

      priv->style_set = FALSE;
    }
}

/**
 * gtk_toolbar_get_n_items:
 * @toolbar: a #GtkToolbar
 * 
 * Returns the number of items on the toolbar.
 * 
 * Return value: the number of items on the toolbar
 * 
 * Since: 2.4
 **/
gint
gtk_toolbar_get_n_items (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv;

  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), -1);

  priv = toolbar->priv;

  return physical_to_logical (toolbar, g_list_length (priv->content));
}

/**
 * gtk_toolbar_get_nth_item:
 * @toolbar: a #GtkToolbar
 * @n: A position on the toolbar
 *
 * Returns the @n<!-- -->'th item on @toolbar, or %NULL if the
 * toolbar does not contain an @n<!-- -->'th item.
 *
 * Return value: (transfer none): The @n<!-- -->'th #GtkToolItem on @toolbar,
 *     or %NULL if there isn't an @n<!-- -->'th item.
 *
 * Since: 2.4
 **/
GtkToolItem *
gtk_toolbar_get_nth_item (GtkToolbar *toolbar,
			  gint        n)
{
  GtkToolbarPrivate *priv;
  ToolbarContent *content;
  gint n_items;
  
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), NULL);

  priv = toolbar->priv;

  n_items = gtk_toolbar_get_n_items (toolbar);
  
  if (n < 0 || n >= n_items)
    return NULL;

  content = g_list_nth_data (priv->content, logical_to_physical (toolbar, n));
  
  g_assert (content);
  g_assert (!toolbar_content_is_placeholder (content));
  
  return GTK_TOOL_ITEM (toolbar_content_get_widget (content));
}

/**
 * gtk_toolbar_get_icon_size:
 * @toolbar: a #GtkToolbar
 *
 * Retrieves the icon size for the toolbar. See gtk_toolbar_set_icon_size().
 *
 * Return value: (type int): the current icon size for the icons on
 * the toolbar.
 **/
GtkIconSize
gtk_toolbar_get_icon_size (GtkToolbar *toolbar)
{
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), DEFAULT_ICON_SIZE);

  return toolbar->priv->icon_size;
}

/**
 * gtk_toolbar_get_relief_style:
 * @toolbar: a #GtkToolbar
 * 
 * Returns the relief style of buttons on @toolbar. See
 * gtk_button_set_relief().
 * 
 * Return value: The relief style of buttons on @toolbar.
 * 
 * Since: 2.4
 **/
GtkReliefStyle
gtk_toolbar_get_relief_style (GtkToolbar *toolbar)
{
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), GTK_RELIEF_NONE);
  
  return get_button_relief (toolbar);
}

/**
 * gtk_toolbar_set_show_arrow:
 * @toolbar: a #GtkToolbar
 * @show_arrow: Whether to show an overflow menu
 * 
 * Sets whether to show an overflow menu when
 * @toolbar doesn't have room for all items on it. If %TRUE,
 * items that there are not room are available through an
 * overflow menu.
 * 
 * Since: 2.4
 **/
void
gtk_toolbar_set_show_arrow (GtkToolbar *toolbar,
			    gboolean    show_arrow)
{
  GtkToolbarPrivate *priv;

  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));

  priv = toolbar->priv;

  show_arrow = show_arrow != FALSE;
  
  if (priv->show_arrow != show_arrow)
    {
      priv->show_arrow = show_arrow;
      
      if (!priv->show_arrow)
	gtk_widget_hide (priv->arrow_button);
      
      gtk_widget_queue_resize (GTK_WIDGET (toolbar));      
      g_object_notify (G_OBJECT (toolbar), "show-arrow");
    }
}

/**
 * gtk_toolbar_get_show_arrow:
 * @toolbar: a #GtkToolbar
 * 
 * Returns whether the toolbar has an overflow menu.
 * See gtk_toolbar_set_show_arrow().
 * 
 * Return value: %TRUE if the toolbar has an overflow menu.
 * 
 * Since: 2.4
 **/
gboolean
gtk_toolbar_get_show_arrow (GtkToolbar *toolbar)
{
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), FALSE);

  return toolbar->priv->show_arrow;
}

/**
 * gtk_toolbar_get_drop_index:
 * @toolbar: a #GtkToolbar
 * @x: x coordinate of a point on the toolbar
 * @y: y coordinate of a point on the toolbar
 *
 * Returns the position corresponding to the indicated point on
 * @toolbar. This is useful when dragging items to the toolbar:
 * this function returns the position a new item should be
 * inserted.
 *
 * @x and @y are in @toolbar coordinates.
 * 
 * Return value: The position corresponding to the point (@x, @y) on the toolbar.
 * 
 * Since: 2.4
 **/
gint
gtk_toolbar_get_drop_index (GtkToolbar *toolbar,
			    gint        x,
			    gint        y)
{
  g_return_val_if_fail (GTK_IS_TOOLBAR (toolbar), -1);
  
  return physical_to_logical (toolbar, find_drop_index (toolbar, x, y));
}

static void
gtk_toolbar_dispose (GObject *object)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (object);
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->arrow_button)
    {
      gtk_widget_unparent (priv->arrow_button);
      priv->arrow_button = NULL;
    }

  if (priv->menu)
    {
      g_signal_handlers_disconnect_by_func (priv->menu,
                                            menu_deactivated, toolbar);
      gtk_widget_destroy (GTK_WIDGET (priv->menu));
      priv->menu = NULL;
    }

 G_OBJECT_CLASS (gtk_toolbar_parent_class)->dispose (object);
}

static void
gtk_toolbar_finalize (GObject *object)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (object);
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->sibling_path != NULL)
    gtk_widget_path_unref (priv->sibling_path);

  g_list_free_full (priv->content, (GDestroyNotify)toolbar_content_free);

  g_timer_destroy (priv->timer);

  if (priv->idle_id)
    g_source_remove (priv->idle_id);

  G_OBJECT_CLASS (gtk_toolbar_parent_class)->finalize (object);
}

/**
 * gtk_toolbar_set_icon_size:
 * @toolbar: A #GtkToolbar
 * @icon_size: (type int): The #GtkIconSize that stock icons in the
 *     toolbar shall have.
 *
 * This function sets the size of stock icons in the toolbar. You
 * can call it both before you add the icons and after they've been
 * added. The size you set will override user preferences for the default
 * icon size.
 * 
 * This should only be used for special-purpose toolbars, normal
 * application toolbars should respect the user preferences for the
 * size of icons.
 **/
void
gtk_toolbar_set_icon_size (GtkToolbar  *toolbar,
			   GtkIconSize  icon_size)
{
  GtkToolbarPrivate *priv;

  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));
  g_return_if_fail (icon_size != GTK_ICON_SIZE_INVALID);

  priv = toolbar->priv;

  if (!priv->icon_size_set)
    {
      priv->icon_size_set = TRUE;
      g_object_notify (G_OBJECT (toolbar), "icon-size-set");
    }

  if (priv->icon_size == icon_size)
    return;

  priv->icon_size = icon_size;
  g_object_notify (G_OBJECT (toolbar), "icon-size");
  
  gtk_toolbar_reconfigured (toolbar);
  
  gtk_widget_queue_resize (GTK_WIDGET (toolbar));
}

/**
 * gtk_toolbar_unset_icon_size:
 * @toolbar: a #GtkToolbar
 * 
 * Unsets toolbar icon size set with gtk_toolbar_set_icon_size(), so that
 * user preferences will be used to determine the icon size.
 **/
void
gtk_toolbar_unset_icon_size (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv;
  GtkIconSize size;

  g_return_if_fail (GTK_IS_TOOLBAR (toolbar));

  priv = toolbar->priv;

  if (priv->icon_size_set)
    {
      GtkSettings *settings = toolbar_get_settings (toolbar);
      
      if (settings)
	{
	  g_object_get (settings,
			"gtk-toolbar-icon-size", &size,
			NULL);
	}
      else
	size = DEFAULT_ICON_SIZE;

      if (size != priv->icon_size)
	{
	  gtk_toolbar_set_icon_size (toolbar, size);
	  g_object_notify (G_OBJECT (toolbar), "icon-size");	  
	}

      priv->icon_size_set = FALSE;
      g_object_notify (G_OBJECT (toolbar), "icon-size-set");      
    }
}

/*
 * ToolbarContent methods
 */
typedef enum {
  UNKNOWN,
  YES,
  NO
} TriState;

struct _ToolbarContent
{
  ItemState      state;

  GtkToolItem   *item;
  GtkAllocation  allocation;
  GtkAllocation  start_allocation;
  GtkAllocation  goal_allocation;
  guint          is_placeholder : 1;
  guint          disappearing : 1;
  guint          has_menu : 2;
};

static void
toolbar_item_visiblity_notify_cb (GObject *obj,
                                  GParamSpec *pspec,
                                  gpointer user_data)
{
  GtkToolbar *toolbar = user_data;

  gtk_toolbar_invalidate_order (toolbar);
}

static ToolbarContent *
toolbar_content_new_tool_item (GtkToolbar  *toolbar,
			       GtkToolItem *item,
			       gboolean     is_placeholder,
			       gint	    pos)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  ToolbarContent *content;

  content = g_slice_new0 (ToolbarContent);
  
  content->state = NOT_ALLOCATED;
  content->item = item;
  content->is_placeholder = is_placeholder;

  priv->content = g_list_insert (priv->content, content, pos);
  
  gtk_widget_set_parent (GTK_WIDGET (item), GTK_WIDGET (toolbar));
  gtk_toolbar_invalidate_order (toolbar);

  g_signal_connect (content->item, "notify::visible",
                    G_CALLBACK (toolbar_item_visiblity_notify_cb), toolbar);

  if (!is_placeholder)
    {
      priv->num_children++;

      gtk_toolbar_stop_sliding (toolbar);
    }

  gtk_widget_queue_resize (GTK_WIDGET (toolbar));
  priv->need_rebuild = TRUE;
  
  return content;
}

static void
toolbar_content_remove (ToolbarContent *content,
			GtkToolbar     *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  gtk_toolbar_invalidate_order (toolbar);
  gtk_widget_unparent (GTK_WIDGET (content->item));

  g_signal_handlers_disconnect_by_func (content->item,
                                        toolbar_item_visiblity_notify_cb,
                                        toolbar);

  priv->content = g_list_remove (priv->content, content);

  if (!toolbar_content_is_placeholder (content))
    priv->num_children--;

  gtk_widget_queue_resize (GTK_WIDGET (toolbar));
  priv->need_rebuild = TRUE;
}

static void
toolbar_content_free (ToolbarContent *content)
{
  g_slice_free (ToolbarContent, content);
}

static gint
calculate_max_homogeneous_pixels (GtkWidget *widget)
{
  PangoContext *context;
  PangoFontMetrics *metrics;
  const PangoFontDescription *font_desc;
  GtkStyleContext *style_context;
  GtkStateFlags state;
  gint char_width;
  
  context = gtk_widget_get_pango_context (widget);
  style_context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);

  font_desc = gtk_style_context_get_font (style_context, state);

  metrics = pango_context_get_metrics (context, font_desc,
				       pango_context_get_language (context));
  char_width = pango_font_metrics_get_approximate_char_width (metrics);
  pango_font_metrics_unref (metrics);
  
  return PANGO_PIXELS (MAX_HOMOGENEOUS_N_CHARS * char_width);
}

static void
toolbar_content_draw (ToolbarContent *content,
                      GtkContainer   *container,
                      cairo_t        *cr)
{
  GtkWidget *widget;

  if (content->is_placeholder)
    return;
  
  widget = GTK_WIDGET (content->item);

  if (widget)
    gtk_container_propagate_draw (container, widget, cr);
}

static gboolean
toolbar_content_visible (ToolbarContent *content,
			 GtkToolbar     *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkToolItem *item;

  item = content->item;

  if (!gtk_widget_get_visible (GTK_WIDGET (item)))
    return FALSE;

  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL &&
      gtk_tool_item_get_visible_horizontal (item))
    return TRUE;

  if (priv->orientation == GTK_ORIENTATION_VERTICAL &&
      gtk_tool_item_get_visible_vertical (item))
    return TRUE;
      
  return FALSE;
}

static void
toolbar_content_size_request (ToolbarContent *content,
			      GtkToolbar     *toolbar,
			      GtkRequisition *requisition)
{
  gtk_widget_get_preferred_size (GTK_WIDGET (content->item),
                                 requisition, NULL);
  if (content->is_placeholder &&
      content->disappearing)
    {
      requisition->width = 0;
      requisition->height = 0;
    }
}

static gboolean
toolbar_content_is_homogeneous (ToolbarContent *content,
				GtkToolbar     *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;
  GtkRequisition requisition;
  gboolean result;
  
  if (priv->max_homogeneous_pixels < 0)
    {
      priv->max_homogeneous_pixels =
	calculate_max_homogeneous_pixels (GTK_WIDGET (toolbar));
    }
  
  toolbar_content_size_request (content, toolbar, &requisition);
  
  if (requisition.width > priv->max_homogeneous_pixels)
    return FALSE;

  result = gtk_tool_item_get_homogeneous (content->item) &&
           !GTK_IS_SEPARATOR_TOOL_ITEM (content->item);

  if (gtk_tool_item_get_is_important (content->item) &&
      priv->style == GTK_TOOLBAR_BOTH_HORIZ &&
      priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      result = FALSE;
    }

  return result;
}

static gboolean
toolbar_content_is_placeholder (ToolbarContent *content)
{
  if (content->is_placeholder)
    return TRUE;
  
  return FALSE;
}

static gboolean
toolbar_content_disappearing (ToolbarContent *content)
{
  if (content->disappearing)
    return TRUE;
  
  return FALSE;
}

static ItemState
toolbar_content_get_state (ToolbarContent *content)
{
  return content->state;
}

static gboolean
toolbar_content_child_visible (ToolbarContent *content)
{
  return gtk_widget_get_child_visible (GTK_WIDGET (content->item));
}

static void
toolbar_content_get_goal_allocation (ToolbarContent *content,
				     GtkAllocation  *allocation)
{
  *allocation = content->goal_allocation;
}

static void
toolbar_content_get_allocation (ToolbarContent *content,
				GtkAllocation  *allocation)
{
  *allocation = content->allocation;
}

static void
toolbar_content_set_start_allocation (ToolbarContent *content,
				      GtkAllocation  *allocation)
{
  content->start_allocation = *allocation;
}

static gboolean
toolbar_content_get_expand (ToolbarContent *content)
{
  if (!content->disappearing &&
      gtk_tool_item_get_expand (content->item))
    return TRUE;

  return FALSE;
}

static void
toolbar_content_set_goal_allocation (ToolbarContent *content,
				     GtkAllocation  *allocation)
{
  content->goal_allocation = *allocation;
}

static void
toolbar_content_set_child_visible (ToolbarContent *content,
				   GtkToolbar     *toolbar,
				   gboolean        visible)
{
  gtk_widget_set_child_visible (GTK_WIDGET (content->item),
                                visible);
}

static void
toolbar_content_get_start_allocation (ToolbarContent *content,
				      GtkAllocation  *start_allocation)
{
  *start_allocation = content->start_allocation;
}

static void
toolbar_content_size_allocate (ToolbarContent *content,
			       GtkAllocation  *allocation)
{
  content->allocation = *allocation;
  gtk_widget_size_allocate (GTK_WIDGET (content->item),
                            allocation);
}

static void
toolbar_content_set_state (ToolbarContent *content,
			   ItemState       state)
{
  content->state = state;
}

static GtkWidget *
toolbar_content_get_widget (ToolbarContent *content)
{
  return GTK_WIDGET (content->item);
}


static void
toolbar_content_set_disappearing (ToolbarContent *content,
				  gboolean        disappearing)
{
  content->disappearing = disappearing;
}

static void
toolbar_content_set_size_request (ToolbarContent *content,
				  gint            width,
				  gint            height)
{
  gtk_widget_set_size_request (GTK_WIDGET (content->item),
                               width, height);
}

static void
toolbar_content_toolbar_reconfigured (ToolbarContent *content,
				      GtkToolbar     *toolbar)
{
  gtk_tool_item_toolbar_reconfigured (content->item);
}

static GtkWidget *
toolbar_content_retrieve_menu_item (ToolbarContent *content)
{
  return gtk_tool_item_retrieve_proxy_menu_item (content->item);
}

static gboolean
toolbar_content_has_proxy_menu_item (ToolbarContent *content)
{
  GtkWidget *menu_item;

  if (content->has_menu == YES)
    return TRUE;
  else if (content->has_menu == NO)
    return FALSE;

  menu_item = toolbar_content_retrieve_menu_item (content);

  content->has_menu = menu_item? YES : NO;

  return menu_item != NULL;
}

static void
toolbar_content_set_unknown_menu_status (ToolbarContent *content)
{
  content->has_menu = UNKNOWN;
}

static gboolean
toolbar_content_is_separator (ToolbarContent *content)
{
  return GTK_IS_SEPARATOR_TOOL_ITEM (content->item);
}

static void
toolbar_content_set_expand (ToolbarContent *content,
                            gboolean        expand)
{
  gtk_tool_item_set_expand (content->item, expand);
}

static void
toolbar_content_show_all (ToolbarContent  *content)
{
  GtkWidget *widget;
  
  widget = toolbar_content_get_widget (content);
  if (widget)
    gtk_widget_show_all (widget);
}

/*
 * Getters
 */
static GtkReliefStyle
get_button_relief (GtkToolbar *toolbar)
{
  GtkReliefStyle button_relief = GTK_RELIEF_NORMAL;

  gtk_widget_style_get (GTK_WIDGET (toolbar),
                        "button-relief", &button_relief,
                        NULL);
  
  return button_relief;
}

static gint
get_internal_padding (GtkToolbar *toolbar)
{
  gint ipadding = 0;
  
  gtk_widget_style_get (GTK_WIDGET (toolbar),
			"internal-padding", &ipadding,
			NULL);
  
  return ipadding;
}

static gint
get_max_child_expand (GtkToolbar *toolbar)
{
  gint mexpand = G_MAXINT;

  gtk_widget_style_get (GTK_WIDGET (toolbar),
                        "max-child-expand", &mexpand,
                        NULL);
  return mexpand;
}

static GtkShadowType
get_shadow_type (GtkToolbar *toolbar)
{
  GtkShadowType shadow_type;
  
  gtk_widget_style_get (GTK_WIDGET (toolbar),
			"shadow-type", &shadow_type,
			NULL);
  
  return shadow_type;
}

/* GTK+ internal methods */

gint
_gtk_toolbar_get_default_space_size (void)
{
  return DEFAULT_SPACE_SIZE;
}

void
_gtk_toolbar_paint_space_line (GtkWidget           *widget,
			       GtkToolbar          *toolbar,
                               cairo_t             *cr)
{
  GtkOrientation orientation;
  GtkStyleContext *context;
  GtkStateFlags state;
  GtkBorder padding;
  gint width, height;
  const gdouble start_fraction = (SPACE_LINE_START / SPACE_LINE_DIVISION);
  const gdouble end_fraction = (SPACE_LINE_END / SPACE_LINE_DIVISION);

  g_return_if_fail (GTK_IS_WIDGET (widget));

  orientation = toolbar ? toolbar->priv->orientation : GTK_ORIENTATION_HORIZONTAL;

  context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);
  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);
  gtk_style_context_get_padding (context, state, &padding);

  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      gboolean wide_separators;
      gint     separator_width;

      gtk_widget_style_get (widget,
                            "wide-separators", &wide_separators,
                            "separator-width", &separator_width,
                            NULL);

      if (wide_separators)
        gtk_render_frame (context, cr,
                          (width - separator_width) / 2,
                          height * start_fraction,
                          separator_width,
                          height * (end_fraction - start_fraction));
      else
        gtk_render_line (context, cr,
                         (width - padding.left) / 2,
                         height * start_fraction,
                         (width - padding.left) / 2,
                         height * end_fraction);
    }
  else
    {
      gboolean wide_separators;
      gint     separator_height;

      gtk_widget_style_get (widget,
                            "wide-separators",  &wide_separators,
                            "separator-height", &separator_height,
                            NULL);

      if (wide_separators)
        gtk_render_frame (context, cr,
                          width * start_fraction,
                          (height - separator_height) / 2,
                          width * (end_fraction - start_fraction),
                          separator_height);
      else
        gtk_render_line (context, cr,
                         width * start_fraction,
                         (height - padding.top) / 2,
                         width * end_fraction,
                         (height - padding.top) / 2);
    }
}

gchar *
_gtk_toolbar_elide_underscores (const gchar *original)
{
  gchar *q, *result;
  const gchar *p, *end;
  gsize len;
  gboolean last_underscore;
  
  if (!original)
    return NULL;

  len = strlen (original);
  q = result = g_malloc (len + 1);
  last_underscore = FALSE;
  
  end = original + len;
  for (p = original; p < end; p++)
    {
      if (!last_underscore && *p == '_')
	last_underscore = TRUE;
      else
	{
	  last_underscore = FALSE;
	  if (original + 2 <= p && p + 1 <= end && 
              p[-2] == '(' && p[-1] == '_' && p[0] != '_' && p[1] == ')')
	    {
	      q--;
	      *q = '\0';
	      p++;
	    }
	  else
	    *q++ = *p;
	}
    }

  if (last_underscore)
    *q++ = '_';
  
  *q = '\0';
  
  return result;
}

static GtkIconSize
toolbar_get_icon_size (GtkToolShell *shell)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (shell);
  GtkToolbarPrivate *priv = toolbar->priv;

  return priv->icon_size;
}

static GtkOrientation
toolbar_get_orientation (GtkToolShell *shell)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (shell);
  GtkToolbarPrivate *priv = toolbar->priv;

  return priv->orientation;
}

static GtkToolbarStyle
toolbar_get_style (GtkToolShell *shell)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (shell);
  GtkToolbarPrivate *priv = toolbar->priv;

  return priv->style;
}

static GtkReliefStyle
toolbar_get_relief_style (GtkToolShell *shell)
{
  return get_button_relief (GTK_TOOLBAR (shell));
}

static void
toolbar_rebuild_menu (GtkToolShell *shell)
{
  GtkToolbar *toolbar = GTK_TOOLBAR (shell);
  GtkToolbarPrivate *priv = toolbar->priv;
  GList *list;

  priv->need_rebuild = TRUE;

  for (list = priv->content; list != NULL; list = list->next)
    {
      ToolbarContent *content = list->data;

      toolbar_content_set_unknown_menu_status (content);
    }
  
  gtk_widget_queue_resize (GTK_WIDGET (shell));
}

typedef struct _CountingData CountingData;
struct _CountingData {
  GtkWidget *widget;
  gboolean found;
  guint before;
  guint after;
};

static void
count_widget_position (GtkWidget *widget,
                       gpointer   data)
{
  CountingData *count = data;

  if (!gtk_widget_get_visible (widget))
    return;

  if (count->widget == widget)
    count->found = TRUE;
  else if (count->found)
    count->after++;
  else
    count->before++;
}

static guint
gtk_toolbar_get_visible_position (GtkToolbar *toolbar,
                                  GtkWidget  *child)
{
  CountingData count = { child, FALSE, 0, 0 };

  if (child == (GtkWidget*)toolbar->priv->highlight_tool_item)
    return 0;

  /* foreach iterates in visible order */
  gtk_container_forall (GTK_CONTAINER (toolbar),
                        count_widget_position,
                        &count);

  g_assert (count.found);

  if (toolbar->priv->orientation == GTK_ORIENTATION_HORIZONTAL &&
      gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_RTL)
    return count.after;

  return count.before;
}

static void
add_widget_to_path (gpointer data,
                    gpointer user_data)
{
  GtkWidget *widget = data;
  GtkWidgetPath *path = user_data;

  if (gtk_widget_get_visible (widget))
    gtk_widget_path_append_for_widget (path, widget);
}

static GtkWidgetPath *
gtk_toolbar_get_path_for_child (GtkContainer *container,
                                GtkWidget    *child)
{
  GtkWidgetPath *path;
  GtkToolbar *toolbar;
  GtkToolbarPrivate *priv;
  gint vis_index;

  toolbar = GTK_TOOLBAR (container);
  priv = toolbar->priv;

  if (priv->sibling_path == NULL)
    {
      GList *children;

      /* build a path for all the visible children;
       * get_children works in visible order
       */
      priv->sibling_path = gtk_widget_path_new ();
      children = gtk_container_get_children (container);

      if (priv->orientation == GTK_ORIENTATION_HORIZONTAL &&
          gtk_widget_get_direction (GTK_WIDGET (toolbar)) == GTK_TEXT_DIR_RTL)
        children = g_list_reverse (children);

      g_list_foreach (children, add_widget_to_path, priv->sibling_path);
      g_list_free (children);
    }

  path = gtk_widget_path_copy (gtk_widget_get_path (GTK_WIDGET (container)));
  if (gtk_widget_get_visible (child))
    {
      vis_index = gtk_toolbar_get_visible_position (toolbar, child);

      if (vis_index < gtk_widget_path_length (priv->sibling_path))
        gtk_widget_path_append_with_siblings (path,
                                              priv->sibling_path,
                                              vis_index);
      else
        gtk_widget_path_append_for_widget (path, child);
    }
  else
    gtk_widget_path_append_for_widget (path, child);

  return path;
}

static void
gtk_toolbar_invalidate_order (GtkToolbar *toolbar)
{
  GtkToolbarPrivate *priv = toolbar->priv;

  if (priv->sibling_path != NULL)
    {
      gtk_widget_path_unref (priv->sibling_path);
      priv->sibling_path = NULL;

      gtk_container_foreach (GTK_CONTAINER (toolbar),
                             (GtkCallback) gtk_widget_reset_style,
                             NULL);
    }
}

