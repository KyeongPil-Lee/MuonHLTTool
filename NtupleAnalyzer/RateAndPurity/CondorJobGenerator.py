import os
import sys

def GetNtupleDir(dataset):
    basePath = "/gv0/Users/kplee/MUO19001"
    return "%s/%s" % (basePath, dataset)


class JobGenerator:
    def __init__(self):
        self.rootMacro = ""
        self.shellScript = ""

        self.baseJobDir = ""
        self.trigger = ""
        self.dataset = ""
        self.version = ""
        self.etaMin = 0.0
        self.etaMax = 5.0
        self.nJob = 1

        # -- internal variables
        self.currentDir = os.getcwd()
        self.WSPath = ""
        self.list_ntuplePath = []
        self.nFilePerJob = 0
        self.condorScriptName = "condorScript_submit.jds"

    def Generate(self):
        self.CreateWorkspace()
        self.CreateSubWorkSpaces()

    def CreateWorkspace(self):
        baseNtupleDir = GetNtupleDir(self.dataset)
        self.list_ntuplePath = self.GetList_NtuplePath(baseNtupleDir)

        self.WSPath = "%s/RateAndPurity_%s/%s/%s" % (self.baseJobDir, self.version, self.dataset, self.trigger)
        if "RateAndPurity_%s" % self.version in os.listdir(self.baseJobDir):
            print "RateAndPurity_%s already exists under %s" % (self.version, self.baseJobDir)
            print "change the version ..."
            sys.exit()
        else:
            os.makedirs(self.WSPath)


    def CreateSubWorkSpaces(self):
        nFile = len(self.list_ntuplePath)
        if self.nJob > nFile:
            print "nJob > nFile -> nJob is set as nFile"
            self.nJob = nFile

        self.nFilePerJob = int( float(nFile) / float(self.nJob) )
        print "nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob)

        for i in range(0, self.nJob):
            # -- make sub workspace directory
            dirName = "Job_v%03d" % i
            subWSPath = "%s/%s" % (self.WSPath, dirName)
            os.mkdir( subWSPath )

            # -- copy files
            cmd_cp_rootMarco   = "cp %s %s" % (self.rootMacro, subWSPath)
            cmd_cp_shellScript = "cp %s %s" % (self.shellScript, subWSPath)

            # os.system(cmd_cp_rootMarco)
            # os.system(cmd_cp_shellScript)

            # -- get list of ntuples for i-th job
            list_ntuplePath_iJob = self.GetNtupleListPerJob(i)

            # -- generate condor script for i-th job
            self.GenerateCondorScript(subWSPath, i, list_ntuplePath_iJob)

            cmd_submit = "condor_submit %s" % self.condorScriptName

            print "subWSPath = %s" % subWSPath
            print cmd_submit
            
            os.chdir(subWSPath)
            os.system(cmd_submit)

        os.chdir(self.currentDir)


    def GenerateCondorScript(self, subWSPath, i_job, list_ntuplePath_iJob):
        vec_ntuplePath = self.GetCppVector_ntuplePath(list_ntuplePath_iJob)
        # scriptNameToRun = self.shellScript.split("/")[-1]
        jobID = "Job"+str(i_job)

        f_script = open("%s/%s" % (subWSPath, self.condorScriptName), "w")
        f_script.write(
"""
executable = {shellScript_}
universe   = vanilla
Arguments  = {trigger_} {dataset_} {version_} {etaMin_} {etaMax_} {jobID_} {vec_ntuplePath_}
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
initialdir={path_}
log        = condor_{jobID_}.log
output = condor_output_{jobID_}.log
error  = condor_error_{jobID_}.log
accounting_group=group_cms
transfer_input_files={rootMacro_}
transfer_output_remaps = "Output-MuonTriggerPurity-{dataset_}-{version_}-{trigger_}-{etaMin_:0.1f}-{etaMax_:0.1f}--{jobID_}.root = {cwd_}/Output-MuonTriggerPurity-{dataset_}-{version_}-{trigger_}-{etaMin_}-{etaMax_}--{jobID_}.root"

queue 1
""".format(path_=subWSPath, shellScript_=self.shellScript, rootMacro_=self.rootMacro,
           dataset_=self.dataset, version_=self.version, trigger_=self.trigger, vec_ntuplePath_=vec_ntuplePath,
           etaMin_=self.etaMin, etaMax_=self.etaMax, jobID_=jobID, cwd_=self.currentDir))

        f_script.close()



    def GetCppVector_ntuplePath(self, list_ntuplePath):
        vec_cpp = ""

        for ntuplePath in list_ntuplePath:
            if ntuplePath == list_ntuplePath[-1]: # -- if it is the last one
                 vec_cpp = vec_cpp + ',\\"%s\\"}' % ntuplePath
            elif ntuplePath == list_ntuplePath[0]: # -- if it is the first one
                vec_cpp = vec_cpp + '{\\"%s\\"' % ntuplePath
            else:
                vec_cpp = vec_cpp + ',\\"%s\\"' % ntuplePath

        return vec_cpp


    def GetNtupleListPerJob(self, i_job):
        list_filePerJob = []
        if i_job == self.nJob-1:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):]
        else:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):int((i_job+1)*self.nFilePerJob)]

        return list_filePerJob


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