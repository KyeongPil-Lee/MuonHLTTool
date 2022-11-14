#!/bin/bash

if [ $MUONHLT_ANALYZER_PATH ]; then
    echo "MUONHLT_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

export MUONHLT_ANALYZER_PATH=$(pwd)

if [[ "$HOSTNAME" == *iihe.ac.be ]]; then
  echo "IIHE T2: setup LCG100 Env."
  source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_100 x86_64-centos7-gcc10-opt
  export LD_PRELOAD=/usr/lib64/libopenblas.so.0
fi

# -- root setup -- #
export ROOT_INCLUDE_PATH=${MUONHLT_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}
export PYTHONPATH=${MUONHLT_ANALYZER_PATH}:${PYTHONPATH}

echo "Enviornment variables"
echo "MUONHLT_ANALYZER_PATH: "$MUONHLT_ANALYZER_PATH
echo "ROOT_INCLUDE_PATH:     "$ROOT_INCLUDE_PATH
echo "PYTHONPATH:            "$PYTHONPATH
echo "Setup for Muon HLT analyzer is done."