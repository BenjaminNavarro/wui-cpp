#!/bin/bash

echo "--------------------------------------------------------------"
echo "----[PID] CI : building the project --------------------------"
echo "--------------------------------------------------------------"

force=true cmake --build . --target build

BUILD_RES=$?

if [ $BUILD_RES != 0 ]; then
  echo "--------------------------------------------------------------"
  echo "----[PID] CI : building the project: FAIL --------------------"
  echo "--------------------------------------------------------------"
  exit $BUILD_RES
fi

echo "--------------------------------------------------------------"
echo "----[PID] CI : building the project: DONE --------------------"
echo "--------------------------------------------------------------"
