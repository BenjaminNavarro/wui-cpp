#!/bin/bash

echo "--------------------------------------------------------------"
echo "----[PID] CI : configuring the project -----------------------"
echo "--------------------------------------------------------------"

#first time configuring with tests and examples enabled
cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DADDITIONAL_DEBUG_INFO=OFF -DBUILD_AND_RUN_TESTS=ON -DENABLE_SANITIZERS=ON -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=ON -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=OFF -DGENERATE_INSTALLER=OFF -DWORKSPACE_DIR="../binaries/pid-workspace" ..
CONF_RES=$?

if [ $CONF_RES != 0 ]; then
  echo "--------------------------------------------------------------"
  echo "----[PID] CI : configuring the project: FAIL -----------------"
  echo "--------------------------------------------------------------"
  exit $CONF_RES
fi

# always generating the dependencies file of the package
write_file=true cmake --build . --target list_dependencies

#put the dependencies description file directly in build folder
mv ./release/share/dependencies.txt ..

echo "--------------------------------------------------------------"
echo "----[PID] CI : configuring the project: DONE -----------------"
echo "--------------------------------------------------------------"
