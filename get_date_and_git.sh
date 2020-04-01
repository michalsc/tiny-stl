#!/bin/sh

>&2 echo `pwd`

BUILD_DATE=`date +"%d.%m.%Y"`
GIT_HEAD=`git rev-parse --short HEAD`

if [ -n "$(git status --porcelain)" ]; then
VERSION_STRING_DATE="($BUILD_DATE) git: $GIT_HEAD,dirty"
else
VERSION_STRING_DATE="($BUILD_DATE) git: $GIT_HEAD"
fi

echo $VERSION_STRING_DATE
