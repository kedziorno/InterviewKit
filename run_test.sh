#!/bin/sh
set -x

./run_testrunner.sh $@

TESTRUNNER="pytest_c_testrunner"

rm -rf build
mkdir build
cd build
cmake ..
make
cp ../${TESTRUNNER}/conftest.py conftest.py.new
sed -i 's/if path.ext == ".c" and path.basename.startswith("test_"):/if path.basename.startswith("InterviewKit"):/g' conftest.py.new
grep "InterviewKit" conftest.py.new
ln -s conftest.py.new conftest.py
pytest $@ .

