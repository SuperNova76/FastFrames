import argparse
import os
import yaml

# Define the colors for the output
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
def DEBUG(text):
    return bcolors.OKGREEN+text+bcolors.ENDC
def TITLE(text):
    return bcolors.OKBLUE+bcolors.BOLD+text+bcolors.ENDC
def HEADER(text):
    return bcolors.HEADER+bcolors.BOLD+bcolors.OKBLUE+bcolors.UNDERLINE+text+bcolors.ENDC
def WARNING(text):
    return bcolors.WARNING+text+bcolors.ENDC
def ERROR(text):
    return bcolors.FAIL+text+bcolors.ENDC

def createParser():
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--config",  help="Path to the yml config file.")
    parser.add_argument("--samples", help="A comma separated list of samples to run. One job is created per listed sample. Default: all samples listed in the yml config.",default="all")
    parser.add_argument("--step",    help="Step to run: 'n' (ntuples) or 'h' (histograms). Default: 'h'", choices=["h","n"],  default="h")
    parser.add_argument("--custom-class-path", help= "Path to the custom class used in the config file (if used). Default: None", default=None)
    parser.add_argument("--max-time", help="Maximum time for the job to run. Default: 1h", default="microcentury")
    parser.add_argument("--dry-run", help="Creates the execution and submission environment without sending the jobs to HTCondor. Useful for debugging.", action="store_true")
    parser.add_argument("--chicago", help="Use this flag if you are running the jobs in the Chicago Analysis Facility.", action="store_true")
    return parser

# The params dictionary contains the parameters that are calculated given the user input.
jobParamatersDict = {
    "initialdir" : "",
    "RequestCpus" : "",
    "transfer_input_files" : "fastframes/,build/,install/,metadata/",
    "+JobFlavour" : ""
}

# Get the path to the FastFrames directory
def getFFPath():
    submissionPath = os.getcwd()
    twoLevelsUp = os.path.abspath(os.path.join(submissionPath, os.pardir, os.pardir))
    return twoLevelsUp

def createSubmissionFile(paramsDictionary):  
    executableCMD = "executable              = runFF.sh\n"
    argumentsCMD = "arguments               = $(ClusterId)$(ProcId)\n"
    outputCMD = "output                  = output/runFF.$(ClusterId).$(ProcId).out\n"
    errorCMD = "error                   = error/runFF.$(ClusterId).$(ProcId).err\n"
    logCMD = "log                     = log/runFF.$(ClusterId).log\n"
    getenvCMD = "getenv                  = True\n"
    preserveRelativePathsCMD = "preserve_relative_paths = True\n"
    with open("condor_submit.sub","w") as f:
        f.write(executableCMD)
        f.write(argumentsCMD)
        f.write(outputCMD)
        f.write(errorCMD)
        f.write(logCMD)
        f.write(getenvCMD) # Get the environment variables from the submission machine (lxplus)
        f.write(preserveRelativePathsCMD)
        for key,value in paramsDictionary.items():
            f.write(key + " = " + str(value) + "\n")
        f.write("\n")
        f.write("queue arguments from inputSamples.txt\n")

def createExecutable(configYMLPath,step):
    # Copy the config file to the submission directory
    os.system('cp ' +configYMLPath+ ' HTCondorConfig.yml')
    with open("runFF.sh","w") as f:
        f.write("#!/bin/bash\n")
        f.write("shopt -s expand_aliases\n") # Enable aliases in the remote machine
        f.write("alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'\n") # Set up the ATLAS environment
        f.write("setupATLAS\n")
        f.write("asetup StatAnalysis,0.3.2\n")
        f.write("source build/setup.sh\n") # Load the FastFrames environment
        f.write("source CustomClassForCondor/build/setup.sh\n") # Load the custom class environment
        f.write("cd fastframes/python\n")
        f.write("python3 FastFrames.py " + "--config HTCondorConfig.yml --step " +step+ " --samples $1\n") # Run
    # Give the file executable rights.
    os.system("chmod +x runFF.sh")

def loadYMLConfig(pathToConfig):
    with open(pathToConfig, 'r') as stream:
        try:
            return yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            print(exc)

def copyCustomClassFilesForSubmission(customClassPath):
    # First, remove any previous custom class used for condor jobs.
    try:
        os.remove('../../CustomClassForCondor/')
    except OSError:
        pass

    # Then, copy the custom class files to the submission directory
    try:
        os.system('rsync -r --exclude .git '+customClassPath+'/*'+' '+'../../CustomClassForCondor') # Avoid copying the .git directory if present
    except FileNotFoundError:
        print("Error copying the custom class files to the submission directory.")
        print(customClassPath,' path to custom class not found.')
        exit()

def setupJobParamsDict(generalBlock,cmdLineArguments):
     # Set to the FastFrames path
    jobParamatersDict["initialdir"] = getFFPath()
    # Set number of cpus as requested in the config file
    jobParamatersDict["RequestCpus"] = generalBlock["number_of_cpus"]
    # Set up the wall-time
    jobParamatersDict["+JobFlavour"] = cmdLineArguments.max_time
    # Add the custom class path to the transfer_input_files
    if cmdLineArguments.custom_class_path is not None:
        jobParamatersDict["transfer_input_files"] += ",CustomClassForCondor/"

    # Configuration change for Chicago Analysis Facility
    if cmdLineArguments.chicago:
        # First remove the options not supported in the Chicago Analysis Facility
        jobParamatersDict.pop("RequestCpus")
        jobParamatersDict.pop("+JobFlavour")
        # Add the Chicago specific options
        jobParamatersDict["request_cpus"] = generalBlock["number_of_cpus"]
        jobParamatersDict["request_memory"] = " 10GB"
        jobParamatersDict["+queue"] = '"short"'


def createInputSamplesFile(listOfSamplesFromInput,samplesBlock):
    listOfSamples = listOfSamplesFromInput
    if listOfSamplesFromInput == ["all"]: # If no samples are given, use all samples in the config file
        listOfSamples = []
        for sample in samplesBlock:
            listOfSamples.append(sample["name"])
    
    with open("inputSamples.txt","w") as f:
        for sample in listOfSamples:
            f.write(sample + "\n")

def checkIsAFSorEOSPath(path):
    if not path.startswith("/eos/") and not path.startswith("/afs/"):
        print(WARNING("The path to the output files is recommended to be an absolute path in EOS or AFS."))
        print(path," is neither an AFS or EOS path.")

def checkAssumptions(geeneralBlock):
    # 1. The script is run from the fastframes/python directory
    currentPath = os.getcwd()
    twoLastLevels = currentPath.split("/")[-2:]
    if twoLastLevels != ["fastframes","python"]:
        print(ERROR("Please run the script from the fastframes/python directory."))
        exit()

    # 2. The build and install directories of FastFrames are two levels up from the current directory, i.e, at the same level than the FastFrames source code.
    fastframesPath = getFFPath()
    directoriesAtSameLevel = os.listdir(fastframesPath)
    if "build" not in directoriesAtSameLevel or "install" not in directoriesAtSameLevel:
        print(ERROR("The build and install directories of FastFrames are not at the same level than the FastFrames source code."))
        exit()

    # 3. The metadata files are stored in a folder called metadata at the same level than the FastFrames source code.
    if "metadata" not in directoriesAtSameLevel:
        print(ERROR("The metadata files are not stored in a folder called metadata at the same level than the FastFrames source code."))
        exit()
    
    # 4. The path to the output files is an absolute path in EOS or AFS.
    pathToOutputHisotograms = geeneralBlock["output_path_histograms"]
    pathToOutputNtuples = geeneralBlock["output_path_ntuples"]
    checkIsAFSorEOSPath(pathToOutputHisotograms)
    checkIsAFSorEOSPath(pathToOutputNtuples)

def askUserForConfirmation():
    print(TITLE("This script submits jobs to the HTCondor batch system from an lxplus-like machine..."))
    print(DEBUG("Please read the following carefully... you are about to potentially submit a large number of jobs to the HTCondor batch system."))
    print("For a correct execution. This script assumes the following:")
    print(TITLE("1. "), "The script is run from the fastframes/python directory")
    print(TITLE("2. "), "The build and install directories of FastFrames are two levels up from the current directory, i.e, at the same level than the FastFrames source code.")
    print(TITLE("3. "), "The metadata files are stored in a folder called metadata at the same level than the FastFrames source code.")
    print(TITLE("4. (OPTIONAL)"), "The path to the output files is an absolute path in EOS or AFS preferentially.")
    print("Are these assumptions correct? [y/n]")
    userConfirmation = input()
    if userConfirmation != "y":
        print(ERROR("Please make sure you fulfill the assumptions before running the script. Or input a valid answer."))
        exit()

if __name__ == "__main__":
    # Parse all the arguments
    commandLineArguments = createParser()
    commandLineArguments = commandLineArguments.parse_args()

    # Load the config file
    config = loadYMLConfig(commandLineArguments.config)
    # Get the the general block and set up all the parameters
    generalBlockSettings = config["general"]

    # Inform the user what assumptions are made by the script and ask to confirm, then check them.
    askUserForConfirmation()
    checkAssumptions(generalBlockSettings)

    # Create the executable file
    createExecutable(commandLineArguments.config,commandLineArguments.step)

    # Copy the necessary files to send with the job
    copyCustomClassFilesForSubmission(commandLineArguments.custom_class_path)

    # Set up job parameters and create the submission file
    setupJobParamsDict(generalBlockSettings,commandLineArguments)
    createSubmissionFile(jobParamatersDict)

    # Create input samples files
    commaSeparatedSamples = commandLineArguments.samples.split(",")
    samplesBlock = config["samples"]
    createInputSamplesFile(commaSeparatedSamples,samplesBlock)

    # Submit the jobs
    if not commandLineArguments.dry_run:
        os.system("condor_submit condor_submit.sub")
    else:
        print(DEBUG("Dry run. The submission files have been created in this directory. But the jobs have not been submitted."))
        print("To submit the jobs, run 'condor_submit condor_submit.sub'")