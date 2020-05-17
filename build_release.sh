#!/bin/bash

cd build

if [ "$(uname -s)" == "Darwin" ] ; then
  # build with XCode
  echo "*** Build on Darwin ... ***"
  xcodebuild -scheme ALL_BUILD -configuration Release build
else
  # build with g++
  echo "*** Build on Linux ... ***"
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
  make -j4
fi;

cd ..
