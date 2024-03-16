#!/bin/bash

# Use all .c files under SingleSource/UnitTests as possible target names.
# Try to make as many targets as possible.
# Should be run from the root source directory of llvm-testsuite.
# Build directory should be symlinked at ./build .

cd SingleSource/UnitTests

# Only C targets, excluding C++.
# Files containing float/double keywords also excluded.
all_targets=$(ls -l *.c | awk '{print $9}' | cut -d '.' -f1 | sort | uniq)
skip_targets=$(grep -nl 'double\|float' *.c  | cut -d '.' -f1 | sort | uniq)
targets=$(comm -13 <(echo "$skip_targets" ) <(echo "$all_targets"))

src_dir=$(pwd)
cd ../../build/SingleSource/UnitTests

n_total=0
n_build_fail=0
n_run_fail=0
n_run_succ=0

for target in $targets; do
    ((n_total+=1))
    echo Trying to build target: $target
    rc=0
    make $target 1>&2 || rc="$?"
    if [ $rc -eq 0 ]; then
        echo Target $target build succeeds

        out=$($DIR_DADAO_TOP/__install/bin/qemu-dadao $target)
        exit_line=$(printf "\nexit $?\n")
        out+="$exit_line"

        ref=$(cat $src_dir/$target.reference_output)
        diff_output=$(diff <(echo "$ref") <(echo "$out"))
        rc=$?
        echo $rc
        if [ $rc -eq 0 ]; then
            ((n_run_succ+=1))
            echo Target $target run succeeds
        else
            ((n_run_fail+=1))
            echo Target $target run fails, diff:
            echo $diff_output
        fi
    else
        ((n_build_fail+=1))
        echo Target $target build fails
    fi
done

echo "Summary:
# All Testcases : $n_total
# Build Failure : $n_build_fail
# Run Failure   : $n_run_fail
# Run Success   : $n_run_succ
"
