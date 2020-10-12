#!/bin/bash

echo "--------------------------------------------------------------"
echo "----[PID] CI : configuring the project -----------------------"
echo "--------------------------------------------------------------"

#NB: never building API doc not binaries on integration, but always geberating developpers reports
if [ "$PACKAGE_HAS_LIBRARIES" = true ] ; then
  if [ "$PACKAGE_HAS_TESTS" = true ] ; then
     if [ "$PACKAGE_HAS_EXAMPLES" = true ] ; then
        cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DADDITIONAL_DEBUG_INFO=ON -DBUILD_AND_RUN_TESTS=ON -DENABLE_SANITIZERS=ON -DBUILD_TESTS_IN_DEBUG=ON -DBUILD_COVERAGE_REPORT=ON -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=ON -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DWORKSPACE_DIR="../binaries/pid-workspace" ..
     else
        cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DADDITIONAL_DEBUG_INFO=ON -DBUILD_AND_RUN_TESTS=ON -DENABLE_SANITIZERS=ON -DBUILD_TESTS_IN_DEBUG=ON -DBUILD_COVERAGE_REPORT=ON -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DWORKSPACE_DIR="../binaries/pid-workspace" ..
     fi
  else
      if [ "$PACKAGE_HAS_EXAMPLES" = true ] ; then
        cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DBUILD_AND_RUN_TESTS=OFF -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=ON -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DADDITIONAL_DEBUG_INFO=ON -DWORKSPACE_DIR="../binaries/pid-workspace" ..
      else
        cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DBUILD_AND_RUN_TESTS=OFF -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DADDITIONAL_DEBUG_INFO=ON -DWORKSPACE_DIR="../binaries/pid-workspace" ..
      fi
  fi
else  # no libraries => this is a pure applicative package, no examples since no lib
    if [ "$PACKAGE_HAS_TESTS" = true ] ; then
      cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DBUILD_AND_RUN_TESTS=ON -DENABLE_SANITIZERS=ON -DBUILD_TESTS_IN_DEBUG=ON -DBUILD_COVERAGE_REPORT=ON -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DADDITIONAL_DEBUG_INFO=OFF -DWORKSPACE_DIR="../binaries/pid-workspace" ..
    else
      cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DBUILD_AND_RUN_TESTS=OFF -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=ON -DADDITIONAL_DEBUG_INFO=OFF -DWORKSPACE_DIR="../binaries/pid-workspace" ..
    fi
fi
CONF_RES=$?

if [ $CONF_RES != 0 ]; then
  echo "--------------------------------------------------------------"
  echo "----[PID] CI : configuring the project: FAIL -----------------"
  echo "--------------------------------------------------------------"
  exit $CONF_RES
fi
# always generating the dependencies file of the package
write_file=true cmake --build . --target list_dependencies

echo "--------------------------------------------------------------"
echo "----[PID] CI : configuring the project: DONE -----------------"
echo "--------------------------------------------------------------"
