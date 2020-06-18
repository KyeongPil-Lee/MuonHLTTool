#!/bin/bash

rsync -avz --progress -e 'ssh -p 1240' \
/u/user/kplee/SE_UserHome/HLTPhysics/crab_MuonHLTNtuple_v20200615_HLTPhysics_Run2018D_Rerun/200615_135223/*/*.root \
kplee@147.47.242.67:/gv0/Users/kplee/MUO19001/HLTPhysicsRun2018D

echo "Run2018D: finished"

rsync -avz --progress -e 'ssh -p 1240' \
/u/user/kplee/SE_UserHome/HLTPhysics/crab_MuonHLTNtuple_v20200617_HLTPhysics_Run2016Hv2_Rerun/200617_090302/*/*.root \
kplee@147.47.242.67:/gv0/Users/kplee/MUO19001/HLTPhysicsRun2016Hv2

echo "Run2016Hv2: finished"

rsync -avz --progress -e 'ssh -p 1240' \
/u/user/kplee/SE_UserHome/HLTPhysics/crab_MuonHLTNtuple_v20200617_HLTPhysics_Run2016Hv3_Rerun/200617_090331/*/*.root \
kplee@147.47.242.67:/gv0/Users/kplee/MUO19001/HLTPhysicsRun2016Hv3

echo "Run2016Hv3: finished"



