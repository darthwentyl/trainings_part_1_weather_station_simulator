#!/bin/bash

BUILD_DIR="build_coverage"
OUTPUT_STAT="coverage"
if [ -d ${BUILD_DIR} ]; then
    rm -rf ${BUILD_DIR}
fi
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=coverage -DUT=1 ..
NUMBER_OF_CPUS="$(nproc --all)"
make -j${NUMBER_OF_CPUS}
./test/weather_tests
find . -name '*.gcda' | xargs gcov -d
lcov --ignore mismatch --directory . --capture --output-file coverage.info
lcov --extract coverage.info '*/src/*' --output-file coverage.src-only.info
genhtml -o ${OUTPUT_STAT} coverage.src-only.info
firefox ${OUTPUT_STAT}/index.html &> /dev/null &
