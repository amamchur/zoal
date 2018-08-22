#!/usr/bin/env bash

DIR="$(dirname "$(readlink -f "$0")")"
cd ${DIR}/../cmake-build-debug/CMakeFiles/TestMCURDK.dir/tests
lcov -z -d . --rc lcov_branch_coverage=1