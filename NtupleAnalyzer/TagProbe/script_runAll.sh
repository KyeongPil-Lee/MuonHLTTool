#!/bin/bash

cd IDoverL1/N
root -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_layer
root -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_match
root -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../N_minus_TMOST
root -b -q MakeHist_MuonIDoverL1.cxx++ >&MakeHist_MuonIDoverL1.log&

cd ../../L3overL1
root -b -q MakeHist_L3overL1.cxx++ >&MakeHist_L3overL1.log&

cd ../NoIDoverL1
root -b -q MakeHist_NoIDoverL1.cxx++ >&MakeHist_L3overL1.log&

echo "submission is done"
