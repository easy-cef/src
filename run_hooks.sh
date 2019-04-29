#!/bin/bash

# ensure build directory
if [ ! -e ./build ] ; then
  mkdir build
fi;

cd build

if [ "$(uname -s)" == "Darwin" ] ; then
  # config project with XCode
  echo "*** nConfig build as Darwin ... ***"
  cmake -G "Xcode" ..
else
  # config project with g++
  echo "*** nConfig build as Linux ... ***"
  cmake -G "Unix Makefiles" ..
fi;
