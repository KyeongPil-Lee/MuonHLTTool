#!/bin/bash

hadd ROOTFile_Isolation_DY.root ROOTFile_Isolation_DY_*.root
hadd ROOTFile_Isolation_TT.root ROOTFile_Isolation_TT_*.root

mkdir splitJobs
mv ROOTFile_Isolation_DY_*.root splitJobs
mv ROOTFile_Isolation_TT_*.root splitJobs

echo "done"