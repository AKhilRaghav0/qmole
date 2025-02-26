#! /bin/sh
# lib/gen-classlist.sh.  Generated from gen-classlist.sh.in by configure.

# Make sure sorting is done the same on all configurations
# Note the use of sort -r below. This is done explicitly to work around
# a gcj bug (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=21418)
LC_ALL=C; export LC_ALL
LANG=C; export LANG

abs_top_builddir=`cd "${top_builddir}"; pwd`
abs_top_srcdir=`cd ".."; pwd`

echo "Adding java source files from srcdir '${abs_top_srcdir}'."

# We construct 'classes.1' as a series of lines.  Each line
# has three fields, which are separated by spaces.  The first
# field is the package of this class (separated by "/"s).
# The second field is the absolute pathname of the top-level directory
# for this file.  E.g., it might look like
# "/home/jezebel/classpath/vm/reference".
# The third field is the file name, like "java/lang/Object.java".
# We do this because it makes splitting for the gcj build much
# cheaper.
(cd ..
 /usr/bin/find java javax gnu org sun -follow -name '*.java' -print |
 sort -r | sed -e 's,/\([^/]*\)$, \1,' |
 while read pkg file; do
    echo $pkg ${abs_top_srcdir} $pkg/$file
 done) > ${top_builddir}/lib/classes.1

# The same, but for the external code.
for dir in \
   ${abs_top_srcdir}/external/w3c_dom \
   ${abs_top_srcdir}/external/sax \
   ${abs_top_srcdir}/external/relaxngDatatype \
   ${abs_top_srcdir}/external/jsr166 \
   ; do
   (cd $dir
   for subdir in java javax gnu org sun; do
      if test -d $subdir; then
	 /usr/bin/find $subdir -follow -name '*.java' -print |
	 sort -r | sed -e 's,/\([^/]*\)$, \1,' |
	 while read pkg file; do
	    echo $pkg $dir $pkg/$file
	 done
      fi
   done)
done >> ${top_builddir}/lib/classes.1

# Generate files for the VM classes.
: > vm.omit
: > vm.add
vm_dirlist=`echo "${top_srcdir}/vm/reference" | sed -e 's/:/ /g'`
echo "Adding java source files from VM directory $vm_dirlist"
for dir in $vm_dirlist; do
   (cd $dir
   for subdir in java javax gnu org com sun; do
      if test -d $subdir; then
	 /usr/bin/find $subdir -name '*.java' -print
      fi
   done) | sed -e 's,/\([^/]*\)$, \1,' |
   while read pkg file; do
      echo $pkg $dir $pkg/$file >> vm.add
      echo $pkg/$file >> vm.omit
   done
done

# Only include generated files once.
if test "$abs_top_builddir" != "$abs_top_srcdir"; then
  echo "Adding generated files in builddir '${top_builddir}'."
  # Currently the only generated files are in gnu.*.
  (cd ${top_builddir}; /usr/bin/find gnu -follow -name '*.java' -print) |
  sort | sed -e 's,/\([^/]*\)$, \1,' |
  while read pkg file; do
     echo $pkg $top_builddir $pkg/$file
  done >> ${top_builddir}/lib/classes.1
fi


cat $1.omit vm.omit > tmp.omit
for dir in $vm_dirlist; do
   if test -f $dir/$1.omit; then
      cat $dir/$1.omit >> tmp.omit
   fi
done

# Mangle the omit expressions into a script suitable for old awk.
# Exploit the fact that many omissions are not regular expressions:
# assume a single file is listed if it does not contain '*', '$',
# and ends in '.java'.
sed_omit_hash='
1i\
  BEGIN {\
    omit[""] = 1
$a\
  }
/[*$]/d
/\.java$/!d
s|^|    omit["|
s|$|"] = 1|'
sed_omit_main_loop='
1i\
  {\
    if (omit[$3]) next
$a\
    print\
  }
/^[^*$]*\.java$/d
s|/|\\/|g
s|^|  if ($3 ~ /|
s|$|/) next|'

sed "$sed_omit_hash" <tmp.omit >tmp.awk
sed "$sed_omit_main_loop" <tmp.omit >>tmp.awk
gawk -f tmp.awk < ${top_builddir}/lib/classes.1 > ${top_builddir}/lib/classes.tmp
mv ${top_builddir}/lib/classes.tmp ${top_builddir}/lib/classes.1

vm_omitlist=
for dir in $vm_dirlist; do
   if test -f $dir/$1.omit; then
      vm_omitlist="$vm_omitlist $dir/$1.omit"
   fi
done
if test -n "$vm_omitlist"; then
   cat $vm_omitlist | sed "$sed_omit_hash" > tmp.awk
   cat $vm_omitlist | sed "$sed_omit_main_loop" >> tmp.awk
fi
gawk -f tmp.awk < vm.add >>${top_builddir}/lib/classes.1

rm -f vm.omit vm.add tmp.omit tmp.awk

new=
if test -f ${top_builddir}/lib/classes.2; then
  p=`diff ${top_builddir}/lib/classes.2 ${top_builddir}/lib/classes.1`
  if test "$p" != ""; then
    new="true"
  fi
else
  new="true"
fi

if test "$new" = "true"; then
  cp ${top_builddir}/lib/classes.1 ${top_builddir}/lib/classes.2
  # Strip the package part.
  sed -e 's/^[^ ]* //' -e 's, ,/,' < ${top_builddir}/lib/classes.1 \
     > ${top_builddir}/lib/classes
  echo "JAVA_SRCS = \\" > ${top_builddir}/lib/java.dep
  for i in `cat ${top_builddir}/lib/classes` ; do
    echo $i "\\" >> ${top_builddir}/lib/java.dep
  done
fi

exit 0
