#!/bin/bash

trigger=$1
dataset=$2
version=$3
etaMin=$4
etaMax=$5
JOBID=$6
list_ntuplePath=$7

echo '------------------------------------------------------'
echo 'Arg: trigger                      '$trigger
echo 'Arg: dataset                      '$dataset
echo 'Arg: version                      '$version
echo 'Arg: etaMin                       '$etaMin
echo 'Arg: etaMax                       '$etaMax
echo 'Arg: JOBID                        '$JOBID
echo 'Arg: list_ntuplePath'
echo $list_ntuplePath
echo '------------------------------------------------------'


cwd=$(pwd)

source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_6
eval `scramv1 runtime -sh`
echo ""

cd $cwd

root -l -b -q 'MuonTriggerHist.cxx+("'$trigger'",    "'$dataset'",  "'$version'", 0.0, 5.0, "'$JOBID'",'$list_ntuplePath')'

echo "finished"
