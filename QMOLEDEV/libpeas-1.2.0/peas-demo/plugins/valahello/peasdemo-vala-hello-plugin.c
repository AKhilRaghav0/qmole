/* peasdemo-vala-hello-plugin.c generated by valac 0.13.3.19-0d118, the Vala compiler
 * generated from peasdemo-vala-hello-plugin.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <libpeas-gtk/peas-gtk.h>
#include <girepository.h>
#include <gmodule.h>
#include <libpeas/peas.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>


#define PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN (peas_demo_vala_hello_plugin_get_type ())
#define PEAS_DEMO_VALA_HELLO_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN, PeasDemoValaHelloPlugin))
#define PEAS_DEMO_VALA_HELLO_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN, PeasDemoValaHelloPluginClass))
#define PEAS_DEMO_IS_VALA_HELLO_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN))
#define PEAS_DEMO_IS_VALA_HELLO_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN))
#define PEAS_DEMO_VALA_HELLO_PLUGIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN, PeasDemoValaHelloPluginClass))

typedef struct _PeasDemoValaHelloPlugin PeasDemoValaHelloPlugin;
typedef struct _PeasDemoValaHelloPluginClass PeasDemoValaHelloPluginClass;
typedef struct _PeasDemoValaHelloPluginPrivate PeasDemoValaHelloPluginPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG (peas_demo_vala_plugin_config_get_type ())
#define PEAS_DEMO_VALA_PLUGIN_CONFIG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG, PeasDemoValaPluginConfig))
#define PEAS_DEMO_VALA_PLUGIN_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG, PeasDemoValaPluginConfigClass))
#define PEAS_DEMO_IS_VALA_PLUGIN_CONFIG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG))
#define PEAS_DEMO_IS_VALA_PLUGIN_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG))
#define PEAS_DEMO_VALA_PLUGIN_CONFIG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG, PeasDemoValaPluginConfigClass))

typedef struct _PeasDemoValaPluginConfig PeasDemoValaPluginConfig;
typedef struct _PeasDemoValaPluginConfigClass PeasDemoValaPluginConfigClass;
typedef struct _PeasDemoValaPluginConfigPrivate PeasDemoValaPluginConfigPrivate;
#define _g_free0(var) (var = (g_free (var), NULL))

struct _PeasDemoValaHelloPlugin {
	PeasExtensionBase parent_instance;
	PeasDemoValaHelloPluginPrivate * priv;
};

struct _PeasDemoValaHelloPluginClass {
	PeasExtensionBaseClass parent_class;
};

struct _PeasDemoValaHelloPluginPrivate {
	GtkWidget* label;
	GObject* _object;
};

struct _PeasDemoValaPluginConfig {
	PeasExtensionBase parent_instance;
	PeasDemoValaPluginConfigPrivate * priv;
};

struct _PeasDemoValaPluginConfigClass {
	PeasExtensionBaseClass parent_class;
};


static gpointer peas_demo_vala_hello_plugin_parent_class = NULL;
static PeasActivatableInterface* peas_demo_vala_hello_plugin_peas_activatable_parent_iface = NULL;
static GType peas_demo_vala_hello_plugin_type_id = 0;
static gpointer peas_demo_vala_plugin_config_parent_class = NULL;
static PeasGtkConfigurableInterface* peas_demo_vala_plugin_config_peas_gtk_configurable_parent_iface = NULL;
static GType peas_demo_vala_plugin_config_type_id = 0;

GType peas_demo_vala_hello_plugin_get_type (void) G_GNUC_CONST;
GType peas_demo_vala_hello_plugin_register_type (GTypeModule * module);
#define PEAS_DEMO_VALA_HELLO_PLUGIN_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN, PeasDemoValaHelloPluginPrivate))
enum  {
	PEAS_DEMO_VALA_HELLO_PLUGIN_DUMMY_PROPERTY,
	PEAS_DEMO_VALA_HELLO_PLUGIN_OBJECT
};
PeasDemoValaHelloPlugin* peas_demo_vala_hello_plugin_new (void);
PeasDemoValaHelloPlugin* peas_demo_vala_hello_plugin_construct (GType object_type);
static void peas_demo_vala_hello_plugin_real_activate (PeasActivatable* base);
GObject* peas_demo_vala_hello_plugin_get_object (PeasDemoValaHelloPlugin* self);
static void peas_demo_vala_hello_plugin_real_deactivate (PeasActivatable* base);
static void peas_demo_vala_hello_plugin_real_update_state (PeasActivatable* base);
static void peas_demo_vala_hello_plugin_set_object (PeasDemoValaHelloPlugin* self, GObject* value);
static void peas_demo_vala_hello_plugin_finalize (GObject* obj);
static void _vala_peas_demo_vala_hello_plugin_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);
static void _vala_peas_demo_vala_hello_plugin_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec);
GType peas_demo_vala_plugin_config_get_type (void) G_GNUC_CONST;
GType peas_demo_vala_plugin_config_register_type (GTypeModule * module);
enum  {
	PEAS_DEMO_VALA_PLUGIN_CONFIG_DUMMY_PROPERTY
};
PeasDemoValaPluginConfig* peas_demo_vala_plugin_config_new (void);
PeasDemoValaPluginConfig* peas_demo_vala_plugin_config_construct (GType object_type);
static GtkWidget* peas_demo_vala_plugin_config_real_create_configure_widget (PeasGtkConfigurable* base);
void peas_register_types (GTypeModule* module);


PeasDemoValaHelloPlugin* peas_demo_vala_hello_plugin_construct (GType object_type) {
	PeasDemoValaHelloPlugin * self = NULL;
	self = (PeasDemoValaHelloPlugin*) g_object_new (object_type, NULL);
	return self;
}


PeasDemoValaHelloPlugin* peas_demo_vala_hello_plugin_new (void) {
	return peas_demo_vala_hello_plugin_construct (PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


static void peas_demo_vala_hello_plugin_real_activate (PeasActivatable* base) {
	PeasDemoValaHelloPlugin * self;
	GObject* _tmp0_;
	GtkWindow* _tmp1_;
	GtkWindow* window;
	GtkLabel* _tmp2_;
	GtkWidget* _tmp3_;
	GtkWidget* _tmp4_ = NULL;
	GtkBox* _tmp5_;
	GtkBox* box;
	GtkWidget* _tmp6_;
	GtkWidget* _tmp7_;
	self = (PeasDemoValaHelloPlugin*) base;
	_tmp0_ = self->priv->_object;
	_tmp1_ = _g_object_ref0 (GTK_IS_WINDOW (_tmp0_) ? ((GtkWindow*) _tmp0_) : NULL);
	window = _tmp1_;
	_tmp2_ = (GtkLabel*) gtk_label_new ("Hello World from Vala!");
	_tmp3_ = (GtkWidget*) g_object_ref_sink (_tmp2_);
	_g_object_unref0 (self->priv->label);
	self->priv->label = _tmp3_;
	_tmp4_ = gtk_bin_get_child ((GtkBin*) window);
	_tmp5_ = _g_object_ref0 (GTK_IS_BOX (_tmp4_) ? ((GtkBox*) _tmp4_) : NULL);
	box = _tmp5_;
	_tmp6_ = self->priv->label;
	gtk_box_pack_start (box, _tmp6_, TRUE, TRUE, (guint) 0);
	_tmp7_ = self->priv->label;
	gtk_widget_show (_tmp7_);
	_g_object_unref0 (box);
	_g_object_unref0 (window);
}


static void peas_demo_vala_hello_plugin_real_deactivate (PeasActivatable* base) {
	PeasDemoValaHelloPlugin * self;
	GObject* _tmp0_;
	GtkWindow* _tmp1_;
	GtkWindow* window;
	GtkWidget* _tmp2_ = NULL;
	GtkBox* _tmp3_;
	GtkBox* box;
	GtkWidget* _tmp4_;
	self = (PeasDemoValaHelloPlugin*) base;
	_tmp0_ = self->priv->_object;
	_tmp1_ = _g_object_ref0 (GTK_IS_WINDOW (_tmp0_) ? ((GtkWindow*) _tmp0_) : NULL);
	window = _tmp1_;
	_tmp2_ = gtk_bin_get_child ((GtkBin*) window);
	_tmp3_ = _g_object_ref0 (GTK_IS_BOX (_tmp2_) ? ((GtkBox*) _tmp2_) : NULL);
	box = _tmp3_;
	_tmp4_ = self->priv->label;
	gtk_container_remove ((GtkContainer*) box, _tmp4_);
	_g_object_unref0 (box);
	_g_object_unref0 (window);
}


static void peas_demo_vala_hello_plugin_real_update_state (PeasActivatable* base) {
	PeasDemoValaHelloPlugin * self;
	self = (PeasDemoValaHelloPlugin*) base;
}


GObject* peas_demo_vala_hello_plugin_get_object (PeasDemoValaHelloPlugin* self) {
	GObject* result;
	GObject* _tmp0_;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->_object;
	result = _tmp0_;
	return result;
}


static void peas_demo_vala_hello_plugin_set_object (PeasDemoValaHelloPlugin* self, GObject* value) {
	GObject* _tmp0_;
	GObject* _tmp1_;
	g_return_if_fail (self != NULL);
	_tmp0_ = value;
	_tmp1_ = _g_object_ref0 (_tmp0_);
	_g_object_unref0 (self->priv->_object);
	self->priv->_object = _tmp1_;
	g_object_notify ((GObject *) self, "object");
}


static void peas_demo_vala_hello_plugin_class_init (PeasDemoValaHelloPluginClass * klass) {
	peas_demo_vala_hello_plugin_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (PeasDemoValaHelloPluginPrivate));
	G_OBJECT_CLASS (klass)->get_property = _vala_peas_demo_vala_hello_plugin_get_property;
	G_OBJECT_CLASS (klass)->set_property = _vala_peas_demo_vala_hello_plugin_set_property;
	G_OBJECT_CLASS (klass)->finalize = peas_demo_vala_hello_plugin_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), PEAS_DEMO_VALA_HELLO_PLUGIN_OBJECT, g_param_spec_object ("object", "object", "object", G_TYPE_OBJECT, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}


static void peas_demo_vala_hello_plugin_peas_activatable_interface_init (PeasActivatableInterface * iface) {
	peas_demo_vala_hello_plugin_peas_activatable_parent_iface = g_type_interface_peek_parent (iface);
	iface->activate = (void (*)(PeasActivatable*)) peas_demo_vala_hello_plugin_real_activate;
	iface->deactivate = (void (*)(PeasActivatable*)) peas_demo_vala_hello_plugin_real_deactivate;
	iface->update_state = (void (*)(PeasActivatable*)) peas_demo_vala_hello_plugin_real_update_state;
}


static void peas_demo_vala_hello_plugin_instance_init (PeasDemoValaHelloPlugin * self) {
	self->priv = PEAS_DEMO_VALA_HELLO_PLUGIN_GET_PRIVATE (self);
}


static void peas_demo_vala_hello_plugin_finalize (GObject* obj) {
	PeasDemoValaHelloPlugin * self;
	self = PEAS_DEMO_VALA_HELLO_PLUGIN (obj);
	_g_object_unref0 (self->priv->label);
	_g_object_unref0 (self->priv->_object);
	G_OBJECT_CLASS (peas_demo_vala_hello_plugin_parent_class)->finalize (obj);
}


GType peas_demo_vala_hello_plugin_get_type (void) {
	return peas_demo_vala_hello_plugin_type_id;
}


GType peas_demo_vala_hello_plugin_register_type (GTypeModule * module) {
	static const GTypeInfo g_define_type_info = { sizeof (PeasDemoValaHelloPluginClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) peas_demo_vala_hello_plugin_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (PeasDemoValaHelloPlugin), 0, (GInstanceInitFunc) peas_demo_vala_hello_plugin_instance_init, NULL };
	static const GInterfaceInfo peas_activatable_info = { (GInterfaceInitFunc) peas_demo_vala_hello_plugin_peas_activatable_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
	peas_demo_vala_hello_plugin_type_id = g_type_module_register_type (module, peas_extension_base_get_type (), "PeasDemoValaHelloPlugin", &g_define_type_info, 0);
	g_type_module_add_interface (module, peas_demo_vala_hello_plugin_type_id, peas_activatable_get_type (), &peas_activatable_info);
	return peas_demo_vala_hello_plugin_type_id;
}


static void _vala_peas_demo_vala_hello_plugin_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	PeasDemoValaHelloPlugin * self;
	self = PEAS_DEMO_VALA_HELLO_PLUGIN (object);
	switch (property_id) {
		case PEAS_DEMO_VALA_HELLO_PLUGIN_OBJECT:
		g_value_set_object (value, peas_demo_vala_hello_plugin_get_object (self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


static void _vala_peas_demo_vala_hello_plugin_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec) {
	PeasDemoValaHelloPlugin * self;
	self = PEAS_DEMO_VALA_HELLO_PLUGIN (object);
	switch (property_id) {
		case PEAS_DEMO_VALA_HELLO_PLUGIN_OBJECT:
		peas_demo_vala_hello_plugin_set_object (self, g_value_get_object (value));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


PeasDemoValaPluginConfig* peas_demo_vala_plugin_config_construct (GType object_type) {
	PeasDemoValaPluginConfig * self = NULL;
	self = (PeasDemoValaPluginConfig*) g_object_new (object_type, NULL);
	return self;
}


PeasDemoValaPluginConfig* peas_demo_vala_plugin_config_new (void) {
	return peas_demo_vala_plugin_config_construct (PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG);
}


static GtkWidget* peas_demo_vala_plugin_config_real_create_configure_widget (PeasGtkConfigurable* base) {
	PeasDemoValaPluginConfig * self;
	GtkWidget* result = NULL;
	gchar* _tmp0_;
	gchar* text;
	GtkLabel* _tmp1_;
	GtkWidget* _tmp2_;
	self = (PeasDemoValaPluginConfig*) base;
	_tmp0_ = g_strdup ("This is a configuration dialog for the ValaHello plugin.");
	text = _tmp0_;
	_tmp1_ = (GtkLabel*) gtk_label_new (text);
	_tmp2_ = (GtkWidget*) g_object_ref_sink (_tmp1_);
	result = _tmp2_;
	_g_free0 (text);
	return result;
}


static void peas_demo_vala_plugin_config_class_init (PeasDemoValaPluginConfigClass * klass) {
	peas_demo_vala_plugin_config_parent_class = g_type_class_peek_parent (klass);
}


static void peas_demo_vala_plugin_config_peas_gtk_configurable_interface_init (PeasGtkConfigurableInterface * iface) {
	peas_demo_vala_plugin_config_peas_gtk_configurable_parent_iface = g_type_interface_peek_parent (iface);
	iface->create_configure_widget = (GtkWidget* (*)(PeasGtkConfigurable*)) peas_demo_vala_plugin_config_real_create_configure_widget;
}


static void peas_demo_vala_plugin_config_instance_init (PeasDemoValaPluginConfig * self) {
}


GType peas_demo_vala_plugin_config_get_type (void) {
	return peas_demo_vala_plugin_config_type_id;
}


GType peas_demo_vala_plugin_config_register_type (GTypeModule * module) {
	static const GTypeInfo g_define_type_info = { sizeof (PeasDemoValaPluginConfigClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) peas_demo_vala_plugin_config_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (PeasDemoValaPluginConfig), 0, (GInstanceInitFunc) peas_demo_vala_plugin_config_instance_init, NULL };
	static const GInterfaceInfo peas_gtk_configurable_info = { (GInterfaceInitFunc) peas_demo_vala_plugin_config_peas_gtk_configurable_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
	peas_demo_vala_plugin_config_type_id = g_type_module_register_type (module, peas_extension_base_get_type (), "PeasDemoValaPluginConfig", &g_define_type_info, 0);
	g_type_module_add_interface (module, peas_demo_vala_plugin_config_type_id, peas_gtk_configurable_get_type (), &peas_gtk_configurable_info);
	return peas_demo_vala_plugin_config_type_id;
}


void peas_register_types (GTypeModule* module) {
	GTypeModule* _tmp0_;
	PeasObjectModule* _tmp1_;
	PeasObjectModule* objmodule;
	g_return_if_fail (module != NULL);
	peas_demo_vala_hello_plugin_register_type (module);
	peas_demo_vala_plugin_config_register_type (module);
	_tmp0_ = module;
	_tmp1_ = _g_object_ref0 (PEAS_IS_OBJECT_MODULE (_tmp0_) ? ((PeasObjectModule*) _tmp0_) : NULL);
	objmodule = _tmp1_;
	peas_object_module_register_extension_type (objmodule, peas_activatable_get_type (), PEAS_DEMO_TYPE_VALA_HELLO_PLUGIN);
	peas_object_module_register_extension_type (objmodule, peas_gtk_configurable_get_type (), PEAS_DEMO_TYPE_VALA_PLUGIN_CONFIG);
	_g_object_unref0 (objmodule);
}



