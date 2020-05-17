#!/bin/bash

# ensure build directory
if [ ! -e ./build ] ; then
  mkdir build
fi;

cd build

if [ "$(uname -s)" == "Darwin" ] ; then
  # config project with XCode
  echo "*** Config build as Darwin ... ***"
  cmake -G "Xcode" ..
else
  # config project with g++
  echo "*** Config build as Linux ... ***"
  cmake -G "Unix Makefiles" ..
fi;
