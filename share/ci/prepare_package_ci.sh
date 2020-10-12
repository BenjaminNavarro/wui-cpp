#!/bin/bash

#########################
#  --  Git prepare  --  #
#########################
echo "--------------------------------------------------------------"
echo "----[PID] CI : preparing workspace ---------------------------"
echo "--------------------------------------------------------------"

# Print Git version
git --version
dir_path=`pwd`
dir_name=`basename $dir_path`

############################################################################################
#  --  initializing the folder where dependencies and installed artefacts will be put  --  #
############################################################################################

#creating the folder for binaries
if [ ! -d "./binaries" ]; then
  mkdir binaries
fi

echo "[PID] CI : initializing workspace ..."
#initializing the pid-workspace
if [ ! -d "./binaries/pid-workspace" ]; then
  cd binaries && git clone git@gite.lirmm.fr:pid/pid-workspace.git && cd pid-workspace/build && git checkout master && cmake -DIN_CI_PROCESS=ON -DFORCE_CONTRIBUTION_SPACE="$PACKAGE_CONTRIBUTION_SPACES" .. && cd ../../..
else
  cd binaries/pid-workspace/build && git pull -f official master && cmake -DIN_CI_PROCESS=ON -DFORCE_CONTRIBUTION_SPACE="$PACKAGE_CONTRIBUTION_SPACES" .. && cd ../../..
fi

# previous to an execution we need to set a link into the workspace that point to the current package
cd binaries/pid-workspace/packages && ln -s $dir_path $dir_name && cd ../../..


#getting the current platform and instance (if any) of the current runner
platform=$1

# 1 extract target platform information from runner targs
platform=${platform/pid/""}
platform=${platform/","/""}
platform=${platform/" "/""}
platform=${platform/site/""}
platform=${platform/","/""}
platform=${platform/" "/""}

# 2 separate platform and environment names
instance=""
reg_expr="^(.+)__(.+)__$"

if [[ $platform =~ $reg_expr ]]; then
    instance=${BASH_REMATCH[2]}
    platform=${BASH_REMATCH[1]}
fi

if [  "$instance" != "" ]; then
  using_profile=use_$1
  echo "[PID] CI : configuring environment $instance on platform $platform ..."
  cd binaries/pid-workspace/build
  cmd=mk profile=$using_profile env=$instance instance=$instance platform=$platform cmake --build . --target profiles
  CONFIGURE_RES=$?
  cd ../../..
  if [ $CONFIGURE_RES != 0 ]; then
    echo "--------------------------------------------------------------"
    echo "----[PID] CI : preparing workspace: FAIL (CONFIGURING) -------"
    echo "--------------------------------------------------------------"
    exit $CONFIGURE_RES
  fi
fi

echo "--------------------------------------------------------------"
echo "----[PID] CI : preparing workspace: DONE ---------------------"
echo "--------------------------------------------------------------"
