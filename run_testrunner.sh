#!/bin/sh
set -x

TESTRUNNER="pytest_c_testrunner"
TESTRUNNER_PATH="${TESTRUNNER}.patch"

rm -rf ${TESTRUNNER}
git submodule update --init --recursive
cd ${TESTRUNNER}
rm -rf *
git clean -fdx
git reset --hard HEAD
git apply ../${TESTRUNNER_PATH}
make
pytest

