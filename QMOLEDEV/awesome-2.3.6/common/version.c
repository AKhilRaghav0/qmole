/*
 * version.c - version message utility functions
 *
 * Copyright © 2008 Julien Danjou <julien@danjou.info>
 * Copyright © 2008 Hans Ulrich Niedermann <hun@n-dimensional.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <stdio.h>

#include "common/version.h"
#include "awesome-version-internal.h"

/** \brief Print version message and quit program.
 * \param executable program name
 */
void
eprint_version(const char *const executable)
{
    printf("%s (awesome) " AWESOME_VERSION_INTERNAL
	   " (" AWESOME_RELEASE ")\n"
	   "compiled",
	   executable);
#if defined(__DATE__) && defined(__TIME__)
    printf(" at " __DATE__ " " __TIME__);
#endif
    printf(" for %s", AWESOME_COMPILE_MACHINE);
#if defined(__GNUC__)				\
    && defined(__GNUC_MINOR__)			\
    && defined(__GNUC_PATCHLEVEL__)
    printf(" by gcc version %d.%d.%d",
	   __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif
    printf(" (%s@%s)\n", AWESOME_COMPILE_BY, AWESOME_COMPILE_HOSTNAME);
    exit(EXIT_SUCCESS);
}

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
