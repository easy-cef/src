#!/bin/bash

cd build
xcodebuild -scheme ALL_BUILD -configuration Debug build
cd ..
