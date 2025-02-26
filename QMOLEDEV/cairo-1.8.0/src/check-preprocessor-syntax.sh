#!/bin/sh

LANG=C

test -z "$srcdir" && srcdir=.
cd "$srcdir"
stat=0


HEADERS=$all_cairo_headers
test "x$HEADERS" = x && HEADERS=`find . -name 'cairo*.h' ! -name 'cairo*-private.h' ! -name 'cairoint.h'`

PRIVATE=$all_cairo_private
test "x$PRIVATE" = x && PRIVATE=`find . -name 'cairo*-private.h' -or -name 'cairoint.h'`

SOURCES=$all_cairo_sources
test "x$SOURCES" = x && SOURCES=`find . -name 'cairo*.c' -or -name 'cairo*.cpp'`

ALL="/dev/null $HEADERS $PRIVATE $SOURCES"

echo 'Checking that public header files #include "cairo.h" first (or none)'

for x in $HEADERS; do
	grep '#.*\<include\>' "$x" /dev/null | head -n 1
done |
grep -v '"cairo[.]h"' |
grep -v 'cairo[.]h:' |
grep . && stat=1


echo 'Checking that private header files #include "some cairo header" first (or none)'

for x in $PRIVATE; do
	grep '#.*\<include\>' "$x" /dev/null | head -n 1
done |
grep -v '"cairo.*[.]h"' |
grep -v 'cairoint[.]h:' |
grep . && stat=1


echo 'Checking that source files #include "cairoint.h" first (or none)'

for x in $SOURCES; do
	grep '#.*\<include\>' "$x" /dev/null | head -n 1
done |
grep -v '"cairoint[.]h"' |
grep . && stat=1


echo 'Checking that there is no #include <cairo.*.h>'
grep '#.*\<include\>.*<.*cairo' $ALL && stat=1


echo 'Checking that feature conditionals are used with #if only (not #ifdef)'
grep '#if.*CAIRO_HAS_' $ALL | grep def && stat=1

exit $stat
