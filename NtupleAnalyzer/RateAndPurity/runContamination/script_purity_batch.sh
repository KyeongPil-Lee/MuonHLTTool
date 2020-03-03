#!/bin/bash

DATE=`date +%Y%m%d`
echo "$DATE"

Dataset=$1
TRIG=$2
VER=$3
JOBID=$4
DATASET=$5

echo '------------------------------------------------------'
echo 'Arg: TRIG                         '$TRIG
echo 'Arg: Dataset                      '$Dataset
echo 'Arg: VER                          '$VER
echo 'Arg: JOBID                        '$JOBID
echo 'Arg: DATASET'
echo $DATASET
echo '------------------------------------------------------'
echo 'PBS: qsub is running on           '$PBS_O_HOST
echo 'PBS: originating queue is         '$PBS_O_QUEUE
echo 'PBS: executing queue is           '$PBS_QUEUE
echo 'PBS: working directory is         '$PBS_O_WORKDIR
echo 'PBS: execution mode is            '$PBS_ENVIRONMENT
echo 'PBS: job identifier is            '$PBS_JOBID
echo 'PBS: job name is                  '$PBS_JOBNAME
echo 'PBS: current home directory is    '$PBS_O_HOME
echo '------------------------------------------------------'

#-- PBS parameters
#PBS -l walltime=48:00:00,cput=48:00:00
#PBS -V
#PBS -o /u/user/msoh/MuonHLT/MuonHLTPaper/CMSSW_10_2_6/src/MuonHLTTool/NtupleAnalyzer/RateAndPurity/BatchLogs/Purity_$VER_$DATE
#PBS -e /u/user/msoh/MuonHLT/MuonHLTPaper/CMSSW_10_2_6/src/MuonHLTTool/NtupleAnalyzer/RateAndPurity/BatchLogs/Purity_$VER_$DATE
#PBS -k o
#PBS -j oe

source /cvmfs/cms.cern.ch/cmsset_default.sh
# cd /u/user/msoh/MuonHLT/MuonHLTPaper/CMSSW_10_2_6/src
cd /d0/scratch/kplee/MuonPOGTnP/CMSSW_10_2_5/src
eval `scramv1 runtime -sh`
which cmsRun
which root
echo ""

cd $PBS_O_WORKDIR

# export MUONHLT_ANALYZER_PATH=/u/user/msoh/MuonHLT/MuonHLTPaper/CMSSW_10_2_6/src/MuonHLTTool/NtupleAnalyzer
export MUONHLT_ANALYZER_PATH=/u/user/kplee/scratch/ServiceWork/MuonHLTTool/RateAndPurity/NtupleAnalyzer
export ROOT_INCLUDE_PATH=${MUONHLT_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}
export PYTHONPATH=${MUONHLT_ANALYZER_PATH}:${PYTHONPATH}

# echo "run MuonTriggerPurity.cxx"
echo 'gROOT->LoadMacro("MuonTriggerPurity.cxx+"); gSystem->Exit(0);' | root -b -l

root -l -b -q 'MuonTriggerPurity.cxx+("'$TRIG'",    "'$Dataset'",  "'$VER'", 0.0, 5.0, "'$JOBID'",'$DATASET')'

# root -l -b -q 'MuonTriggerPurity.cxx+("'$TRIG'",    "'$Dataset'",  "'$VER'", 0.0, 0.3, "'$JOBID'",'$DATASET')'

# root -l -b -q 'MuonTriggerPurity.cxx+("'$TRIG'",    "'$Dataset'",  "'$VER'", 0.3, 1.2, "'$JOBID'",'$DATASET')'

# root -l -b -q 'MuonTriggerPurity.cxx+("'$TRIG'",    "'$Dataset'",  "'$VER'", 1.2, 2.1, "'$JOBID'",'$DATASET')'

# root -l -b -q 'MuonTriggerPurity.cxx+("'$TRIG'",    "'$Dataset'",  "'$VER'", 2.1, 2.4, "'$JOBID'",'$DATASET')'

