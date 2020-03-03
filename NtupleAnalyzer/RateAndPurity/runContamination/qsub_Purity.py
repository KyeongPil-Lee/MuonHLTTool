#!/usr/bin/env python3
import sys
import os
import time
import glob
import subprocess
from shutil import copyfile
import gc

def jobSpritting( dataset, paths, nfiles ):
  str_dcap  = "dcap://cluster142.knu.ac.kr/"
  # base_path = "/pnfs/knu.ac.kr/data/cms/store/user/jskim/"
  base_path = "/pnfs/knu.ac.kr/data/cms/store/user/moh/"
  if "MUOVal" in dataset:
    base_path = "/pnfs/knu.ac.kr/data/cms/store/user/jskim/"

  out = []

  lines = glob.glob(base_path+paths+"/ntuple_*.root")

  n = 0
  files = ''
  for i, line in enumerate(lines):
    if 'ntuple_' not in line:
      continue
    if '.root' not in line:
      continue

    files += '\\"%s%s\\",' % (str_dcap, line)

    if i == nfiles*(n+1) - 1:
      filesout = '\'{'+files+'}\''
      filesout = filesout.replace(',}', '}')
      out.append( ( n, filesout) )
      n = n+1
      files = ''

    if i == len(lines)-1 and files != '':
      filesout = '\'{'+files+'}\''
      filesout = filesout.replace(',}', '}')
      out.append( ( n, filesout) )

  return out




VER = "v101"
Datasets = [  # HERE
  # ('TTSemiLep80X',  'TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2016_TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/191124_074512/0000/', 1),
  ('TTSemiLep102X',       'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/191124_074614/0000/', 5),
  ('TTSemiLep102XMUOVal', 'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_MuonHLTNtuple_SimMatchingInfo_2018_TTToSemiLeptonic__PUAvg50ForMUOVal/200227_162718/0000/', 40)
]
Triggers = [  # HERE
  'hltL1fL1sMu22L1Filtered0',
  'hltL2fL1sMu22L1f0L2Filtered10Q', # -- it will automatically changed to 102X version

  'HLT_IsoMu24_v',
  'HLT_Mu50_v',
  'HLT_Mu24_v',

  # 'HLT_IsoTkMu24_v',
  # 'HLT_TkMu50_v',
  # 'HLT_TkMu24_v'
]
script = "script_purity_batch.sh"
macro  = "MuonTriggerPurity.cxx"

PWD = os.getcwd()
# scratch = "/d3/scratch/msoh"
scratch = "/d0/scratch/kplee"

WorkingDir = scratch+"/BatchDir_"+VER
if not os.path.isdir(WorkingDir):
  os.makedirs(WorkingDir)
  print WorkingDir, '  --> created'
else:
  print WorkingDir, '  --> already exists'
  sys.exit()

LogDir    = PWD+"/Logs_"+VER
if not os.path.isdir(LogDir):
  os.makedirs(LogDir)
  print LogDir, '  --> created'
else:
  print LogDir, '  --> already exists'

while not os.path.isdir(WorkingDir):
  time.sleep(0.2)
while not os.path.isdir(LogDir):
  time.sleep(0.2)

for dataset, paths, nfiles in Datasets:

  jobid_files = jobSpritting(dataset, paths, nfiles)
  print "Dataset:  ", dataset
  print "\t --> # jobs:  ", len(jobid_files) * len(Triggers)

  for trig in Triggers:

    if( "80X" not in dataset and "TkMu" in trig ):
      continue

    if "102X" in dataset and trig == "hltL2fL1sMu22L1f0L2Filtered10Q":
      trig = "hltL2fL1sSingleMu22L1f0L2Filtered10Q"


    for jobid, files in jobid_files:
      strJobId = 'Job'+str(jobid)

      # HERE
      # if(jobid>0):
      #   break

      jodDir = WorkingDir+'/'+dataset+'/'+trig+'/Job'+str(jobid)
      os.makedirs(jodDir) # , exist_ok=True)
      copyfile( PWD+'/'+macro, jodDir+'/'+macro)
      copyfile( PWD+'/'+script, jodDir+'/'+script)
      os.chdir(jodDir)

      while not os.path.isfile(jodDir+'/'+macro):
        time.sleep(0.1)
      while not os.path.isfile(jodDir+'/'+script):
        time.sleep(0.1)

      name = 'Purity_'+dataset+'_'+trig+'--'+strJobId
      # cmd = 'qsub  -q cms  -V  -o %s -e %s  -j oe  -l walltime=48:00:00,cput=48:00:00  -N %s  -F "%s %s %s %s %s"  %s' \
      cmd = 'qsub  -q cms  -o %s -e %s  -j oe  -l walltime=48:00:00,cput=48:00:00  -N %s  -F "%s %s %s %s %s"  %s' \
                    % (LogDir, LogDir, name, dataset, trig, VER, strJobId, files, script)
      print(cmd)
      subprocess.call(cmd,shell=True)
      time.sleep(0.2)

      sys.stdout.flush()


