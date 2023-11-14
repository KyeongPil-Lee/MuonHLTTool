# -- usage: python CRAB_Status.py -v -d <CRAB directory>
# -- reference: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRABClientLibraryAPI

class CRABStatusInfo:
    def __init__(self):
        self.status = ""
        self.jobsPerStatus = {}
        self.crabDir = ""
        self.nTotJob = 0

    def __init__(self, crabDir, crabInfo):
        self.status = crabInfo["status"]
        self.jobsPerStatus = crabInfo["jobsPerStatus"]
        self.crabDir = crabDir
        self.nTotJob = len(crabInfo["jobList"])

    def CRABDir(self):
        return self.crabDir

    def Status(self):
        return self.Status

    def NeedResubmit(self):
        flag = False
        if self.nFailedJob() != 0:
            flag = True

        return flag

    def IsStatus(self, status):
        flag = False
        if self.status == status:
            flag = True

        return flag

    def IsCompleted(self):
        flag = False
        if self.IsStatus("COMPLETED") and self.nFailedJob() == 0:
            flag = True

        return flag

    def IsUnknown(self):
        return self.IsStatus("UNKNOWN")

    def nStatusJob(self, status):
        nJob = 0
        if status not in self.jobsPerStatus.keys():
            nJob = 0
        else:
            nJob = self.jobsPerStatus[status]

        return nJob

    def nFailedJob(self):
        return self.nStatusJob("failed")

    def nFinishedJob(self):
        return self.nStatusJob("finished")

    def nRunningJob(self):
        return self.nStatusJob("running")

    def fracStatusJob(self, status):
        frac = 0
        nStatusJob = self.nStatusJob(status)

        if self.nTotJob == 0:
            print("self.nTotJob == 0 ... something went wrong\n")
            sys.exit()

        frac = float(nStatusJob) / float(self.nTotJob)
        return frac

    def fracFailedJob(self):
        return self.fracStatusJob("failed")

    def fracFinishedJob(self):
        return self.fracStatusJob("finished")

    def fracRunningJob(self):
        return self.fracStatusJob("running")




#!/usr/bin/env python
try:
    import CRABClient
    from CRABAPI.RawCommand import crabCommand
except ImportError as ex:
    print("*"*50)
    print("Problem with status encountered: %s" % ex)
    print("run the command:")
    print("source /cvmfs/cms.cern.ch/common/crab-setup.sh")
    print("*"*50)
    sys.exit()

import subprocess
import os
import sys
import time
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--directory', default="CRABDir")
parser.add_argument('-v', '--verboseErrors', action='store_true', default=False)
args = parser.parse_args()

proxy = subprocess.check_output(['voms-proxy-info', '-p']).strip()
if proxy == "":
    print("proxy path is not found: please run the command first:")
    print("voms-proxy-init --voms")
    sys.exit()

print("#"*100)
print("CRAB base directory: %s" % args.directory)
print("VerboseErrors: %s" % args.verboseErrors)
print("proxy : %s" % proxy)
print("#"*100)
print("\n")

list_listdir = os.listdir("./%s" % args.directory)
list_CRABDir = []
print "available crabDir list: "
for filename in list_listdir:
    if "crab_" in filename:
        print "'"+filename+"',"
        list_CRABDir.append( filename )

list_CRABDir.sort()

# theTime = time.strftime('%Y%m%d_%H%M%S', time.localtime(time.time()))
# logFileName = "CRABStatusLog_v%s.txt" % (theTime)
# f_log = open( logFileName, 'w' )
# fullLog = ""

list_resubmit = []
list_completed = []
list_unknown = []
list_others = []

options = ""
if args.verboseErrors:
    options = "--verboseErrors"

for crabDir in list_CRABDir:
    crabDirPath = os.path.join(args.directory, crabDir)

    crabInfo = crabCommand("status", dir=crabDirPath, *options)

    statusInfo = CRABStatusInfo(crabDirPath, crabInfo)

    if statusInfo.NeedResubmit():
        list_resubmit.append( statusInfo )
    elif statusInfo.IsCompleted():
        list_completed.append( statusInfo )
    elif statusInfo.IsUnknown():
        list_unknown.append( statusInfo )
    else:
        list_others.append( statusInfo )


#     # -- for log
#     cmd = 'crab status "'+crabDirPath+'" --proxy='+proxy
#     if args.verboseErrors:
#         cmd = cmd + " --verboseErrors"
#     result = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
#     (stdout, stderr) = result.communicate()
#     thisLog = """
# {line_}
# {cmd_}
# "[stdout]"
# {stdout_}
# "[stderr]"
# {stderr_}
# {line_}

# """.format(line_="#"*100, cmd_=cmd, stdout_=stdout, stderr_=stderr)
    
#     fullLog += thisLog


fullSummary = ""

fullSummary += "[Completed list]\n"
for statusInfo in list_completed:
    fullSummary += "%s\n" % statusInfo.crabDir
fullSummary += "\n"

fullSummary += "[Unknown list]\n"
for statusInfo in list_unknown:
    fullSummary += "%s\n" % statusInfo.crabDir
fullSummary += "\n"

fullSummary += "[Others list]\n"
for statusInfo in list_others:
    comment = "# -- nFinished = %d (%.1f%%), nRunning = %d (%.1f%%)" % (statusInfo.nFinishedJob(), statusInfo.fracFinishedJob()*100, statusInfo.nRunningJob(), statusInfo.fracRunningJob()*100)
    fullSummary += "%s %s\n" % (statusInfo.crabDir, comment)
fullSummary += "\n"

fullSummary += "[CRAB jobs that should be resubmitted]\n"
for statusInfo in list_resubmit:
    cmd = 'crab resubmit '+statusInfo.crabDir+' --proxy='+proxy
    comment = "# -- nFinished = %d (%.1f%%), nFailed = %d (%.1f%%)" % (statusInfo.nFinishedJob(), statusInfo.fracFinishedJob()*100, statusInfo.nFailedJob(), statusInfo.fracFailedJob()*100)
    fullSummary += "%s %s\n" % (cmd, comment)
fullSummary += "\n"

print(fullSummary)

# -- write in the file
# f_log.write(fullLog)
# f_log.write(fullSummary)
# print("To check the full log:")
# print("cat %s" % logFileName)
# f_log.close()
