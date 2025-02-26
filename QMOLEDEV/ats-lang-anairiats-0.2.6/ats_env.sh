#!/bin/sh

#
# Author: Likai Liu (likai AT cs DOT bu DOT edu)
#

PACKAGE_TARNAME=ats-anairiats
PACKAGE_VERSION=0.2.6
prefix=/usr/local

export ATSHOME="${prefix}/lib/${PACKAGE_TARNAME}-${PACKAGE_VERSION}"
export ATSHOMERELOC="ATS-${PACKAGE_VERSION}"

if [ ! -d "$ATSHOME" ]; then
  echo "ATS must be installed at '$ATSHOME'"
  exit 1
fi

prog=`basename $0`

case $prog in
  ats_env.sh)
    echo "This is a wrapper script for atscc, atslib, and atsopt."
    echo "The script should be symbolic linked from these aliases."
    exit 1
    ;;
  *)
    exec "$ATSHOME/bin/$prog" "$@"
    ;;
esac

#
# end of [ats_env.sh.in]
#
