import os
import sys
import shutil
import yaml

class MultiCondorJobSubmitter:
    def __init__(self):
        self.ROOTCodeName = ""
        self.sampleInfoYAML = ""
        self.dic_sample_nJob = {}

        # -- automatically merge the output files from binned samples
        self.doMergeBinnedSample = False

        ###########################
        # -- internal variable -- #
        ###########################
        self.cwd = os.getcwd() # -- where the job submitter is executed: the final ROOT files should come here

        # -- base directory for all condor-job related files (condor log & err files, text files w/ ntuple path, all ROOT files before merging...)
        self.baseDirPath = ""

        # -- define below to have a coherent file names for all multiple jobs
        # -- directory for the text files with the ntuple paths
        self.textFileDirName = "textFiles"

        # -- shell script name to run (executable in the condor script)
        self.scriptNameToRun = "script_condorRun.sh"

        # -- condor script name
        self.condorScriptName = "condorscript_submit.jds"

        # -- base name of the text file with the ntuple path lists (full name: baseName_ntupleList_$(Process).txt)
        self.baseName_ntupleList = "ntuplePathList"


    def Submit(self):
        self.GenerateBaseDir()

        for sampleType in self.dic_sample_nJob.keys():
            submitter = CondorJobSubmitter()

            submitter.textFileDirName = self.textFileDirName
            submitter.scriptNameToRun = self.scriptNameToRun
            submitter.condorScriptName = self.condorScriptName
            submitter.baseName_ntupleList = self.baseName_ntupleList

            ROOTCodePath = "%s/%s" % (self.cwd, self.ROOTCodeName)
            submitter.ROOTCodePath = ROOTCodePath
            submitter.sampleInfoYAML = self.sampleInfoYAML
            submitter.sampleType = sampleType
            submitter.nJob = self.dic_sample_nJob[sampleType]

            submitter.baseDirPath = self.baseDirPath

            submitter.Submit()

        self.GenerateScript_hadd()


    def GenerateBaseDir(self):
        i = 1
        while "condorJob_v%02d" % i in os.listdir(self.cwd):
            i = i+1

        # -- create in the current directory
        self.baseDirPath = "%s/condorJob_v%02d" % (self.cwd, i)
        os.mkdir(self.baseDirPath)

    def GenerateScript_hadd(self):
        scriptPath = "%s/hadd_allSamples.sh" % self.baseDirPath
        f_script = open(scriptPath, "w")
        f_script.write("#!/bin/bash\n")

        for sampleType in self.dic_sample_nJob.keys():
            mergedROOTFileName = "ROOTFile_%s_%s.root" % (self.ROOTCodeName.split(".cxx")[0], sampleType)
            mergedROOTFilePath = "%s/%s" % (self.cwd, mergedROOTFileName)

            sampleWorkingDirPath = "%s/%s" % (self.baseDirPath, sampleType)

            command_hadd = "hadd %s %s/*.root" % (mergedROOTFilePath, sampleWorkingDirPath)
            f_script.write(command_hadd+"\n")

        f_script.write("\n")
        f_script.close()

        print "[MultiCondorJobSubmitter] Get merged ROOT files after the condor jobs are finished"
        print "source %s\n" % scriptPath



class CondorJobSubmitter:
    def __init__(self):
        self.ROOTCodePath = ""
        self.sampleInfoYAML = ""
        self.nJob = -1
        self.sampleType = ""

        ############################
        # -- internal variables -- #
        ############################
        # -- # files per job
        self.nFilePerJob = 0

        # -- directory contains "condor" directory (log & err) and "textFiles" directory (text files for the ntuple paths)
        # -- it will be provided by MultiCondorJobSubmitter
        self.baseDirPath = ""

        # -- working directory path: baseDirPath + "/" + sampleType
        self.workingDirPath = "";

        # -- directory for the text files with the ntuple paths
        self.textFileDirName = "textFiles"

        # -- shell script name to run (executable in the condor script)
        self.scriptNameToRun = "script_condorRun.sh"

        # -- condor script name
        self.condorScriptName = "condorscript_submit.jds"

        # -- base name of the text file with the ntuple path lists (full name: baseName_ntupleList_$(Process).txt)
        self.baseName_ntupleList = "ntuplePathList"

        # -- list of directories that contains ntuples
        self.list_ntupleDirPath = []

        # -- list of full paths to each ntuple in self.list_ntupleDirPath
        self.list_ntuplePath = []

        # -- cross section & sum(weight) from yaml file
        self.xSec = -1.0
        self.sumWeight = -1.0


    def Submit(self):
        self.GenerateWorkingDir()

        self.GetSampleInfoFromYAML()

        self.GenerateTextFile_NtuplePathList()

        self.GenerateCommonShellScript()

        self.GenerateCondorScript()

        condorScriptPath = "%s/%s" % (self.workingDirPath, self.condorScriptName)
        cmd_submit = "condor_submit %s" % condorScriptPath
        os.system(cmd_submit)
        # print cmd_submit
        print "All jobs are submitted"

    def GenerateWorkingDir(self):
        if self.sampleType not in os.listdir(self.baseDirPath):
            self.workingDirPath = "%s/%s" % (self.baseDirPath, self.sampleType)
            os.mkdir(self.workingDirPath)
        else:
            print "%s exists in %s ... the jobs are not created. please check" % (self.sampleType, self.baseDirPath)
            sys.exit()

    def GetSampleInfoFromYAML(self):
        analyzerPath = os.getenv("MUONHLT_ANALYZER_PATH")
        includePath = "%s/Include" % analyzerPath
        path_sampleInfoYAML = "%s/%s" % (includePath, self.sampleInfoYAML)

        yamlParser = ""
        with open(path_sampleInfoYAML) as f:
            yamlParser = yaml.load(f)
            # yamlParser = yaml.load(f, Loader=yaml.FullLoader)

        list_sampleInfo = yamlParser["list_sampleInfo"]

        isFound = False
        for sampleInfo in list_sampleInfo:
            if self.sampleType == sampleInfo["sampleType"]:
                self.list_ntupleDirPath = sampleInfo["list_ntupleDirPath"]
                self.xSec = sampleInfo["xSec"]
                self.sumWeight = sampleInfo["sumWeight"]
                isFound = True
                break

        if not isFound:
            print "Sample type = %s is not found!" % self.sampleType
            sys.exit()

    # -- all files are written with its absolute paths to avoid any confusion
    def GenerateCondorScript(self):

        # -- directory for condor log files
        if "condor" not in os.listdir(self.workingDirPath):
            os.mkdir("%s/condor" % self.workingDirPath)

        condorScriptPath = "%s/%s" % (self.workingDirPath, self.condorScriptName)
        f_script = open(condorScriptPath, "w")
        # -- $(Process): 0, 1, 2, ... nJob-1
        f_script.write(
"""
executable = {scriptPathToRun_}
universe   = vanilla
log        = {workingDirPath_}/condor/condor.log
getenv     = True
# -- to transfer input files
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
# -- output files will directly be copied in the shell script
# -- to avoid to transfer some (unnecessary) temp. files
Transfer_Output_Files = ""

accounting_group=group_cms

transfer_input_files = {textFileDirPath}/{baseName_ntupleList_}_$(Process).txt,{ROOTCodePath_}
Arguments            = "{baseName_ntupleList_}_$(Process).txt"
output               = {workingDirPath_}/condor/condor_output_$(Process).log
error                = {workingDirPath_}/condor/condor_error_$(Process).log

queue {nJob_}

""".format(scriptPathToRun_ = "%s/%s" % (self.workingDirPath, self.scriptNameToRun),
           workingDirPath_ = self.workingDirPath,
           textFileDirPath = "%s/%s" % (self.workingDirPath, self.textFileDirName), 
           baseName_ntupleList_ = self.baseName_ntupleList, 
           ROOTCodePath_ = self.ROOTCodePath, 
           nJob_ = self.nJob)
        )

        f_script.close()


    def GenerateCommonShellScript(self):
        scriptPathToRun = "%s/%s" % (self.workingDirPath, self.scriptNameToRun)
        f_script = open(scriptPathToRun, "w")

        ROOTCodeName = self.ROOTCodePath.split("/")[-1]
        f_script.write(
"""#!/bin/bash

fileName_ntuplePathList=$1

root -l -b -q '{ROOTCodeName_}++("'$fileName_ntuplePathList'")'

# -- copy the output ROOT files to the working directory
cp *.root %s {workingDirPath_}

echo "finished"

""".format(ROOTCodeName_ = ROOTCodeName, workingDirPath_=self.workingDirPath)
        )

        f_script.close()


    def GenerateTextFile_NtuplePathList(self):
        for ntupleDirPath in self.list_ntupleDirPath:
            list_temp = self.GetList_NtuplePath(ntupleDirPath)
            self.list_ntuplePath.extend( list_temp )

        nFile = len(self.list_ntuplePath)
        if self.nJob > nFile:
            print "nJob > nFile -> nJob is set as nFile"
            self.nJob = nFile

        self.nFilePerJob = int( float(nFile) / float(self.nJob) )
        print "nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob)

        self.MakeTextFileDir()

        for i_job in range(0, self.nJob):
            list_ntuplePath_iJob = self.GetNtupleListPerJob(i_job)

            textFileName = "%s/%s/%s_%d.txt" % (self.workingDirPath, self.textFileDirName, self.baseName_ntupleList, i_job)
            f_textFile = open(textFileName, "w")

            str_info_sampleType = "# -- sample type: %s" % self.sampleType
            str_info_xSec       = "# -- cross section: %s" % self.xSec
            str_info_sumWeight  = "# -- sum of weights: %s" % self.sumWeight 

            # -- put some information about the sample at first
            f_textFile.write(str_info_sampleType+"\n")
            f_textFile.write(str_info_xSec+"\n")
            f_textFile.write(str_info_sumWeight+"\n")

            # -- and put the ntuple paths
            for fileName in list_ntuplePath_iJob:
                f_textFile.write( fileName + "\n" )

            f_textFile.close()


    def MakeTextFileDir(self):
        if self.textFileDirName not in os.listdir(self.workingDirPath):
            textFileDirPath = "%s/%s" % (self.workingDirPath, self.textFileDirName)
            os.mkdir(textFileDirPath)
        else:
            print "[CondorJobSubmitter] %s exists in this directory... please check" % self.textFileDirName
            sys.exit()


    def GetList_NtuplePath(self, baseNtupleDir):
        list_rootFile = []

        list_file = os.listdir(baseNtupleDir)
        for fileName in list_file:
            if ".root" in fileName:
                ntuplePath = "%s/%s" % (baseNtupleDir, fileName)
                list_rootFile.append( ntuplePath )

        if len(list_rootFile) == 0:
            print "[GetList_NtuplePath] no root file under %s ... exit" % baseNtupleDir
            sys.exit()

        list_rootFile.sort()

        return list_rootFile


    def GetNtupleListPerJob(self, i_job):
        list_filePerJob = []
        if i_job == self.nJob-1:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):]
        else:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):int((i_job+1)*self.nFilePerJob)]

        return list_filePerJob


