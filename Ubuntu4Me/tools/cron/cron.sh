#!/bin/bash

# git back-up
GITDIR=/home/kavonm/opens/WorkNotes/Ubuntu4Me

cp ~/.bashrc $GITDIR/env/bashrc
cp ~/.bash_aliases $GITDIR/env/bash_aliases
cp ~/.hosts $GITDIR/env/hosts
cp ~/.tmux.conf $GITDIR/env/dot.tmux.conf
cp ~/.vimrc $GITDIR/tools/vimrc

cp ~/bin/to $GITDIR/tools/to
cp ~/bin/build $GITDIR/tools/build
cp ~/bin/quick_ftp $GITDIR/tools/quick_ftp

# back-up myself
cp -Rp ~/bin/cron/ $GITDIR/tools/

# remove .local
if [ -d ~/.local ]; then
    rm -rf ~/.local
fi
