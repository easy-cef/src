#!/bin/bash

# ensure build directory
if [ ! -e ./build ] ; then
  mkdir build
fi;

cd build

# config project with XCode
cmake -G "Xcode" ..
