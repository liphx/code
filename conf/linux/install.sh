#!/bin/bash

mkdir -p $HOME/usr/src
cd $HOME/usr/src

curl -O https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.2.tar.gz
curl -O https://ftp.nluug.nl/pub/vim/unix/vim-8.2.tar.bz2
curl -O http://ftp.gnu.org/gnu/bash/bash-5.1.8.tar.gz
curl -O https://www.python.org/ftp/python/3.9.7/Python-3.9.7.tgz
curl -O http://prdownloads.sourceforge.net/ctags/ctags-5.8.tar.gz

find . -maxdepth 1 -type f -name '*.tar.gz' -exec tar xvf {} \;
find . -maxdepth 1 -type f -name '*.tgz' -exec tar xvf {} \;
dirs=$(find . -maxdepth 1 -type d)
for dir in $dirs; do
    pushd $dir
    env CPPFLAGS="-I/$HOME/usr/include" LDFLAGS="-L/$HOME/usr/lib" ./configure --prefix="$HOME/usr"
    make && make install
    popd
done

###################
# package   options
###################
# python3   --enable-shared --with-ssl --enable-optimizations
# vim       --enable-python3interp=yes
###################
