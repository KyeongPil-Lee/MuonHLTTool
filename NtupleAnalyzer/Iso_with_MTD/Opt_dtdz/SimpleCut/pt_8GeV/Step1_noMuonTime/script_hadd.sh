#!/bin/bash

hadd ROOTFile_IsoScan_NoMuonTime_DY.root ROOTFile_IsoScan_NoMuonTime_DY_*.root
hadd ROOTFile_IsoScan_NoMuonTime_TT.root ROOTFile_IsoScan_NoMuonTime_TT_*.root

mkdir splitJobs
mv ROOTFile_IsoScan_NoMuonTime_DY_*.root splitJobs
mv ROOTFile_IsoScan_NoMuonTime_TT_*.root splitJobs

echo "done"