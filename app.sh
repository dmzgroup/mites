#!/bin/sh

. ../scripts/envsetup.sh

#export CYCLES_LOG=true
export MITES_WORKING_DIR="./"
$RUN_DEBUG$BIN_HOME/mites
