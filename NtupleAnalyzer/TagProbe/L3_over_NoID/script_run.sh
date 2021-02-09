#!/bin/bash

cd /home/kplee/Detector/MuonHLTTool/Run3_IDIso/NtupleAnalyzer/TagProbe/IDoverL3
root -l -b -q MakeHist_IDoverL3.cxx++ >&MakeHist_IDoverL3.log

echo "finished"
