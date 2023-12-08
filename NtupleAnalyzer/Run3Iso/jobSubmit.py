import os, sys, time, argparse


parser = argparse.ArgumentParser()
parser.add_argument('-c', '--code', default="")
# parser.add_argument('-v', '--verboseErrors', action='store_true', default=False)
parser.add_argument('-n', '--newSample', action='store_true', default=False)
args = parser.parse_args()

print("arguments")
print("args.newSample = %d" % args.newSample)
print("args.code = %s" % args.code)
print("="*30)
g_basePath = '/pnfs/iihe/cms/store/user/kplee/MuonHLTTree/Run3_IDIso_132X_v1'
g_dic_datasetName = {
  "DY" : "DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8",
  "QCD_Pt20to30" : "QCD_PT-20to30_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8",
  "QCD_Pt30to50" : "QCD_PT-30to50_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8",
  "QCD_Pt50to80" : "QCD_PT-50to80_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8",
  "QCD_Pt80to120" : "QCD_PT-80to120_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8",
  "QCD_Pt120to170" : "QCD_PT-120to170_MuEnrichedPT5_TuneCP5_13p6TeV_pythia8",
}
g_dic_nJob = {
  "DY" : 30,
  "QCD_Pt20to30" : 5,
  "QCD_Pt30to50" : 5,
  "QCD_Pt50to80" : 5,
  "QCD_Pt80to120" : 5,
  "QCD_Pt120to170" : 5,
}
# g_cppCode = "MakeHist_Isolation.cxx"

class SampleInfo:
  def __init__(self):
    self.tag = ""
    self.textDirName = "ROOTFileList"
    self.list_rootFile = []

  def Fill_ROOTFileList(self):
    textFileName = "%s/%s.txt" % (self.textDirName, self.tag)
    if not os.path.exists(textFileName):
      print("%s does not exist ... use Make_TextFile first" % textFileName)
      sys.exit()

    f = open(textFileName, "r")
    for line in f.readlines():
      self.list_rootFile.append( line.split("\n")[0] )

    # for rootFile in self.list_rootFile:
    #   print(rootFile)

  def Make_TextFile(self, where):
    print("[Make_TextFile] path  = %s" % where)

    # -- find the root file
    for (root, list_dir, list_file) in os.walk(where):
      print("searching in %s ..." % root)
      # print("--> list_file = ", list_file)

      for fileName in list_file:
        if fileName.endswith(".root"):
          absFilePath = "%s/%s" % (root, fileName)
          self.list_rootFile.append( absFilePath )

    if not os.path.exists(self.textDirName):
      os.makedirs(self.textDirName)

    textFileName = "%s/%s.txt" % (self.textDirName, self.tag)
    if textFileName in os.listdir(self.textDirName):
      print("textFileName = %s already exists under %s" % (self.textDirName, textFileName) )
      sys.exit()

    f = open(textFileName, "w")
    for rootFileName in self.list_rootFile:
      f.write(rootFileName+"\n")

    print("text file is created: %s" % textFileName)

    f.close()


class SingleJob:
  def __init__(self):
    self.cppCode = ""
    self.sampleTag = ""
    self.motherDir = ""
    self.i_job = -1
    self.list_rootFile = []

    self.dirPath = ""
    self.shellScriptName = ""

  def Setup(self):
    if self.i_job < 0 or len(self.list_rootFile) == 0 or self.motherDir == "":
      print("SingleJob is not set!")
      sys.exit()

    self.Make_Dir()
    self.Make_TextFile()
    self.Make_ShellScript()
    self.Make_CondorScript()

  def Submit(self):
    cmd = "condor_submit %s" % (self.condorScriptName)
    # print(cmd)
    os.system(cmd)

  def Make_Dir(self):
    self.dirPath = "%s/Job_%03d" % (self.motherDir, self.i_job)
    os.makedirs(self.dirPath)

  def Make_TextFile(self):
    filePath = "%s/fileList.txt" % self.dirPath

    f = open(filePath, "w")

    for rootFile in self.list_rootFile:
      f.write(rootFile+"\n")

    f.close()

  def Make_ShellScript(self):
    self.shellScriptName = "%s/%03d.sh" % (self.dirPath, self.i_job)

    f = open(self.shellScriptName, "w")
    f.write("""
#!/bin/bash

cd {cwd_}
cp {cppCodePath_} ./

root -l -b -q '{cppCode_}++("{tag_}", "{i_job_}")'

echo "done"

mv *.root ../

""".format(cwd_=self.dirPath, cppCodePath_=self.cppCode, cppCode_=self.cppCode.split("/")[-1], tag_=self.sampleTag, i_job_ = self.i_job))

    f.close()

  def Make_CondorScript(self):
    self.condorScriptName = "%s/%03d.sub.txt" % (self.dirPath, self.i_job)

    f = open(self.condorScriptName, "w")
    f.write("""
universe    = vanilla
executable  = {shellScriptName_}
getenv      = True
output      = {cwd_}/output.txt
error       = {cwd_}/error.txt
log         = {cwd_}/log.txt
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
queue

""".format(shellScriptName_=self.shellScriptName, cwd_=self.dirPath))

    f.close()

class SampleJob:
  def __init__(self):
    self.cppCode = ""
    self.motherDir = ""
    self.sampleInfo = ""
    self.nJob = -1

    self.nFilePerJob = 0
    self.dirPath = ""

  def SetupAndSubmit(self):
    if self.sampleInfo == "" or self.nJob < 0 or self.motherDir == "":
      print("SampleJob is not set!")
      sys.exit()

    self.Make_Dir()
    self.Adjust_nJob()
    for i_job in range(0, self.nJob):
      self.SetupAndSubmit_SingleJob(i_job)


  def Make_Dir(self):
    self.dirPath = "%s/%s" % (self.motherDir, self.sampleInfo.tag)
    os.makedirs(self.dirPath)

  def Adjust_nJob(self):
    nFile = len(self.sampleInfo.list_rootFile)
    if self.nJob > nFile:
      print("(nJob = %d > # files = %d ... set nJob = nFile" % (self.nJob, nFile))
      self.nJob = nFile

    self.nFilePerJob = int( float(nFile) / float(self.nJob) )
    print("nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob))

  def SetupAndSubmit_SingleJob(self, i_job):
    list_rootFile_iJob = self.Get_ROOTFileList_iJob(i_job)

    singleJob = SingleJob()
    singleJob.cppCode = self.cppCode
    singleJob.sampleTag = self.sampleInfo.tag
    singleJob.motherDir = self.dirPath
    singleJob.i_job = i_job
    singleJob.list_rootFile = list_rootFile_iJob

    singleJob.Setup()
    singleJob.Submit()

  def Get_ROOTFileList_iJob(self, i_job):
      list_rootFile = []
      if i_job == self.nJob-1:
          list_rootFile = self.sampleInfo.list_rootFile[int(i_job*self.nFilePerJob):]
      else:
          list_rootFile = self.sampleInfo.list_rootFile[int(i_job*self.nFilePerJob):int((i_job+1)*self.nFilePerJob)]

      return list_rootFile

class JobSubTool:
  def __init__(self):
    self.cppCode = ""
    self.list_sampleInfo = []
    self.list_nJob = []

    self.motherDirPath = ""

  def Submit(self):
    self.Make_MotherDir()
    self.Make_HaddScript()

    for i_sample in range(0, len(self.list_sampleInfo)):
      print("Start submitting the sample (%s) ..." % self.list_sampleInfo[i_sample].tag)
      sampleJob = SampleJob()

      sampleJob.cppCode = self.cppCode
      sampleJob.motherDir = self.motherDirPath
      sampleJob.sampleInfo = self.list_sampleInfo[i_sample]
      sampleJob.nJob = self.list_nJob[i_sample]

      sampleJob.SetupAndSubmit()
      print("Submission of the sample (%s): done" % list_sampleInfo[i_sample].tag)

  def Make_HaddScript(self):
    scriptName = "script_hadd.sh"
    scriptPath = "%s/%s" % (self.motherDirPath, scriptName)

    f = open(scriptPath, "w")
    f.write("#!bin/bash\n\n")

    for sampleInfo in self.list_sampleInfo:
      mergedFile = "%s.root" % sampleInfo.tag
      cmd_hadd = "hadd %s %s/*.root" % (mergedFile, sampleInfo.tag)
      f.write(cmd_hadd+"\n")
      f.write('echo "hadd for %s sample: done"\n\n' % sampleInfo.tag)

    f.close()

  def Make_MotherDir(self):
    cwd = os.getcwd()
    dirName = self.Get_DirName()
    self.motherDirPath = "%s/%s" % (cwd, dirName)
    os.makedirs(self.motherDirPath)

  def Get_DirName(self):
    timeInfo = time.localtime()
    baseName = "CondorJob_%d%d%d" % (timeInfo.tm_year, timeInfo.tm_mon, timeInfo.tm_mday)

    dirName_final = ""
    for version in range(0, 100):
      dirName = "%s_v%03d" % (baseName, version)
      if not os.path.exists(dirName):
        dirName_final = dirName
        break

    return dirName_final

def MakeList_SampleInfo():
  list_nJob = []
  list_info = []
  for tag in g_dic_datasetName.keys():
    list_info.append( Make_SampleInfo(tag, g_basePath, g_dic_datasetName[tag]) )
    list_nJob.append( g_dic_nJob[tag] )

  return (list_info, list_nJob)

def Make_SampleInfo(tag, basePath, dirName):
  info = SampleInfo()
  info.tag = tag

  if args.newSample:
    dirPath = "%s/%s" % (g_basePath, g_dic_datasetName[tag])
    info.Make_TextFile(dirPath)
  else:
    info.Fill_ROOTFileList()

  return info

if __name__ == '__main__':
  if args.code == "":
    print("code should be provided")
    sys.exit()

  (list_sampleInfo, list_nJob) = MakeList_SampleInfo()

  tool = JobSubTool()
  cwd = os.getcwd()
  tool.cppCode = "%s/%s" % (cwd, args.code)
  tool.list_sampleInfo = list_sampleInfo
  tool.list_nJob = list_nJob

  tool.Submit()
