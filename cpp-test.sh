#!/use/bin/bash

CXX="c++"
CXXFLAGS="-std=c++1z -Wall -Iinclude"
APP="build/`basename $1 | sed 's/\.[^\.]*$//'`.test"
BUILD="$CXX $CXXFLAGS $1 -o $APP"

if ! [ -d build ]; then
    mkdir build
fi

$CXX $CXXFLAGS $1 -o $APP
$APP

