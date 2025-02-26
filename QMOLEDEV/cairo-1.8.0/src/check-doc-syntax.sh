#!/bin/sh

LANG=C

if grep --version 2>/dev/null | grep GNU >/dev/null; then
	:
else
	echo "GNU grep not found; skipping test"
	exit 0
fi

test -z "$srcdir" && srcdir=.
stat=0

echo Checking documentation for incorrect syntax

cd "$srcdir"

# Note: this test is also run from doc/public/ to check the SGML files

if test "x$SGML_DOCS" = x; then
    FILES=$all_cairo_files
    if test "x$FILES" = x; then
        FILES=`find . -name 'cairo*.h' -or -name 'cairo*.c' -or -name 'cairo*.cpp'`
    fi
fi

enum_regexp='\([^%@]\|^\)\<\(FALSE\|TRUE\|NULL\|CAIRO_[0-9A-Z_]*\)\($\|[^(A-Za-z0-9_]\)'
if test "x$SGML_DOCS" = x; then
	enum_regexp='^[^:]*:[/ ][*]\(\|[ \t].*\)'$enum_regexp
fi
if echo $FILES | xargs grep . /dev/null | sed -e '/<programlisting>/,/<\/programlisting>/d' | grep "$enum_regexp" | grep -v '#####'; then
	stat=1
	echo Error: some macros in the docs are not prefixed by percent sign.
	echo Fix this by searching for the following regexp in the above files:
	echo "	'$enum_regexp'"
fi

type_regexp='\( .*[^#]\| \|^\)\<cairo[0-9a-z_]*_t\>\($\|[^:]$\|[^:].\)'
if test "x$SGML_DOCS" = x; then
	type_regexp='^[^:]*:[/ ][*]'$type_regexp
else
	type_regexp='\(.'$type_regexp'\)\|\('$type_regexp'.\)'
fi

if echo $FILES | xargs grep . /dev/null | sed -e '/<programlisting>/,/<\/programlisting>/d' | grep "$type_regexp" | grep -v '#####'; then
	stat=1
	echo Error: some type names in the docs are not prefixed by hash sign,
	echo neither are the only token in the doc line followed by colon.
	echo Fix this by searching for the following regexp in the above files:
	echo "	'$type_regexp'"
fi

func_regexp='\([^#]\|^\)\<\(cairo_[][<>/0-9a-z_]*\>[^][<>(]\)'
if test "x$SGML_DOCS" = x; then
	func_regexp='^[^:]*:[/ ][*]\(\|[ \t].*\)'$func_regexp
fi

# We need to filter out gtk-doc markup errors for program listings.
if echo $FILES | xargs grep . /dev/null | sed -e '/<programlisting>/,/<\/programlisting>/d' | grep "$func_regexp" | grep -v '^[^:]*: [*] [a-z_0-9]*:$' | grep -v '#####'; then
	stat=1
	echo Error: some function names in the docs are not followed by parentheses.
	echo Fix this by searching for the following regexp in the above files:
	echo "	'$func_regexp'"
fi

note_regexp='NOTE'
if echo $FILES | xargs grep "$note_regexp" /dev/null; then
	stat=1
	echo Error: some source files contain the string 'NOTE'.
	echo Be civil and replace it by 'Note' please.
fi

exit $stat
