#!bin/bash

root -l -b -q ROOTFile_MakeHist_SumWeight_ZMuMu_M50to120.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_ZMuMu_M120to200.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_ZMuMu_M200to400.root Print_SumWeight.cxx

root -l -b -q ROOTFile_MakeHist_SumWeight_ttbar.root Print_SumWeight.cxx

root -l -b -q ROOTFile_MakeHist_SumWeight_WJets.root Print_SumWeight.cxx

root -l -b -q ROOTFile_MakeHist_SumWeight_QCDMuEnriched_Pt20to30.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_QCDMuEnriched_Pt30to50.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_QCDMuEnriched_Pt50to80.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_QCDMuEnriched_Pt80to120.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_QCDMuEnriched_Pt120to170.root Print_SumWeight.cxx

echo "finished"