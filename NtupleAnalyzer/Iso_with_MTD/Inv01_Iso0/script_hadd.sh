#!/bin/bash

hadd ROOTFile_DY.root ROOTFile_DY_*.root
hadd ROOTFile_TT.root ROOTFile_TT_*.root

mkdir splitJobs
mv ROOTFile_DY_*.root splitJobs
mv ROOTFile_TT_*.root splitJobs

echo "done"