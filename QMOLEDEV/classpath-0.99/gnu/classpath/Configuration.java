/* Configuration.java --
   Copyright (C) 1998, 2001, 2003, 2005  Free Software Foundation, Inc.

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

package gnu.classpath;

/**
 * This file defines compile-time constants that can be accessed by
 * java code. It is pre-processed by configure.  
 */
public interface Configuration
{
  /**
   * The value of CLASSPATH_HOME is the location that the classpath
   * libraries and support files where installed in. It is set according to
   * the argument for --prefix given to configure and used to set the
   * System property gnu.classpath.home.
   */
  String CLASSPATH_HOME = "/usr/local/classpath";

  /**
   * The release version number of GNU Classpath.
   * It is set according to the value of 'version' in the configure[.in] file
   * and used to set the System property gnu.classpath.version.
   */
  String CLASSPATH_VERSION = "0.99";

  /**
   * The value of DEBUG is substituted according to whether the
   * "--enable-debug" argument was passed to configure. Code
   * which is made conditional based on the value of this flag - typically 
   * code that generates debugging output - will be removed by the optimizer 
   * in a non-debug build.
   */
  boolean DEBUG = false;

  /**
   * The value of LOAD_LIBRARY is substituted according to whether the
   * "--enable-load-library" or "--disable-load-library" argument was passed 
   * to configure.  By default, configure should define this is as true.
   * If set to false, loadLibrary() calls to load native function
   * implementations, typically found in static initializers of classes
   * which contain native functions, will be omitted.  This is useful for
   * runtimes which pre-link their native function implementations and do
   * not require additional shared libraries to be loaded.
   */
  boolean INIT_LOAD_LIBRARY = true;

  /**
   * Name of default AWT peer library.
   */
  String default_awt_peer_toolkit = "gnu.java.awt.peer.x.XToolkit";

  /**
   * Whether to automatically run the init* methods in java.lang.System
   * (the default) at class initialization time or whether to have the VM
   * explicitly invoke them.
   *
   * The default is false, meaning the VM does not explicitly run the
   * initializers. 
   * 
   */
  boolean JAVA_LANG_SYSTEM_EXPLICIT_INITIALIZATION = 
	  false;

  /**
   * The Eclipse Java Compiler jar file for use by the
   * com.sun.tools.javac implementation in tools.zip.
   */
  String ECJ_JAR = "/usr/bin/ecj.jar";

  /**
   * Set to <code>true</code> if the config script found that (a) an
   * implementation of java.math.BigInteger, based on the GNU MP library, is
   * desired in preference to a pure Java one, and (b) the GNU MP library was
   * found on the platform where the JVM is to run. Otherwise, this field is
   * set to <code>false</code>.
   */
  boolean WANT_NATIVE_BIG_INTEGER = true;

}
