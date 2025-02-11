#!/bin/sh
set -x

rm -rf pytest_c_testrunner
git submodule update --init --recursive
cd pytest_c_testrunner
rm -rf *
git clean -fdx
git reset --hard HEAD
git apply ../pytest_c_testrunner.patch
make
pytest
