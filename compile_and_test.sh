#!/bin/bash
set -e
build_dir="cmake-build-debug"
target="hyriseTest"
test_command=$target
cd $build_dir
make $target
cd ".."
./$build_dir/$test_command