#!/bin/bash

hadd ROOTFile_IsoScan_MuonTime_DY.root ROOTFile_IsoScan_MuonTime_DY_*.root
hadd ROOTFile_IsoScan_MuonTime_TT.root ROOTFile_IsoScan_MuonTime_TT_*.root

mkdir splitJobs
mv ROOTFile_IsoScan_MuonTime_DY_*.root splitJobs
mv ROOTFile_IsoScan_MuonTime_TT_*.root splitJobs

echo "done"