#!/bin/sh

metapath=$(readlink -f $(dirname "$0"))
srcpath=$(dirname "$metapath")
toppath=$(dirname "$srcpath")

source "$metapath/include.sh"
init_distpath "$1" "$toppath/protocon-bin"

mkdir -p "$distpath/bin"
cp -a -t "$distpath/bin" "$toppath/bin/protocon" "$toppath/bin/protocon-gui" "$toppath/bin/protocon-mpi"

copy_examples

make_doc "$srcpath"
cp -a -t "$distpath" "$srcpath/doc"
rm -fr "$distpath/doc/webtex"
rm -f "$distpath/doc/Makefile"

zip_up

