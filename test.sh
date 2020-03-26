#!/bin/bash

set -e

REPO_PATH=$(dirname $(realpath $0))
EXT_PATH="${REPO_PATH}/build/src/py"


while [[ $# -gt 0 ]]; do
  key=$1
  case $key in
    -g|--gdb)
      shift
      GDB=1
      ;;
  esac
done


cd ${REPO_PATH}
export PYTHONPATH=${EXT_PATH}

if [[ -z ${GDB} ]]; then
  python3 -m unittest discover -p "*_test.py" -s tests
else
  gdb --args python3 -m unittest discover -p "*_test.py" -s tests
fi