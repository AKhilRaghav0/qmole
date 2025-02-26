# -*- Mode: Python -*-

# GDBus - GLib D-Bus Library
#
# Copyright (C) 2008-2011 Red Hat, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place, Suite 330,
# Boston, MA 02111-1307, USA.
#
# Author: David Zeuthen <davidz@redhat.com>

import utils

class Annotation:
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.annotations = []

class Arg:
    def __init__(self, name, signature):
        self.name = name
        self.signature = signature
        self.annotations = []
        self.doc_string = ''
        self.since = ''

    def post_process(self, interface_prefix, c_namespace, arg_number):
        if len(self.doc_string) == 0:
            self.doc_string = utils.lookup_docs(self.annotations)
        if len(self.since) == 0:
            self.since = utils.lookup_since(self.annotations)

        if self.name == None:
            self.name = 'unnamed_arg%d'%arg_number
        # default to GVariant
        self.ctype_in_g  = 'GVariant *'
        self.ctype_in  = 'GVariant *'
        self.ctype_out = 'GVariant **'
        self.gtype = 'G_TYPE_VARIANT'
        self.free_func = 'g_variant_unref'
        self.format_in = '@' + self.signature
        self.format_out = '@' + self.signature
        if not utils.lookup_annotation(self.annotations, 'org.gtk.GDBus.C.ForceGVariant'):
            if self.signature == 'b':
                self.ctype_in_g  = 'gboolean '
                self.ctype_in  = 'gboolean '
                self.ctype_out = 'gboolean *'
                self.gtype = 'G_TYPE_BOOLEAN'
                self.free_func = None
                self.format_in = 'b'
                self.format_out = 'b'
            elif self.signature == 'y':
                self.ctype_in_g  = 'guchar '
                self.ctype_in  = 'guchar '
                self.ctype_out = 'guchar *'
                self.gtype = 'G_TYPE_UCHAR'
                self.free_func = None
                self.format_in = 'y'
                self.format_out = 'y'
            elif self.signature == 'n':
                self.ctype_in_g  = 'gint '
                self.ctype_in  = 'gint16 '
                self.ctype_out = 'gint16 *'
                self.gtype = 'G_TYPE_INT'
                self.free_func = None
                self.format_in = 'n'
                self.format_out = 'n'
            elif self.signature == 'q':
                self.ctype_in_g  = 'guint '
                self.ctype_in  = 'guint16 '
                self.ctype_out = 'guint16 *'
                self.gtype = 'G_TYPE_UINT'
                self.free_func = None
                self.format_in = 'q'
                self.format_out = 'q'
            elif self.signature == 'i':
                self.ctype_in_g  = 'gint '
                self.ctype_in  = 'gint '
                self.ctype_out = 'gint *'
                self.gtype = 'G_TYPE_INT'
                self.free_func = None
                self.format_in = 'i'
                self.format_out = 'i'
            elif self.signature == 'u':
                self.ctype_in_g  = 'guint '
                self.ctype_in  = 'guint '
                self.ctype_out = 'guint *'
                self.gtype = 'G_TYPE_UINT'
                self.free_func = None
                self.format_in = 'u'
                self.format_out = 'u'
            elif self.signature == 'x':
                self.ctype_in_g  = 'gint64 '
                self.ctype_in  = 'gint64 '
                self.ctype_out = 'gint64 *'
                self.gtype = 'G_TYPE_INT64'
                self.free_func = None
                self.format_in = 'x'
                self.format_out = 'x'
            elif self.signature == 't':
                self.ctype_in_g  = 'guint64 '
                self.ctype_in  = 'guint64 '
                self.ctype_out = 'guint64 *'
                self.gtype = 'G_TYPE_UINT64'
                self.free_func = None
                self.format_in = 't'
                self.format_out = 't'
            elif self.signature == 'd':
                self.ctype_in_g  = 'gdouble '
                self.ctype_in  = 'gdouble '
                self.ctype_out = 'gdouble *'
                self.gtype = 'G_TYPE_DOUBLE'
                self.free_func = None
                self.format_in = 'd'
                self.format_out = 'd'
            elif self.signature == 's':
                self.ctype_in_g  = 'const gchar *'
                self.ctype_in  = 'const gchar *'
                self.ctype_out = 'gchar **'
                self.gtype = 'G_TYPE_STRING'
                self.free_func = 'g_free'
                self.format_in = 's'
                self.format_out = 's'
            elif self.signature == 'o':
                self.ctype_in_g  = 'const gchar *'
                self.ctype_in  = 'const gchar *'
                self.ctype_out = 'gchar **'
                self.gtype = 'G_TYPE_STRING'
                self.free_func = 'g_free'
                self.format_in = 'o'
                self.format_out = 'o'
            elif self.signature == 'g':
                self.ctype_in_g  = 'const gchar *'
                self.ctype_in  = 'const gchar *'
                self.ctype_out = 'gchar **'
                self.gtype = 'G_TYPE_STRING'
                self.free_func = 'g_free'
                self.format_in = 'g'
                self.format_out = 'g'
            elif self.signature == 'ay':
                self.ctype_in_g  = 'const gchar *'
                self.ctype_in  = 'const gchar *'
                self.ctype_out = 'gchar **'
                self.gtype = 'G_TYPE_STRING'
                self.free_func = 'g_free'
                self.format_in = '^ay'
                self.format_out = '^ay'
            elif self.signature == 'as':
                self.ctype_in_g  = 'const gchar *const *'
                self.ctype_in  = 'const gchar *const *'
                self.ctype_out = 'gchar ***'
                self.gtype = 'G_TYPE_STRV'
                self.free_func = 'g_strfreev'
                self.format_in = '^as'
                self.format_out = '^as'
            elif self.signature == 'ao':
                self.ctype_in_g  = 'const gchar *const *'
                self.ctype_in  = 'const gchar *const *'
                self.ctype_out = 'gchar ***'
                self.gtype = 'G_TYPE_STRV'
                self.free_func = 'g_strfreev'
                self.format_in = '^ao'
                self.format_out = '^ao'
            elif self.signature == 'aay':
                self.ctype_in_g  = 'const gchar *const *'
                self.ctype_in  = 'const gchar *const *'
                self.ctype_out = 'gchar ***'
                self.gtype = 'G_TYPE_STRV'
                self.free_func = 'g_strfreev'
                self.format_in = '^aay'
                self.format_out = '^aay'

class Method:
    def __init__(self, name):
        self.name = name
        self.in_args = []
        self.out_args = []
        self.annotations = []
        self.doc_string = ''
        self.since = ''
        self.deprecated = False

    def post_process(self, interface_prefix, c_namespace):
        if len(self.doc_string) == 0:
            self.doc_string = utils.lookup_docs(self.annotations)
        if len(self.since) == 0:
            self.since = utils.lookup_since(self.annotations)

        name = self.name
        overridden_name = utils.lookup_annotation(self.annotations, 'org.gtk.GDBus.C.Name')
        if utils.is_ugly_case(overridden_name):
            self.name_lower = overridden_name.lower()
        else:
            if overridden_name:
                name = overridden_name
            self.name_lower = utils.camel_case_to_uscore(name).lower().replace('-', '_')
        self.name_hyphen = self.name_lower.replace('_', '-')

        arg_count = 0
        for a in self.in_args:
            a.post_process(interface_prefix, c_namespace, arg_count)
            arg_count += 1

        for a in self.out_args:
            a.post_process(interface_prefix, c_namespace, arg_count)
            arg_count += 1

        if utils.lookup_annotation(self.annotations, 'org.freedesktop.DBus.Deprecated') == 'true':
            self.deprecated = True

class Signal:
    def __init__(self, name):
        self.name = name
        self.args = []
        self.annotations = []
        self.doc_string = ''
        self.since = ''
        self.deprecated = False

    def post_process(self, interface_prefix, c_namespace):
        if len(self.doc_string) == 0:
            self.doc_string = utils.lookup_docs(self.annotations)
        if len(self.since) == 0:
            self.since = utils.lookup_since(self.annotations)

        name = self.name
        overridden_name = utils.lookup_annotation(self.annotations, 'org.gtk.GDBus.C.Name')
        if utils.is_ugly_case(overridden_name):
            self.name_lower = overridden_name.lower()
        else:
            if overridden_name:
                name = overridden_name
            self.name_lower = utils.camel_case_to_uscore(name).lower().replace('-', '_')
        self.name_hyphen = self.name_lower.replace('_', '-')

        arg_count = 0
        for a in self.args:
            a.post_process(interface_prefix, c_namespace, arg_count)
            arg_count += 1

        if utils.lookup_annotation(self.annotations, 'org.freedesktop.DBus.Deprecated') == 'true':
            self.deprecated = True

class Property:
    def __init__(self, name, signature, access):
        self.name = name
        self.signature = signature
        self.access = access
        self.annotations = []
        self.arg = Arg('value', self.signature)
        self.arg.annotations = self.annotations
        self.readable = False
        self.writable = False
        if self.access == 'readwrite':
            self.readable = True
            self.writable = True
        elif self.access == 'read':
            self.readable = True
        elif self.access == 'write':
            self.writable = True
        else:
            raise RuntimeError('Invalid access type %s'%self.access)
        self.doc_string = ''
        self.since = ''
        self.deprecated = False

    def post_process(self, interface_prefix, c_namespace):
        if len(self.doc_string) == 0:
            self.doc_string = utils.lookup_docs(self.annotations)
        if len(self.since) == 0:
            self.since = utils.lookup_since(self.annotations)

        name = self.name
        overridden_name = utils.lookup_annotation(self.annotations, 'org.gtk.GDBus.C.Name')
        if utils.is_ugly_case(overridden_name):
            self.name_lower = overridden_name.lower()
        else:
            if overridden_name:
                name = overridden_name
            self.name_lower = utils.camel_case_to_uscore(name).lower().replace('-', '_')
        if self.name_lower == 'type':
            self.name_lower = 'type_'
        self.name_hyphen = self.name_lower.replace('_', '-')

        # recalculate arg
        self.arg.annotations = self.annotations
        self.arg.post_process(interface_prefix, c_namespace, 0)

        if utils.lookup_annotation(self.annotations, 'org.freedesktop.DBus.Deprecated') == 'true':
            self.deprecated = True

class Interface:
    def __init__(self, name):
        self.name = name
        self.methods = []
        self.signals = []
        self.properties = []
        self.annotations = []
        self.doc_string = ''
        self.doc_string_brief = ''
        self.since = ''
        self.deprecated = False

    def post_process(self, interface_prefix, c_namespace):
        if len(self.doc_string) == 0:
            self.doc_string = utils.lookup_docs(self.annotations)
        if len(self.doc_string_brief) == 0:
            self.doc_string_brief = utils.lookup_brief_docs(self.annotations)
        if len(self.since) == 0:
            self.since = utils.lookup_since(self.annotations)

        overridden_name = utils.lookup_annotation(self.annotations, 'org.gtk.GDBus.C.Name')
        if utils.is_ugly_case(overridden_name):
            name = overridden_name.replace('_', '')
            name_with_ns = c_namespace + name
            self.name_without_prefix = name
            self.camel_name = name_with_ns
            if len(c_namespace) > 0:
                self.ns_upper = utils.camel_case_to_uscore(c_namespace).upper() + '_'
                self.name_lower = utils.camel_case_to_uscore(c_namespace) + '_' + overridden_name.lower()
            else:
                self.ns_upper = ''
                self.name_lower = overridden_name.lower()
            self.name_upper = overridden_name.upper()

            #raise RuntimeError('handle Ugly_Case ', overridden_name)
        else:
            if overridden_name:
                name = overridden_name
            else:
                name = self.name
                if name.startswith(interface_prefix):
                    name = name[len(interface_prefix):]
            self.name_without_prefix = name
            name = utils.strip_dots(name)
            name_with_ns = utils.strip_dots(c_namespace + '.' + name)

            self.camel_name = name_with_ns
            if len(c_namespace) > 0:
                self.ns_upper = utils.camel_case_to_uscore(c_namespace).upper() + '_'
                self.name_lower = utils.camel_case_to_uscore(c_namespace) + '_' + utils.camel_case_to_uscore(name)
            else:
                self.ns_upper = ''
                self.name_lower = utils.camel_case_to_uscore(name_with_ns)
            self.name_upper = utils.camel_case_to_uscore(name).upper()

        self.name_hyphen = self.name_upper.lower().replace('_', '-')

        if utils.lookup_annotation(self.annotations, 'org.freedesktop.DBus.Deprecated') == 'true':
            self.deprecated = True

        for m in self.methods:
            m.post_process(interface_prefix, c_namespace)

        for s in self.signals:
            s.post_process(interface_prefix, c_namespace)

        for p in self.properties:
            p.post_process(interface_prefix, c_namespace)
