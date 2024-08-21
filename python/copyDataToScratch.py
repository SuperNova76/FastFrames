# Import necesary modules
import os
import sys
import yaml

def loadYMLConfig(pathToConfig):
    with open(pathToConfig, 'r') as stream:
        return yaml.safe_load(stream)
    
def filterFiles(fileList,correctSampleDic):
    filteredFiles = []
    # First filter only the files that have the correct DSID
    DSIDs = correctSampleDic['dsids']
    for ifileList in fileList:
        if any((str(i) in ifileList[0]) for i in DSIDs):
            filteredFiles.append(ifileList)

    # Then filter only the files that have the correct campaign
    resultFiles = []
    campaigns = correctSampleDic['campaigns'] 
    for ifileList in filteredFiles:
        if any((i in ifileList[1]) for i in campaigns):
            resultFiles.append(ifileList[3])

    return resultFiles


def copyDataToScratch(scratchPath, sampleID, metadataPath):
    # Read the config input file and extract the samples
    config = loadYMLConfig('fastframes/python/HTCondorConfig.yml')
    samples = config['samples']

    # Loop over the samples and find the one with the same ID as the input
    correctSample = {}
    for sample in samples:
        if sample['name'] == sampleID:
            # Copy the data to the scratch directory
            correctSample = sample
            break
    
    # Get the list of files in the permanent data directory as a list of lists [DISD, campaign, data_type, path]
    files = []
    with open(metadataPath+'/filelist.txt', 'r') as file:
        for line in file:
            processedLine = line.strip('\n')
            processedLine = [i for i in processedLine.split(' ') if i!='']
            files.append(processedLine)

    # Filter only the relevant files
    filesToBeCopied = filterFiles(files,correctSample)

    # Create the target directory
    os.makedirs('TempData', exist_ok=True)

    # Give user info:
    print('Copying the following files to the scratch directory:')
    for file in filesToBeCopied:
        print(file)
    

    # Copy the data to the scratch directory
    for fileToCopy in filesToBeCopied:
        os.system('cp -r --parents ' + fileToCopy + ' TempData/.')

    # Change the paths in the metadata file
    with open('metadata/filelist.txt', 'r') as file:
        data = file.read()
        data = data.replace(' /', ' ' + scratchPath + '/TempData/')
        with open('metadata/newFilelist.txt', 'w') as file:
            file.write(data)
    # Replace old file with new file
    os.system('mv metadata/newFilelist.txt metadata/filelist.txt')

if __name__ == "__main__":
    copyDataToScratch(sys.argv[1], sys.argv[2], sys.argv[3])    
