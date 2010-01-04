#!/bin/sh

. ../scripts/envsetup.sh

$RUN_DEBUG$BIN_HOME/dmzAppQt -f config/runtime.xml config/common.xml config/arena.xml config/canvas.xml config/lua.xml config/input.xml config/version.xml config/resource.xml $*
