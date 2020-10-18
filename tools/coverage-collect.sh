#!/usr/bin/env bash
pwd
cd ./cmake-build-debug-host/CMakeFiles/zoal_tests_atmega328.dir/tests
pwd

lcov --capture --directory . --output-file coverage_full.info
lcov --remove coverage_full.info \
    '/usr/include/*' '*/gtest/*' '*/tests/*' '*/v1/*' \
    --output-file coverage.info

genhtml coverage.info -o ../../../../coverage

# lcov -z -d .
