/* This file was converted from the GNU Classpath Project by a
 * perl script, written by Jochen Hoenicke <jochen\@gnu.org>.
 */
/* RandomAccess.java -- A tagging interface that lists can use to tailor
   operations to the correct algorithm
   Copyright (C) 2001, 2005  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package gnu.java.util.collections;

/**
 * Marker interface used to inform <code>List</code> implementations that
 * they support fast (usually constant time) random access. This allows
 * generic list algorithms to tailor their behavior based on the list
 * type.
 * <p>
 *
 * For example, some sorts are n*log(n) on an array, but decay to quadratic
 * time on a linked list.  As a rule of thumb, this interface should be
 * used is this loop:<br>
 * <code>for (int i = 0, n = list.size(); i &lt; n; i++) list.get(i);</code>
 * <br>runs faster than this loop:<br>
 * <code>for (Iterator i = list.iterator(); i.hasNext(); ) i.next();</code>
 *
 * @author Eric Blake (ebb9@email.byu.edu)
 * @see List
 * @since 1.4
 * @status updated to 1.4
 */
public interface RandomAccess
{
  // Tagging interface only.
}
