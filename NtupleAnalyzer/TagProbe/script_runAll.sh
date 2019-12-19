#!/bin/bash

cd IDoverL1/N
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_layer
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_match
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_TMOST
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_nTrackerLayer
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_nPixelLayer
root -l -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../../L3overL1
root -l -b -q MakeHist_L3overL1.cxx++ >&MakeHist_L3overL1.log&

cd ../NoIDoverL1
root -l -b -q MakeHist_NoIDoverL1.cxx++ >&MakeHist_L3overL1.log&

echo "submission is done"
