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
import os.path
import re

from zenmapGUI.higwidgets.higwindows import HIGWindow
from zenmapGUI.higwidgets.higboxes import HIGVBox
from zenmapGUI.higwidgets.higbuttons import HIGButton, HIGToggleButton
from zenmapGUI.higwidgets.higboxes import HIGVBox, HIGHBox, HIGSpacer, hig_box_space_holder
from zenmapGUI.higwidgets.higlabels import HIGSectionLabel, HIGEntryLabel, HintWindow
from zenmapGUI.higwidgets.higtables import HIGTable
from zenmapGUI.higwidgets.higdialogs import HIGAlertDialog

from threading import Timer
from time import localtime
from types import StringTypes
from datetime import date as Date

from zenmapCore.Name import APP_DISPLAY_NAME
from zenmapCore.I18N import _
from zenmapCore.UmitLogging import log
from zenmapCore.NmapCommand import split_quoted
from zenmapCore.NmapParser import months
from zenmapCore.SearchResult import SearchDir, SearchDB
from zenmapCore.UmitConf import is_maemo
from zenmapCore.UmitConf import SearchConfig

from zenmapGUI.FileChoosers import DirectoryChooserDialog

search_config = SearchConfig()


class SearchParser(object):
    """This class is responsible for parsing the search string, and updating
    the search dictionary (which is, in turn, passed to classes that perform
    the actual search). It holds a reference to the SearchGUI object, which is
    used to access its search_dict dictionary, so that all dictionary handling
    is performed here. It is also responsible for adding additional directories
    to the SearchGUI object via the 'dir:' operator."""
    
    def __init__(self, search_gui):
        self.search_gui = search_gui
        self.search_dict = search_gui.search_dict
        
        # We need to make an operator->searchkey mapping, since the search entry
        # field and the search classes have different syntax.
        #
        # NOTE: if you want to add a new search key not handled by the SearchResult
        # class, you should add a new method match_CRITERIANAME to the SearchResult class.
        # For example, if you'd like a "noodles" criteria, you need to create the method
        # SearchResult.match_noodles(self, noodles_string). To see how searches are
        # actually performed, start reading from the SearchResult.search() method.
        self.ops2keys = dict()
        self.ops2keys["keyword"] = "keyword"
        self.ops2keys["profile"] = "profile"
        self.ops2keys["pr"] = "profile"
        self.ops2keys["target"] = "target"
        self.ops2keys["t"] = "target"
        self.ops2keys["option"] = "option"
        self.ops2keys["o"] = "option"
        self.ops2keys["date"] = "date"
        self.ops2keys["d"] = "date"
        self.ops2keys["after"] = "after"
        self.ops2keys["a"] = "after"
        self.ops2keys["before"] = "before"
        self.ops2keys["b"] = "before"
        self.ops2keys["os"] = "os"
        self.ops2keys["scanned"] = "scanned"
        self.ops2keys["sp"] = "scanned"
        self.ops2keys["open"] = "open"
        self.ops2keys["op"] = "open"
        self.ops2keys["closed"] = "closed"
        self.ops2keys["cp"] = "closed"
        self.ops2keys["filtered"] = "filtered"
        self.ops2keys["fp"] = "filtered"
        self.ops2keys["unfiltered"] = "unfiltered"
        self.ops2keys["ufp"] = "unfiltered"
        self.ops2keys["open|filtered"] = "open_filtered"
        self.ops2keys["ofp"] = "open_filtered"
        self.ops2keys["closed|filtered"] = "closed_filtered"
        self.ops2keys["cfp"] = "closed_filtered"
        self.ops2keys["service"] = "service"
        self.ops2keys["s"] = "service"
        self.ops2keys["inroute"] = "in_route"
        self.ops2keys["ir"] = "in_route"
        
        # This is not really an operator (see below)
        self.ops2keys["dir"] = "dir"
        
    def update(self, search):
        """Updates the search dictionary by parsing the input string."""
        
        # Kill leftover keys and parse again. SLOW? Not really.
        self.search_dict.clear()
        
        for word in split_quoted(search):
            if word.find(":") != -1:
                # We have an operator in our word, so we make the part left of
                # the semicolon a key, and the part on the right a value
                op, arg = word.split(":", 1)
                if op in self.ops2keys:
                    key = self.ops2keys[op]
                    if key in self.search_dict:
                        self.search_dict[key].append(arg)
                    else:
                        self.search_dict[key] = [arg]
            else:
                # Just a simple keyword
                if "keyword" in self.search_dict:
                    self.search_dict["keyword"].append(word)
                else:
                    self.search_dict["keyword"] = [word]
        
        # Check if we have any dir: operators in our map, and if so, add them to the
        # search_gui object and remove them from the map. The dir: operator isn't a real
        # operator, in a sense that it doesn't need to be processed by the
        # SearchResult.search() function. It is needed only to create a new SearchDir
        # object, which is then used to perform the actual search().
        if "dir" in self.search_dict:
            self.search_gui.init_search_dirs(self.search_dict["dir"])  
        else:
            self.search_gui.init_search_dirs([])

class SearchGUI(gtk.VBox, object):
    """This class is a VBox that holds the search entry field and buttons on
    top, and the results list on the bottom. The "Cancel" and "Open" buttons
    are a part of the SearchWindow class, not SearchGUI."""
    def __init__(self, search_window):
        gtk.VBox.__init__(self)

        self._create_widgets()
        self._pack_widgets()
        self._connect_events()

        # Search options
        self.options = {}
        self.options["file_extension"] = search_config.file_extension
        self.options["directory"] = search_config.directory
        self.options["search_db"] = search_config.search_db
        
        self.parsed_results = {}
        self._set_result_view()
        self.id = 0
        self.search_window = search_window
        
        # The Search* objects are created once per Search Window invocation, so that
        # they get a list of scans only once, not whenever the search conditions change
        if self.options["search_db"]:
            self.search_db = SearchDB()
        
        # Search directories can be added via the "dir:" operator, so it needs to be a map
        self.search_dirs = {}
        self.init_search_dirs()
        
        # We create an empty search dictionary, since SearchParser will fill it
        # with keywords as it encounters different operators in the search string.
        self.search_dict = dict()
        self.search_parser = SearchParser(self)
        
        # This list holds the (operator, argument) tuples, parsed from the GUI criteria rows
        self.gui_criteria_list = []
        
        # Do an initial "empty" search, so that the results window initially holds
        # all scans in the database
        self.search_parser.update("")
        self.start_search()
    
    def init_search_dirs(self, dirs = []):
        # Start fresh
        self.search_dirs.clear()
        
        # If specified, add the search directory from the Zenmap config file to the map
        conf_dir = self.options["directory"]
        if conf_dir:
            self.search_dirs[conf_dir] = SearchDir(conf_dir, self.options["file_extension"])
        
        # Process any other dirs (as added by the dir: operator)
        for dir in dirs:
            self.search_dirs[dir] = SearchDir(dir, self.options["file_extension"])
    
    def _create_widgets(self):
        # Search box and buttons
        self.search_top_hbox = HIGHBox()
        self.search_label = HIGSectionLabel(_("Search:"))
        self.search_entry = gtk.Entry()
        self.expressions_btn = HIGToggleButton(_("Expressions "), gtk.STOCK_EDIT)
        
        # The quick reference tooltip button
        self.search_tooltip_btn = HIGButton(" ", gtk.STOCK_INFO)
        
        # The expression VBox. This is only visible once the user clicks on "Expressions"
        self.expr_vbox = gtk.VBox()
        
        # Results section
        self.result_list = gtk.ListStore(str, str, int) # title, date, id
        self.result_view = gtk.TreeView(self.result_list)
        self.result_scrolled = gtk.ScrolledWindow()
        self.result_title_column = gtk.TreeViewColumn(_("Scan"))
        self.result_date_column = gtk.TreeViewColumn(_("Date"))
        
        self.expr_window = None
        
    def _pack_widgets(self):
        # Packing label, search box and buttons
        self.search_top_hbox.set_spacing(4)
        self.search_top_hbox.pack_start(self.search_label, False)
        self.search_top_hbox.pack_start(self.search_entry, True)
        self.search_top_hbox.pack_start(self.expressions_btn, False)
        self.search_top_hbox.pack_start(self.search_tooltip_btn, False)
        
        # The expressions (if any) should be tightly packed so that they don't take
        # too much screen real-estate
        self.expr_vbox.set_spacing(0)
        
        # Packing the result section
        self.result_scrolled.add(self.result_view)
        self.result_scrolled.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        
        # Packing it all together
        self.set_spacing(4)
        self.pack_start(self.search_top_hbox, False)
        self.pack_start(self.expr_vbox, False)
        self.pack_start(self.result_scrolled, True)
        
    def _connect_events(self):
        self.search_entry.connect("changed", self.update_search_entry)
        self.search_tooltip_btn.connect("clicked", self.show_quick_help)
        self.expressions_btn.connect("toggled", self.expressions_clicked)
    
    def show_quick_help(self, widget=None, extra=None):
        quick_help = """Entering the text into the search performs a <b>keyword search</b> - \
the search string is matched against the entire output of each scan.

To refine the search, you can use <b>operators</b> to search only within a specific part of \
a scan. Operators can be added to the search interactively if you click on the \
<b>Expressions</b> button, or you can enter them manually into the search field. \
You can also use <b>operator aliases</b> if you're an experienced user who likes to \
type in his searches quickly.

<b>profile: (pr:)</b> - Profile used.
<b>target: (t:)</b> - User-supplied target, or a rDNS result.
<b>option: (o:)</b> - Scan options.
<b>date: (d:)</b> - The date when scan was performed. Fuzzy matching is possible using the \
"~" suffix. Each "~" broadens the search by one day on "each side" of the date. In addition, \
it is possible to use the \"date:-n\" notation which means "n days ago".
<b>after: (a:)</b> - Matches scans made after the supplied date (<i>YYYY-MM-DD</i> or <i>-n</i>).
<b>before (b:)</b> - Matches scans made before the supplied date(<i>YYYY-MM-DD</i> or <i>-n</i>).
<b>os:</b> - All OS-related fields.
<b>scanned: (sp:)</b> - Matches a port if it was among those scanned.
<b>open: (op:)</b> - Open ports discovered in a scan.
<b>closed: (cp:)</b> - Closed ports discovered in a scan.
<b>filtered: (fp:)</b> - Filtered ports discovered in scan.
<b>unfiltered: (ufp:)</b> - Unfiltered ports found in a scan (using, for example, an ACK scan).
<b>open|filtered: (ofp:)</b> - Ports in the \"open|filtered\" state.
<b>closed|filtered: (cfp:)</b> - Ports in the \"closed|filtered\" state.
<b>service: (s:)</b> - All service-related fields.
<b>inroute: (ir:)</b> - Matches a router in the scan's traceroute output.
"""
        hint_window = HintWindow(quick_help)
        hint_window.show_all()
    
    def expressions_clicked(self, widget=None, extra=None):
        if len(self.expr_vbox.get_children()) == 0 and self.search_entry.get_text() == "":
            # This is the first time the user has clicked on "Show Expressions"
            # and the search entry box is empty, so we add a single Criterion row
            self.expr_vbox.pack_start(Criterion(self))
        
        if self.expressions_btn.get_active():
            # The Expressions GUI is about to be displayed. It needs to reflect all the
            # conditions in the search entry field, so a comparison between the entry field
            # and the GUI needs to be performed.
            
            # Make the search entry field insensitive while expressions are visible
            self.search_entry.set_sensitive(False)
            
            # Get a map of operator => argument from the Expressions GUI so that
            # we can compare them with the ones in the search entry field
            gui_ops = {}
            for criterion in self.expr_vbox.get_children():
                if criterion.operator in gui_ops:
                    gui_ops[criterion.operator].append(criterion.argument)
                else:
                    gui_ops[criterion.operator] = [criterion.argument]
            
            # We compare the search entry field to the Expressions GUI. Every
            # (operator, value) pair must be present in the GUI after this loop is done.
            for op, args in self.search_dict.iteritems():
                for arg in args:
                    if (op not in gui_ops) or (arg not in gui_ops[op]):
                        # We need to add this pair to the GUI
                        self.expr_vbox.pack_start(Criterion(self, op, arg), False)
            
            # Now we check if there are any leftover criterion rows that aren't present
            # in the search_dict (for example, if a user has deleted something from the
            # search entry field)
            for criterion in self.expr_vbox.get_children():
                if criterion.operator not in self.search_dict or \
                   criterion.argument not in self.search_dict[criterion.operator]:
                    criterion.destroy()
            # If we have deleted all rows, add an empty one
            if len(self.expr_vbox.get_children()) == 0:
                self.expr_vbox.pack_start(Criterion(self))
            
            # Display all elements
            self.expr_vbox.show_all()
        else:
            # The Expressions GUI is about to be hidden. No updates to the search entry field
            # are necessary, since it gets updated on every change in one of the criterion rows.
            self.expr_vbox.hide_all()
            self.search_entry.set_sensitive(True)
    
    def close(self):
        if self.expr_window != None:
            self.expr_window.close()
    
    def add_criterion(self, caller):
        # We need to find where the caller (Criteria object) is located among
        # all the rows, so that we can insert the new row after it
        caller_index = self.expr_vbox.get_children().index(caller)
        
        # Make a new Criteria row and insert it after the calling row
        criteria = Criterion(self, "keyword")
        self.expr_vbox.pack_start(criteria, False)
        self.expr_vbox.reorder_child(criteria, caller_index + 1)
        criteria.show_all()
    
    def remove_criterion(self, c):
        if len(self.expr_vbox.get_children()) > 1:
            c.destroy()
            self.criterion_changed()
    
    def criterion_changed(self):
        # We go through all criteria rows and make a new search string
        search_string = ""
        for criterion in self.expr_vbox.get_children():
            if criterion.operator != "keyword":
                search_string += criterion.operator + ":"
            search_string += criterion.argument.replace(" ", "") + " "
        
        self.search_entry.set_text(search_string.strip())
        
        self.search_parser.update(self.search_entry.get_text())
        self.start_search()
    
    def add_search_dir(self, dir):
        if dir not in self.search_dirs:
            self.search_dirs[dir] = SearchDir(dir, self.options["file_extension"])
    
    def update_search_entry(self, widget, extra=None):
        """Called when the search entry field is modified."""
        self.search_parser.update(widget.get_text())
        self.start_search()

    def start_search(self):
        if not self.options["search_db"] and not self.options["directory"]:
            d = HIGAlertDialog(message_format=_("No search method selected!"),
                               secondary_text=_("%s can search results on directories or \
inside it's own database. Please, select a method by choosing a directory or by checking \
the search data base option at the 'Search options' tab before start the search" % APP_DISPLAY_NAME))
            d.run()
            d.destroy()
            return

        self.clear_result_list()
        
        matched = 0
        total = 0
        if self.options["search_db"]:
            total += len(self.search_db.get_scan_results())
            for result in self.search_db.search(**self.search_dict):
                self.append_result(result)
                matched += 1

        for search_dir in self.search_dirs.itervalues():
            total += len(search_dir.get_scan_results())
            for result in search_dir.search(**self.search_dict):
                self.append_result(result)
                matched += 1
        
        #total += len(self.search_tabs.get_scan_results())
        #for result in self.search_tabs.search(**self.search_dict):
        #    self.append_result(result)
        #    matched += 1
        
        self.search_window.set_label_text("Matched <b>%s</b> out of <b>%s</b> scans." % \
                                         (str(matched), str(total)))

    def clear_result_list(self):
        for i in range(len(self.result_list)):
            iter = self.result_list.get_iter_root()
            del(self.result_list[iter])

    def append_result(self, parsed_result):
        title = ""
        if parsed_result.scan_name:
            title = parsed_result.scan_name
        elif parsed_result.filename:
            title = os.path.split(parsed_result.filename)[-1]
        elif parsed_result.profile_name and parsed_result.target:
            title = "%s on %s" % (parsed_result.profile_name, parsed_result.target)
        else:
            title = parsed_result.get_nmap_command()

        try:
            date = localtime(float(parsed_result.start))
            #date_field = "%02d %s %04d" % (date[2], months[date[1]][:3], date[0])
            date_field = "%04d-%02d-%02d %02d:%02d" % (date[0], date[1], date[2], date[3], date[4])
        except ValueError:
            date_field = _("Unknown")


        self.parsed_results[self.id] = [title, parsed_result]
        self.result_list.append([title, date_field, self.id])
        self.id += 1
        
    def get_selected_results(self):
        selection = self.result_view.get_selection()
        rows = selection.get_selected_rows()
        list_store = rows[0]

        results = {}
        for row in rows[1]:
            r = row[0]
            results[list_store[r][2]] = self.parsed_results[list_store[r][2]]

        return results

    def _set_result_view(self):
        self.result_view.set_enable_search(True)
        self.result_view.set_search_column(0)
        
        selection = self.result_view.get_selection()
        selection.set_mode(gtk.SELECTION_MULTIPLE)

        self.result_view.append_column(self.result_title_column)
        self.result_view.append_column(self.result_date_column)
        
        self.result_title_column.set_resizable(True)
        self.result_title_column.set_min_width(200)
        self.result_date_column.set_resizable(True)
        
        self.result_title_column.set_sort_column_id(0)
        self.result_date_column.set_sort_column_id(1)
        
        self.result_title_column.set_reorderable(True)
        self.result_date_column.set_reorderable(True)

        cell = gtk.CellRendererText()
        
        self.result_title_column.pack_start(cell, True)
        self.result_date_column.pack_start(cell, True)
        
        self.result_title_column.set_attributes(cell, text=0)
        self.result_date_column.set_attributes(cell, text=1)
        
    selected_results = property(get_selected_results)
    

class Criterion(gtk.HBox):
    """This class holds one criterion row, represented as an HBox.
    It holds a ComboBox and a Subcriterion's subclass instance, depending on the
    selected entry in the ComboBox. For example, when the 'Target' option is
    selected, a SimpleSubcriterion widget is displayed, but when the 'Date'
    operator is selected, a DateSubcriterion widget is displayed."""
    
    def __init__(self, search_window, operator="keyword", argument=""):
        """A reference to the search window is passed so that we can call
        add_criterion and remove_criterion."""
        gtk.HBox.__init__(self)
         
        self.search_window = search_window
        self.default_operator = operator
        self.default_argument = argument
        
        # We need this as a map, so that we can pass the operator into
        # the SimpleSubcriterion instance
        self.combo_entries = {"Keyword" : ["keyword"],
                              "Profile Name" : ["profile"],
                              "Target" : ["target"],
                              "Options" : ["option"],
                              "Date" : ["date", "after", "before"],
                              "Operating System" : ["os"],
                              "Port" : ["open", "scanned", "closed", "filtered",
                                        "unfiltered", "open_filtered", "closed_filtered"],
                              "Service" : ["service"],
                              "Host In Route" : ["inroute"],
                              "Include Directory" : ["dir"]}
        
        self._create_widgets()
        self._pack_widgets()
        self._connect_events()
    
    def _create_widgets(self):
        # A ComboBox containing the list of operators
        self.operator_combo = gtk.combo_box_new_text()
        
        # Sort all the keys from combo_entries and make an entry for each of them
        sorted_entries = self.combo_entries.keys()
        sorted_entries.sort()
        for name in sorted_entries:
            self.operator_combo.append_text(name)
        
        # Select the default operator
        for entry, operators in self.combo_entries.iteritems():
            for operator in operators:
                if operator == self.default_operator:
                    self.operator_combo.set_active(sorted_entries.index(entry))
                    break
        
        # Create a subcriterion
        self.subcriterion = self.new_subcriterion(self.default_operator, self.default_argument)
        
        # The "add" and "remove" buttons
        self.add_btn = HIGButton(" ", gtk.STOCK_ADD)
        self.remove_btn = HIGButton(" ", gtk.STOCK_REMOVE)
    
    def _pack_widgets(self):
        self.pack_start(self.operator_combo, False)
        self.pack_start(self.subcriterion, True, True)
        self.pack_start(self.add_btn, False)
        self.pack_start(self.remove_btn, False)
    
    def _connect_events(self):
        self.operator_combo.connect("changed", self.operator_changed)
        self.add_btn.connect("clicked", self.add_clicked)
        self.remove_btn.connect("clicked", self.remove_clicked)
    
    def get_operator(self):
        return self.subcriterion.operator
    
    def get_argument(self):
        return self.subcriterion.argument
    
    def add_clicked(self, widget=None, extra=None):
        self.search_window.add_criterion(self)
    
    def remove_clicked(self, widget=None, extra=None):
        self.search_window.remove_criterion(self)
    
    def value_changed(self, op, arg):
        """Subcriterion instances call this method when something changes
        inside of them."""
        # We let the search window know about the change
        self.search_window.criterion_changed()
    
    def new_subcriterion(self, operator="keyword", argument=""):
        if operator in self.combo_entries["Date"]:
            return DateSubcriterion(operator, argument)
        elif operator in self.combo_entries["Port"]:
            return PortSubcriterion(operator, argument)
        elif operator == "dir":
            return DirSubcriterion(operator, argument)
        else:
            return SimpleSubcriterion(operator, argument)
    
    def operator_changed(self, widget=None, extra=None):
        """This function is called when the user selects a different entry in
        the Criterion's ComboBox."""
        # Destroy the previous subcriterion
        self.subcriterion.destroy()
        
        # Create a new subcriterion depending on the selected operator
        selected = self.operator_combo.get_active_text()
        operator = self.combo_entries[selected][0]
        self.subcriterion = self.new_subcriterion(operator)
        
        # Pack it, and place it on the right side of the ComboBox
        self.pack_start(self.subcriterion, True, True)
        self.reorder_child(self.subcriterion, 1)
        
        # Notify the search window about the change
        self.search_window.criterion_changed()
        
        # Good to go
        self.subcriterion.show_all()
    
    operator = property(get_operator)
    argument = property(get_argument)

class Subcriterion(gtk.HBox):
    """This class is a base class for all subcriterion types. Depending on the
    criterion selected in the Criterion's ComboBox, a subclass of Subcriterion
    is created to display the appropriate GUI."""
    def __init__(self):
        gtk.HBox.__init__(self)
        
        self.operator = ""
        self.argument = ""
    
    def value_changed(self):
        """Propagates the operator and the argument up to the Criterion parent."""
        self.get_parent().value_changed(self.operator, self.argument)

class SimpleSubcriterion(Subcriterion):
    """This class represents all 'simple' criterion types that need only an
    entry box in order to define the criterion."""
    def __init__(self, operator="keyword", argument=""):
        Subcriterion.__init__(self)
        
        self.operator = operator
        self.argument = argument
        
        self._create_widgets()
        self._pack_widgets()
        self._connect_widgets()
    
    def _create_widgets(self):
        self.entry = gtk.Entry()
        if self.argument:
            self.entry.set_text(self.argument)
    
    def _pack_widgets(self):
        self.pack_start(self.entry, True)
    
    def _connect_widgets(self):
        self.entry.connect("changed", self.entry_changed)
    
    def entry_changed(self, widget=None, extra=None):
        self.argument = widget.get_text()
        self.value_changed()

class PortSubcriterion(Subcriterion):
    """This class shows the port criterion GUI."""
    def __init__(self, operator="open", argument=""):
        Subcriterion.__init__(self)
        
        self.operator = operator
        self.argument = argument
        
        self._create_widgets()
        self._pack_widgets()
        self._connect_widgets()
    
    def _create_widgets(self):
        self.entry = gtk.Entry()
        if self.argument:
            self.entry.set_text(self.argument)
        
        self.label = gtk.Label("  is  ")
        
        self.port_state_combo = gtk.combo_box_new_text()
        states = ["open", "scanned", "closed", "filtered", "unfiltered", "open|filtered",
                  "closed|filtered"]
        for state in states:
            self.port_state_combo.append_text(state)
        self.port_state_combo.set_active(states.index(self.operator.replace("_", "|")))
    
    def _pack_widgets(self):
        self.pack_start(self.entry, True)
        self.pack_start(self.label, False)
        self.pack_start(self.port_state_combo, False)
    
    def _connect_widgets(self):
        self.entry.connect("changed", self.entry_changed)
        self.port_state_combo.connect("changed", self.port_criterion_changed)
    
    def entry_changed(self, widget=None, extra=None):
        self.argument = widget.get_text()
        self.value_changed()
    
    def port_criterion_changed(self, widget=None, extra=None):
        self.operator = widget.get_active_text()
        self.value_changed()

class DirSubcriterion(Subcriterion):
    def __init__(self, operator="dir", argument=""):
        Subcriterion.__init__(self)
        
        self.operator = operator
        self.argument = argument
        
        self._create_widgets()
        self._pack_widgets()
        self._connect_widgets()
    
    def _create_widgets(self):
        self.dir_entry = gtk.Entry()
        if self.argument:
            self.dir_entry.set_text(self.argument)
        self.chooser_btn = HIGButton("Choose...", gtk.STOCK_OPEN)
        
    def _pack_widgets(self):
        self.pack_start(self.dir_entry, True)
        self.pack_start(self.chooser_btn, False)
    
    def _connect_widgets(self):
        self.chooser_btn.connect("clicked", self.choose_clicked)
        self.dir_entry.connect("changed", self.dir_entry_changed)
    
    def choose_clicked(self, widget=None, extra=None):
        # Display a directory chooser dialog
        chooser_dlg = DirectoryChooserDialog("Include folder in search")
        
        if chooser_dlg.run() == gtk.RESPONSE_OK:
            self.dir_entry.set_text(chooser_dlg.get_filename())
        
        chooser_dlg.destroy()
    
    def dir_entry_changed(self, widget=None, extra=None):
        self.argument = widget.get_text()
        self.value_changed()

class DateSubcriterion(Subcriterion):
    def __init__(self, operator="date", argument=""):
        Subcriterion.__init__(self)
        
        self.text2op = {"is" : "date",
                        "after" : "after",
                        "before" : "before"}
        
        self.operator = operator
        
        self._create_widgets()
        self._pack_widgets()
        self._connect_widgets()
        
        # Count the fuzzy operators, so that we can append them to the argument later
        self.fuzzies = argument.count("~")
        argument = argument.replace("~", "")
        self.minus_notation = False
        if re.match("\d\d\d\d-\d\d-\d\d", argument) != None:
            year, month, day = argument.split("-")
            self.date = (int(year), int(month), int(day))
            self.argument = argument
        elif re.match("[-|\+]\d+", argument) != None:
            # Convert the date from the "-n" notation into YYYY-MM-DD
            parsed_date = Date.fromordinal(Date.today().toordinal() + int(argument))
            self.argument = argument
            self.date = (parsed_date.year, parsed_date.month, parsed_date.day)
            
            self.minus_notation = True
        else:
            self.date = localtime()[:3]
            self.argument = "%d-%02d-%02d" % (self.date[0], self.date[1], self.date[2])
        
        # Append fuzzy operators, if any
        self.argument += "~" * self.fuzzies

    def _create_widgets(self):
        t = localtime()
        self.date_criterion_combo = gtk.combo_box_new_text()
        self.date_criterion_combo.append_text("is")
        self.date_criterion_combo.append_text("after")
        self.date_criterion_combo.append_text("before")
        if self.operator == "date":
            self.date_criterion_combo.set_active(0)
        elif self.operator == "after":
            self.date_criterion_combo.set_active(1)
        else:
            self.date_criterion_combo.set_active(2)
        self.date_button = HIGButton()
        
    def _pack_widgets(self):
        self.pack_start(self.date_criterion_combo, False)
        self.pack_start(self.date_button, True)
    
    def _connect_widgets(self):
        self.date_criterion_combo.connect("changed", self.date_criterion_changed)
        self.date_button.connect("clicked", self.show_calendar)
    
    def date_criterion_changed(self, widget=None, extra=None):
        self.operator = self.text2op[widget.get_active_text()]
        
        # Let the parent know that the operator has changed
        self.value_changed()
    
    def show_calendar(self, widget):
        calendar = DateCalendar()
        calendar.connect_calendar(self.update_button)
        calendar.show_all()

    def update_button(self, widget):
        date = list(widget.get_date())
        date[1] += 1 # Add 1 to month, because calendar date is zero-based
        self.date = tuple(date)
        
        # Set the argument, using the search format
        year, month, day = self.date
        if self.minus_notation:
            # We need to calculate the date's offset from today, so that we can
            # represent the date in the "-n" notation
            selected_date = Date(year, month, day)
            today = Date.today()
            offset = selected_date.toordinal() - today.toordinal()
            if offset > 0:
                self.argument = "+" + str(offset)
            else:
                self.argument = str(offset)
        else:
            self.argument = "%d-%02d-%02d" % (year, month, day)
        self.argument += "~" * self.fuzzies
        
        # Let the parent know about the change
        self.value_changed()

    def set_date(self, date):
        # Localtime Format: (year, month, day)
        self.date_button.set_label("%02d %s %04d" % (date[2], months[date[1]][:3], date[0]))
        self._date = date

    def get_date(self):
        return self._date

    date = property(get_date, set_date)
    _date = localtime()[:3]

class DateCalendar(gtk.Window, object):
    def __init__(self):
        gtk.Window.__init__(self, gtk.WINDOW_POPUP)
        self.set_position(gtk.WIN_POS_MOUSE)
        
        self.calendar = gtk.Calendar()
        self.add(self.calendar)

    def connect_calendar(self, update_button_cb):
        self.calendar.connect("day-selected-double-click", \
                              self.kill_calendar, update_button_cb)

    def kill_calendar(self, widget, method):
        method(widget)
        self.destroy()
