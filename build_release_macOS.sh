#!/bin/bash

cd build
xcodebuild -scheme ALL_BUILD -configuration Release build
cd ..
