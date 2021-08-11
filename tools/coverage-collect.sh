#!/usr/bin/env bash
pwd
cd ./cmake-build-debug-host/CMakeFiles/zoal_tests_atmega328.dir/tests
pwd

lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage_full.info
lcov --rc lcov_branch_coverage=1 --remove coverage_full.info \
    '/usr/include/*' '*/msys64/*' '*/gtest/*' '*/tests/*' '*/v1/*' \
    --output-file coverage.info

genhtml coverage.info --rc lcov_branch_coverage=1 -o ./coverage

# lcov -z -d .
