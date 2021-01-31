#!/usr/bin/env bash

DIR="$(dirname "$(readlink -f "$0")")"
cd ${DIR}/../cmake-build-debug-host/CMakeFiles/zoal_tests.dir/tests
lcov -z -d . --rc lcov_branch_coverage=1