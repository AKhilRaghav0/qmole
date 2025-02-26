/* This file was converted from the GNU Classpath Project by a
 * perl script, written by Jochen Hoenicke <jochen\@gnu.org>.
 */
/* UnsupportedOperationException.java -- thrown when an operation is not
   supported
   Copyright (C) 1998, 1999, 2001, 2002, 2005  Free Software Foundation, Inc.

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
 * This exception is thrown by an object when an operation is
 * requested of it that it does not support.
 *
 * @author Warren Levy (warrenl@cygnus.com)
 * @author Andrew John Hughes (gnu_andrew@member.fsf.org)
 * @since 1.2
 * @status updated to 1.5
 */
public class UnsupportedOperationException extends RuntimeException
{
  /**
   * Compatible with JDK 1.2+.
   */
  private static final long serialVersionUID = -1242599979055084673L;

  /**
   * Create an exception without a message.
   */
  public UnsupportedOperationException()
  {
  }

  /**
   * Create an exception with a message.
   *
   * @param s the message
   */
  public UnsupportedOperationException(String s)
  {
    super(s);
  }

  /**
   * <p>
   * Constructs a <code>UnsupportedOperationException</code> using
   * the specified error message, which should give further details
   * as to the reason for this exception.  The specified cause
   * <code>Throwable</code> may be used to provide additional history,
   * with regards to the root of the problem.  It is perfectly valid
   * for this to be null, if the cause of the problem is unknown.
   * </p>
   * <p>
   * <strong>Note</strong>: the detail message from the cause is not
   * automatically incorporated into the resulting detail message of
   * this exception.
   * </p>
   *
   * @param message the detail message, which should give the reason for
   *                this exception being thrown.
   * @param cause the cause of this exception, or null if the cause
   *              is unknown.
   * @since 1.5
   */
  public UnsupportedOperationException(String message, Throwable cause)
  {
    super(message, cause);
  }

  /**
   * <p>
   * Constructs a <code>UnsupportedOperationException</code> using
   * the specified cause <code>Throwable</code>, which may be used
   * to provide additional history, with regards to the root of the
   * problem.  It is perfectly valid for this to be null, if the
   * cause of the problem is unknown.
   * </p>
   * <p>
   * The detail message is automatically constructed from the detail
   * message of the supplied causal exception.  If the cause is null,
   * then the detail message will also be null.  Otherwise, the detail
   * message of this exception will be that of the causal exception.
   * This makes this constructor very useful for simply wrapping another
   * exception.
   * </p>
   *
   * @param cause the cause of this exception, or null if the cause
   *              is unknown.
   * @since 1.5
   */
  public UnsupportedOperationException(Throwable cause)
  {
    super(cause);
  }

}
