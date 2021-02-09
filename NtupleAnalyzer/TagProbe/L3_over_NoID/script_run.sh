#!/bin/bash

cd /home/kplee/Detector/MuonHLTTool/Run3_IDIso/NtupleAnalyzer/TagProbe/L3_over_NoID
root -l -b -q MakeHist_L3overNoID.cxx++ >&MakeHist_L3overNoID.log

echo "finished"
