#! /bin/sh
#
# Copyright (C) 2006 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# This is a script that determines the exported symbols of an object file.
# This is a separate script because $(GLOBAL_SYMBOL_PIPE) cannot be used in a
# Makefile, since it may contain dollar signs.

nm_cmd="/usr/bin/nm -p $1 | sed -n -e 's/^.*[	 ]\([BCDEGRST][BCDEGRST]*\)[	 ][	 ]*_\([_A-Za-z][_A-Za-z0-9]*\)$/\1 _\2 \2/p'"
eval $nm_cmd
