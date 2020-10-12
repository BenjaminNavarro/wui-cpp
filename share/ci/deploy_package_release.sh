#!/bin/bash

if [ "$PACKAGE_HAS_SITE" = true ] ; then

  for a_tag in $@
  do
    if [ $a_tag != "pid" ] && [ $a_tag != "pid," ]
    then
      platform=${a_tag/","/""}
    fi
  done

  echo "--------------------------------------------------------------"
  echo "----[PID] CI : deploying the project -------------------------"
  echo "--------------------------------------------------------------"
  site_publish_coverage=OFF
  site_publish_static_checks=OFF
  site_publish_api=OFF
  runner_only_binaries="true"
  if [ "$platform" = "$PACKAGE_MAIN_PLATFORM" ]; then
    # the current runner is in charge of generating the static site
    runner_only_binaries="false"

    # managing publication of developpers info
    if [ "$PACKAGE_DEV_INFO_PUBLISHED" = true ]; then
      site_publish_static_checks=ON
      if [ "$PACKAGE_HAS_TESTS" = true ] ; then
        site_publish_coverage=ON
      fi
    fi

    # publishing API doc as soon as there are libraries
    if [ "$PACKAGE_HAS_LIBRARIES" = true ]; then
      site_publish_api=ON
    fi
  fi


  BIN_ARCH_RES=0
  # 1) generating the binary archives (if necessary),this step is separate from following to avoid including in binary archive unecessary developper infos
  if [ "$PACKAGE_BINARIES_PUBLISHED" = true ]; then
    echo "[PID] CI : generating the binary archive..."
    cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DADDITIONAL_DEBUG_INFO=OFF -DBUILD_AND_RUN_TESTS=OFF -DENABLE_SANITIZERS=OFF -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=OFF -DBUILD_STATIC_CODE_CHECKING_REPORT=OFF -DGENERATE_INSTALLER=ON ..
    #build the package to get clean binary archives (without dev info)
    force=true cmake --build . --target build
    BIN_ARCH_RES=$?
  elif [ "$runner_only_binaries" = true ]; then
    #the runner does not publish pages, nor binaries !! => nothing to do
    echo "--------------------------------------------------------------"
    echo "----[PID] CI : deploying the project: SKIPPED ----------------"
    echo "--------------------------------------------------------------"
    exit 0
  fi

  # 2) configuring the package adequately to make it generate other artefacts included in the static site (API doc for instance)
  echo "[PID] CI : generating the static site..."
  cmake -DREQUIRED_PACKAGES_AUTOMATIC_DOWNLOAD=ON -DADDITIONAL_DEBUG_INFO=OFF -DBUILD_AND_RUN_TESTS=$site_publish_coverage -DBUILD_TESTS_IN_DEBUG=$site_publish_coverage -DBUILD_COVERAGE_REPORT=$site_publish_coverage -DENABLE_PARALLEL_BUILD=ON -DBUILD_EXAMPLES=OFF -DBUILD_API_DOC=$site_publish_api -DBUILD_STATIC_CODE_CHECKING_REPORT=$site_publish_static_checks -DGENERATE_INSTALLER=OFF ..
  #build the package again to get all dev infos available
  force=true cmake --build . --target build
  BIN_FULL_INFO_RES=$?
  #build the static site
  only_binaries=$runner_only_binaries cmake --build . --target site

  SITE_RES=$?
  if [ $SITE_RES != 0 ]; then
    echo "--------------------------------------------------------------"
    echo "----[PID] CI : deploying the project: FAIL (STATIC SITE) -----"
    echo "--------------------------------------------------------------"
  	exit $SITE_RES
  fi

  if [ $BIN_FULL_INFO_RES != 0 ]; then
    echo "--------------------------------------------------------------"
    echo "----[PID] CI : deploying the project: FAIL (DEV INFO) --------"
    echo "--------------------------------------------------------------"
  	exit $BIN_FULL_INFO_RES
  fi

  if [ $BIN_ARCH_RES != 0 ]; then
    echo "--------------------------------------------------------------"
    echo "----[PID] CI : deploying the project: FAIL (BIN ARCHIVE) -----"
    echo "--------------------------------------------------------------"
    exit $BIN_ARCH_RES
  fi
  echo "--------------------------------------------------------------"
  echo "----[PID] CI : deploying the project: DONE -------------------"
  echo "--------------------------------------------------------------"
fi

#if no site to publish then nothing to do
