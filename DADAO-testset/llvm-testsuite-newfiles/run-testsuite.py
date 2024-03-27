#!/usr/bin/python3

import os
import subprocess
from subprocess import PIPE
import tempfile

# Use all .c files under SingleSource/UnitTests as possible target names.
# Try to make as many targets as possible.
# Should be run from the root source directory of llvm-testsuite.
# Build directory should be symlinked at ./build .

def get_command_stdout(command, checked=False):
    r = subprocess.run(command, shell=True, executable="/bin/bash", stdout=PIPE)
    if checked:
        r.check_returncode()
    return r.stdout.decode(), r.returncode

def run_command(command, checked=False):
    r = subprocess.run(command, shell=True, executable="/bin/bash")
    if checked:
        r.check_returncode()
    return r.stdout.decode(), r.returncode

os.chdir("SingleSource/UnitTests")

# Only C targets, excluding C++.
# Files containing float/double keywords also excluded.
# Files related to ms_struct excluded.

# all_targets=$(ls -l *.c | awk '{print $9}' | cut -d '.' -f1 | sort | uniq)
# skip_targets=$(grep -nl 'double\|float' *.c  | cut -d '.' -f1 | sort | uniq)
# targets=$(comm -13 <(echo "$skip_targets" ) <(echo "$all_targets"))

# 2006-12-01-float_varg will cause qemu to run infinitely.
all_targets, _ = get_command_stdout("ls -l *.c | awk '{print $9}' | cut -d '.' -f1 | sort | uniq | grep -v ms_struct | grep -v 2006-12-01-float_varg", checked=True)
skip_targets, _ = get_command_stdout("grep -nl 'double\|float' *.c  | cut -d '.' -f1 | sort | uniq", checked=True)
targets, _ = get_command_stdout('comm -13 <(echo "{}" ) <(echo "{}")'.format(skip_targets.strip(), all_targets.strip()), checked=True)

# src_dir=$(pwd)
# cd ../../build/SingleSource/UnitTests

src_dir = os.curdir
os.chdir("../../build/SingleSource/UnitTests")

n_total=0
n_build_fail=0
n_run_fail=0
n_run_succ=0

for target in targets.split():
    n_total += 1
    print("Trying to build target: {}".format(target), flush=True)
    out, rc = get_command_stdout("make {} 1>&2".format(target))
    if rc == 0:
        print("Target {} build succeeds".format(target))

        try:
            out, rc = get_command_stdout("$DIR_DADAO_TOP/__install/bin/qemu-dadao {}".format(target))
            out += "exit {}\n".format(rc)
        except Exception as error:
            n_run_fail += 1
            print("Target {} run fails, an exception occurred: ".format(target), error)
            continue
        
        with tempfile.NamedTemporaryFile(delete=False) as fp:
            filename = fp.name
            fp.write(out.encode())
            fp.close()
            diff_output, rc = get_command_stdout('diff {}/{}.reference_output {}'.format(src_dir, target, filename))
        print(rc)
        if rc == 0:
            n_run_succ += 1
            print("Target {} run succeeds".format(target))
        else:
            n_run_fail += 1
            print("Target {} run fails, diff:".format(target))
            print(diff_output)
    else:
        n_build_fail += 1
        print("Target {} build fails".format(target))

print("All Testcases : {}\nBuild Failure : {}\nRun Failure : {}\nRun Success : {}"
      .format(n_total, n_build_fail, n_run_fail, n_run_succ))

# for target in $targets; do
#     ((n_total+=1))
#     echo Trying to build target: $target
#     rc=0
#     make $target 1>&2 || rc="$?"
#     if [ $rc -eq 0 ]; then
#         echo Target $target build succeeds

#         out=$($DIR_DADAO_TOP/__install/bin/qemu-dadao $target)
#         exit_line=$(printf "\nexit $?\n")
#         out+="$exit_line"

#         ref=$(cat $src_dir/$target.reference_output)
#         diff_output=$(diff -u --strip-trailing-cr <(echo "$ref") <(echo "$out"))
#         rc=$?
#         echo $rc
#         if [ $rc -eq 0 ]; then
#             ((n_run_succ+=1))
#             echo Target $target run succeeds
#         else
#             ((n_run_fail+=1))
#             echo Target $target run fails, diff:
#             echo "$diff_output"
#         fi
#     else
#         ((n_build_fail+=1))
#         echo Target $target build fails
#     fi
# done

# echo "Summary:
# All Testcases : $n_total
# Build Failure : $n_build_fail
# Run Failure   : $n_run_fail
# Run Success   : $n_run_succ
# "
