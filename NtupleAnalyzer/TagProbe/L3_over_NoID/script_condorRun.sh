#!/bin/bash

fileName_ntuplePathList=$1

root -l -b -q 'MakeHist_L3overNoID.cxx++("'$fileName_ntuplePathList'")'

echo "finished"

