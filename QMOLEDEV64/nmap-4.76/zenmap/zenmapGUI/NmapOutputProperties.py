#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2005 Insecure.Com LLC.
#
# Author: Adriano Monteiro Marques <py.adriano@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

import gtk
import gtk.gdk
import pango

from zenmapCore.I18N import _
from zenmapCore.UmitConf import NmapOutputHighlight

from zenmapGUI.higwidgets.higdialogs import HIGDialog
from zenmapGUI.higwidgets.hignotebooks import HIGNotebook
from zenmapGUI.higwidgets.higboxes import HIGVBox
from zenmapGUI.higwidgets.higtables import HIGTable
from zenmapGUI.higwidgets.higlabels import HIGEntryLabel
from zenmapGUI.higwidgets.higbuttons import HIGButton, HIGToggleButton

class NmapOutputProperties(HIGDialog):
    def __init__(self, nmap_output_view):
        HIGDialog.__init__(self, _("Nmap Output Properties"),
                           buttons=(gtk.STOCK_CLOSE, gtk.RESPONSE_CLOSE))
        
        self.nmap_highlight = NmapOutputHighlight()

        self.__create_widgets()
        self.__pack_widgets()
        self.highlight_tab()

        self.vbox.show_all()

    def __create_widgets(self):
        self.properties_notebook = HIGNotebook()

    def __pack_widgets(self):
        self.vbox.pack_start(self.properties_notebook)

    def highlight_tab(self):
        # Creating highlight tab main box
        self.highlight_main_vbox = HIGVBox()

        # Creating highlight tab main table
        self.highlight_main_table = HIGTable()
        self.highlight_main_table.set_border_width(6)

        #############
        # Properties:
        self.property_names = {"details": [_("details"), "MAC Address:"],
                               "port_list": [_("port listing title"), "PORT   STATE   SERVICE"],
                               "open_port": [_("open port"), "22/tcp   open   ssh"],
                               "closed_port": [_("closed port"), "70/tcp   closed   gopher"],
                               "filtered_port": [_("filtered port"), "80/tcp   filtered   http"],
                               "date": [_("date"), "2006-05-26 11:14 BRT"],
                               "hostname": [_("hostname"), "scanme.nmap.org"],
                               "ip": [_("ip"), "127.0.0.1"]}

        for p in self.property_names:
            settings = self.nmap_highlight.__getattribute__(p)

            self.property_names[p].append(settings[0])
            self.property_names[p].append(settings[1])
            self.property_names[p].append(settings[2])
            self.property_names[p].append(gtk.gdk.Color(*settings[3]))
            self.property_names[p].append(gtk.gdk.Color(*settings[4]))
            self.property_names[p].append(settings[5])
            
        # Creating properties and related widgets and attaching it to main table
        y1 = 0
        y2 = 1
        for p in self.property_names:
            hp = HighlightProperty(p, self.property_names[p])
            self.highlight_main_table.attach(hp.property_name_label, 0, 1, y1, y2)
            self.highlight_main_table.attach(hp.example_label, 1, 2, y1, y2)
            self.highlight_main_table.attach(hp.bold_tg_button, 2, 3, y1, y2)
            self.highlight_main_table.attach(hp.italic_tg_button, 3, 4, y1, y2)
            self.highlight_main_table.attach(hp.underline_tg_button, 4, 5, y1, y2)
            self.highlight_main_table.attach(hp.text_color_button, 5, 6, y1, y2)
            self.highlight_main_table.attach(hp.highlight_color_button, 6, 7, y1, y2)

            # Setting example styles and colors
            hp.update_example()

            self.property_names[p].append(hp)
            
            y1 += 1
            y2 += 1


        # Packing main table into main vbox
        self.highlight_main_vbox.pack_start(self.highlight_main_table)

        # Adding color tab
        self.properties_notebook.append_page(self.highlight_main_vbox,
                                             gtk.Label(_("Highlight definitions")))


class HighlightProperty(object):
    def __init__(self, property_name, property):
        self.__create_widgets()

        self.property_name = property_name
        
        self.property_label = property[0].capitalize()
        self.example = property[1]
        self.bold = property[2]
        self.italic = property[3]
        self.underline = property[4]
        
        self.text_color = property[5]
        self.highlight_color = property[6]

        self.__connect_buttons()

    def __create_widgets(self):
        self.property_name_label = HIGEntryLabel("")
        self.example_label = HIGEntryLabel("")
        self.bold_tg_button = HIGToggleButton("", gtk.STOCK_BOLD)
        self.italic_tg_button = HIGToggleButton("", gtk.STOCK_ITALIC)
        self.underline_tg_button = HIGToggleButton("", gtk.STOCK_UNDERLINE)
        self.text_color_button = HIGButton(_("Text"), stock=gtk.STOCK_SELECT_COLOR)
        self.highlight_color_button = HIGButton(_("Highlight"), stock=gtk.STOCK_SELECT_COLOR)

    def __connect_buttons(self):
        self.bold_tg_button.connect("toggled", self.update_example)
        self.italic_tg_button.connect("toggled", self.update_example)
        self.underline_tg_button.connect("toggled", self.update_example)

        self.text_color_button.connect("clicked", self.text_color_dialog)
        self.highlight_color_button.connect("clicked", self.highlight_color_dialog)


    ####################################
    # Text color dialog
    
    def text_color_dialog(self, widget):
        color_dialog = gtk.ColorSelectionDialog("%s %s" % (self.label, _("text color")))
        color_dialog.colorsel.set_current_color(self.text_color)
        
        color_dialog.ok_button.connect("clicked", self.text_color_dialog_ok, color_dialog)
        color_dialog.cancel_button.connect("clicked",
                                           self.text_color_dialog_cancel, color_dialog)
        color_dialog.connect("delete-event", self.text_color_dialog_close, color_dialog)
        
        color_dialog.run()

    def text_color_dialog_ok(self, widget, color_dialog):
        self.text_color = color_dialog.colorsel.get_current_color()
        color_dialog.destroy()
        self.update_example()

    def text_color_dialog_cancel(self, widget, color_dialog):
        color_dialog.destroy()

    def text_color_dialog_close(self, widget, extra, color_dialog):
        color_dialog.destroy()


    #########################################
    # Highlight color dialog
    def highlight_color_dialog(self, widget):
        color_dialog = gtk.ColorSelectionDialog("%s %s" % (self.property_name,
                                                           _("highlight color")))
        color_dialog.colorsel.set_current_color(self.highlight_color)

        color_dialog.ok_button.connect("clicked", self.highlight_color_dialog_ok, color_dialog)
        color_dialog.cancel_button.connect("clicked", self.highlight_color_dialog_cancel,
                                           color_dialog)
        color_dialog.connect("delete-event", self.highlight_color_dialog_close, color_dialog)
        
        color_dialog.run()

    def highlight_color_dialog_ok(self, widget, color_dialog):
        self.highlight_color = color_dialog.colorsel.get_current_color()
        color_dialog.destroy()
        self.update_example()

    def highlight_color_dialog_cancel(self, widget, color_dialog):
        color_dialog.destroy()

    def highlight_color_dialog_close(self, widget, extra, color_dialog):
        color_dialog.destroy()

    def update_example(self, widget=None):
        start = 0
        end = len(self.example)
        
        attributes = pango.AttrList()

        attributes.insert(pango.AttrForeground(self.text_color.red, self.text_color.green,
                                               self.text_color.blue, start, end))
        attributes.insert(pango.AttrBackground(self.highlight_color.red,
                                               self.highlight_color.green,
                                               self.highlight_color.blue,
                                               start, end))

        # Bold verification
        if self.bold_tg_button.get_active():
            attributes.insert(pango.AttrWeight(pango.WEIGHT_HEAVY, start, end))
        else:
            attributes.insert(pango.AttrWeight(pango.WEIGHT_NORMAL, start, end))

        # Italic verification
        if self.italic_tg_button.get_active():
            attributes.insert(pango.AttrStyle(pango.STYLE_ITALIC, start, end))
        else:
            attributes.insert(pango.AttrStyle(pango.STYLE_NORMAL, start, end))

        # Underline verification
        if self.underline_tg_button.get_active():
            attributes.insert(pango.AttrUnderline(pango.UNDERLINE_SINGLE, start, end))
        else:
            attributes.insert(pango.AttrUnderline(pango.UNDERLINE_NONE, start, end))
        
        self.example_label.set_attributes(attributes)


    def show_bold(self, widget):
        self.example_label.set_markup("<>")

    def get_example(self):
        return self.example_label.get_text()

    def set_example(self, example):
        self.example_label.set_text(example)

    def get_bold(self):
        if self.bold_tg_button.get_active():
            return 1
        return 0

    def set_bold(self, bold):
        self.bold_tg_button.set_active(bold)

    def get_italic(self):
        if self.italic_tg_button.get_active():
            return 1
        return 0

    def set_italic(self, italic):
        self.italic_tg_button.set_active(italic)

    def get_underline(self):
        if self.underline_tg_button.get_active():
            return 1
        return 0

    def set_underline(self, underline):
        self.underline_tg_button.set_active(underline)

    def get_label(self):
        return self.property_name_label.get_text()

    def set_label(self, label):
        self.property_name_label.set_text(label)

    label = property(get_label, set_label)
    example = property(get_example, set_example)
    bold = property(get_bold, set_bold)
    italic = property(get_italic, set_italic)
    underline = property(get_underline, set_underline)

if __name__ == "__main__":
    n = NmapOutputProperties(None)
    n.run()
    gtk.main()
