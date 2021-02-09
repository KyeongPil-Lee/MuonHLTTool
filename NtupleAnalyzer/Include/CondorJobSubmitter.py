import os
import sys
import shutil

class CondorJobSubmitter:
    def __init__(self):
        self.ROOTCodeName = ""
        self.list_ntupleDirPath = []
        self.nJob = 0
        self.removeTempDirAfterSubmit = False


        # -- internal variable

        # -- # files per job
        self.nFilePerJob = 0
        # -- temp. directory for the text files with the ntuple paths
        self.tempDir = "temp"

        # -- shell script name to run (executable in the condor script)
        self.scriptNameToRun = "script_condorRun.sh"

        # -- condor script name
        self.condorScriptName = "condorscript_submit.jds"

        # -- base name of the text file with the ntuple path lists (full name: baseName_ntupleList_$(Process).txt)
        self.baseName_ntupleList = "ntuplePathList";

        # -- list of full paths to each ntuple in self.list_ntupleDirPath
        self.list_ntuplePath = []


    def Submit(self):
        self.GenerateTextFile_NtuplePathList()

        self.GenerateCommonShellScript()

        self.GenerateCondorScript()

        cmd_submit = "condor_submit %s" % self.condorScriptName
        os.system(cmd_submit)
        # print cmd_submit
        print "All jobs are submitted"

        if self.removeTempDirAfterSubmit:
            shutil.rmtree(self.tempDir)
            print "%s is successfully removed" % self.tempDir


    def GenerateCondorScript(self):

        # -- directory for condor log files
        if "condor" not in os.listdir("."):
            os.mkdir("condor")

        f_script = open(self.condorScriptName, "w")
        # -- $(Process): 0, 1, 2, ... nJob-1
        f_script.write(
"""
executable = {scriptNameToRun_}
universe   = vanilla
log        = condor/condor.log
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT

accounting_group=group_cms

transfer_input_files = {tempDir_}/{baseName_ntupleList_}_$(Process).txt,{ROOTCodeName_}
Arguments            = "{baseName_ntupleList_}_$(Process).txt"
output               = condor/condor_output_$(Process).log
error                = condor/condor_error_$(Process).log

queue {nJob_}

""".format(scriptNameToRun_ = self.scriptNameToRun, tempDir_=self.tempDir, baseName_ntupleList_=self.baseName_ntupleList, ROOTCodeName_=self.ROOTCodeName, nJob_=self.nJob)
        )

        f_script.close()


    def GenerateCommonShellScript(self):

        f_script = open(self.scriptNameToRun, "w")
        f_script.write(
"""#!/bin/bash

fileName_ntuplePathList=$1

root -l -b -q '{ROOTCodeName_}++("'$fileName_ntuplePathList'")'

echo "finished"

""".format(ROOTCodeName_ = self.ROOTCodeName)
        )

        f_script.close()


    def GenerateTextFile_NtuplePathList(self):
        for ntuplePath in self.list_ntupleDirPath:
            list_temp = self.GetList_NtuplePath(ntuplePath)
            self.list_ntuplePath.extend( list_temp )

        nFile = len(self.list_ntuplePath)
        if self.nJob > nFile:
            print "nJob > nFile -> nJob is set as nFile"
            self.nJob = nFile

        self.nFilePerJob = int( float(nFile) / float(self.nJob) )
        print "nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob)

        self.MakeTempDir()

        for i_job in range(0, self.nJob):
            list_ntuplePath_iJob = self.GetNtupleListPerJob(i_job)

            textFileName = "%s/%s_%d.txt" % (self.tempDir, self.baseName_ntupleList, i_job)
            f_textFile = open(textFileName, "w")
            for fileName in list_ntuplePath_iJob:
                f_textFile.write( fileName + "\n" )
            f_textFile.close()


    def MakeTempDir(self):
        if self.tempDir not in os.listdir("."):
            os.mkdir(self.tempDir)
        else:
            print "[CondorJobSubmitter] %s exists in this directory... please check" % self.tempDir
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


