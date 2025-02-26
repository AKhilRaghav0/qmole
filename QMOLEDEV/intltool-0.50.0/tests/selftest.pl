#!/usr/local/bin/perl -w

#  Selftest for intltool
#
#  Copyright (C) 2000 Free Software Foundation.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of the
#  License, or (at your option) any later version.
#
#  This script is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this library; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#  Author(s): Kenneth Christiansen
#             Rodney Dawes

## Unset LINGUAS environment variable
$ENV{"LINGUAS"} = "";

## Release information
my $PROGRAM  = "selftest";
my $VERSION  = "0.1";
my $result;
my $case;

## Loaded modules
use strict;

## Always print as the first thing
$| = 1;

my $failed = 0;

my $srcdir = $ENV{"srcdir"} || ".";

my $strip_trailing_cr = ($^O eq 'MSWin32' || $^O eq 'msys') ? '--strip-trailing-cr' : '';

sub check_command_result
{
    my ($command) = shift;
    my ($testcase) = shift;

    my $result =`$command`;
    
    if ($? == 0 && ($result eq "" || $result =~ /^No differences/)) { 
        print "[OK]\n"; ## diff was empty, ie. files were equal
    } else { 
        print "[FAILED] ($testcase)\n"; 
        open OUT, ">>errors";
        print OUT $result;
        close OUT;
        $failed = 1;
    }
}

sub check_extract_result($)
{
    my ($testcase) = @_;
    check_command_result("diff -u $strip_trailing_cr $srcdir/results/$testcase.h cases/$testcase.h", $testcase);
}

sub check_merge_result($)
{
    my ($testcase) = @_;

    $testcase =~ s/\.in$//;
    check_command_result("diff -u $strip_trailing_cr $srcdir/results/$testcase cases/$testcase", $testcase);
}


sub check_multimerge_result
{
    my ($testcase) = shift;
    my ($language) = shift;

    $testcase =~ s/\.in$//;
    check_command_result("diff -u $strip_trailing_cr $srcdir/results/multi.$testcase $language/$testcase", $testcase);
}


## SELFTESTS

my $INTLTOOL_MERGE="env LC_ALL=C /usr/local/bin/perl ../intltool-merge";
my $INTLTOOL_EXTRACT="env LC_ALL=C /usr/local/bin/perl ../intltool-extract";

## Welcome notice

print "Running self test of the intltool module\n\n";
unlink "errors";

system("cp $srcdir/cases/*.* cases") if $srcdir ne ".";

print " 1. Extract messages from simple desktop file:                       ";
$case = "extract1.desktop";
system("$INTLTOOL_EXTRACT --type=gettext/ini --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print " 2. Merge translations into a Bonobo UI style file:                  ";
$case = "merge1.xml";
system("$INTLTOOL_MERGE -o --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print " 3. Extract messages from a multi-line XML file:                     ";
$case = "extract7.xml";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print " 4. Merge translations into a multi-line XML file:                   ";
$case = "merge7.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print " 5. Extract messages with unusual characters in the XML tag:         ";
$case = "extract3.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print " 6. Merge translations into a pong style XML file:                   ";
$case = "merge4.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print " 7. Extract messages from a glade style XML file:                    ";
$case = "extract6.glade";
system("$INTLTOOL_EXTRACT --type=gettext/glade --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print " 8. Merge translations into a dia sheet style XML file:              ";
$case = "merge5u.sheet";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print " 9. Merge translations into a dia sheet style XML file [broken XML]: ";
$case = "merge5p.sheet";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "10. Merge translations that include escaped .po strings:             ";
$case = "merge6.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "11. Merge translations into a .keys file:                            ";
$case = "merge8.keys";
system("$INTLTOOL_MERGE -k --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "12. Extract messages from a glade 2.0 style XML file:                ";
$case = "extract8.glade";
system("$INTLTOOL_EXTRACT --type=gettext/glade --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "13. Extract messages from an rfc822 style file:                      ";
$case = "extract10.templates_";
system("$INTLTOOL_EXTRACT --type=gettext/rfc822deb --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "14. Merge translations into an rfc822 style file:                    ";
$case = "merge10.templates";
system("$INTLTOOL_MERGE -r --quiet cases cases/${case}_ cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "15. Extract messages from a gconf schema style file:                 ";
$case = "test.schemas.in";
system("$INTLTOOL_EXTRACT --type=gettext/schemas --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "16. Merge translations into a gconf schema  style file:              ";
$case = "test.schemas";
system("$INTLTOOL_MERGE -s --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "17. Extract messages from a xml file using the xml:space attribute:  ";
$case = "extract9.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "18. Merge messages into xml file using the xml:space attribute:      ";
$case = "extract9.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "19. Merge translations into multiple files:                          ";
$case = "merge6.xml";
system("$INTLTOOL_MERGE -x -s --quiet --multiple-output cases cases/$case.in $case") == 0 or $failed = 1;
check_multimerge_result($case, "merge6");

print "20. Extract comments for translators from XML file:                  ";
$case = "extract-comments.xml";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "21. Merge into file with CDATA sections:                             ";
$case = "merge-cdata.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "22. Merge into XML file with deep translation/attribute nesting:     ";
$case = "merge-deepattr.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "23. Ignore empty strings from XML file                               ";
$case = "extract12.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "24. Extract messages from a xml file using inherited xml:space:      ";
$case = "space-preserve.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "25. Merge messages into xml file using inherited xml:space:          ";
$case = "space-preserve.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "26. Merge non-well-formed translations into xml file:                ";
$case = "merge11.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);
 
print "27. Extract UTF-8 encoded messages/comments from xml file:           ";
$case = "unicodetext.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "28. Extract iso-8859-1 encoded messages/comments from xml file:      ";
$case = "iso88591text.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "29. Extract messages from a quoted style file:                       ";
$case = "test-quoted.dtd";
system("$INTLTOOL_EXTRACT --type=gettext/quoted --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "30. Merge translations into multiple quoted style files:             ";
$case = "test-quoted.dtd";
system("$INTLTOOL_MERGE --quoted-style --quiet --multiple-output cases cases/$case $case") == 0 or $failed = 1;
check_multimerge_result($case, "test-quoted");

print "31. Extract comments for translators from a .desktop.in file:        ";
$case = "extract13.desktop.in";
system("$INTLTOOL_EXTRACT --type=gettext/ini --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "32. Merge translations from a .desktop.in file:                      ";
$case = "extract13.desktop.in";
$result = $case;
($result = $case) =~ s/\.in//;
system("$INTLTOOL_MERGE -d --quiet cases cases/$case cases/$result") == 0 or $failed = 1;
check_merge_result($case);

print "33. Decode XML entities in the right order when extracting:          ";
$case = "extract14.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "34. Merge XML translations, decoding entities in the right order:    ";
$case = "merge12.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "35. Extract messages from an XML file with msgctxts:                 ";
$case = "context.xml.in";
system("$INTLTOOL_EXTRACT --type=gettext/xml --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "36. Merge translations into an XML file with msgctxts:               ";
$case = "context.xml";
system("$INTLTOOL_MERGE -x --quiet cases cases/$case.in cases/$case") == 0 or $failed = 1;
check_merge_result($case);

print "37. Extract messages from an GtkBuilder XML file with msgctxts:      ";
$case = "extract-gtkbuilder.ui";
system("$INTLTOOL_EXTRACT --type=gettext/glade --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

print "38. Extract messages from an GSettings gschema XML file:             ";
$case = "gsettings.gschema.xml";
system("$INTLTOOL_EXTRACT --type=gettext/gsettings --quiet --update cases/$case") == 0 or $failed = 1;
check_extract_result($case);

system("rm -f cases/*.*") if $srcdir ne ".";
system("rm -rf C az extract9 fr fr_BE fr_FR merge6 schemasmerge1 schemasmerge2 spacepreserve test test-quoted merge6.xml");

exit $failed;
