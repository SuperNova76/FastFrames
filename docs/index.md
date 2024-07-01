# Fast Frames documentation

FastFrames is an histograming and ntuple reprocessing framework. The core code is written in C++ with a python interface around it.

## How to checkout and compile the code

#### Linux local (assuming you have ROOT and Boost libraries installed)

To run the code, you firstly need to check out the repository and compile the C++ part of the package. In order to do so:

The following structure is suggested:
```
cd YourFavoriteFolder
mkdir build install

git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git

cd build
cmake -DCMAKE_INSTALL_PREFIX=../install/ ../fastframes
# alternatively, point the path to the install folder

make # alternatively "make -jN", where N is number of CPUs you want to use for compilation

# install the library
make install
```

Then you will need to set the `LD_LIBRARY_PATH` to the installation path using the produced setup.sh script

```
source setup.sh
```

At this point the code should be compiled. Make sure you are using at least 6.28 version of the ROOT.

To install Boost libraries, do (or similarly on different linux platforms)
```
sudo apt-get install libboost-all-dev
```

If you do not have git large file storage (LFS) installed on your machine, you need to use the following steps:

```
sudo apt install git-lfs #or similar
git lfs install
git lfs pull
```

This should download the ROOT files that are used for the CI tests, but can also be used to run the code as an example

#### MacOS local

First, install ROOT, CMake and the BOOST libraries, you can use the package manager of your preference, in this tutorial we will use [homebrew](https://brew.sh)

```
brew install root
brew install cmake
brew install boost-python3
```
Now, create a folder to store the project files:
```
mkdir FF && cd FF # Pick your favourite name if you do not like FF :)
```
After this step, we clone the FastFrames repository:

```
git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git
```

We can now configure the build, compile and install:
```
cmake -S fastframes -B build -DCMAKE_INSTALL_PREFIX=install
# -S should point to the folder that contains the FastFrames source code and -B to the build directory.

cmake --build build -jN --target install
# N is the number of cores you want to use for the compilation.
```

Finally, you will need to set the `LD_LIBRARY_PATH` to the installation path using the produced setup.sh script

```
source build/setup.sh
```

To download the ROOT files that are used for the CI tests, and can also be used to run the code as an example do:

```
git lfs pull
```

#### Using lxplus(-like) machine
The instructions are the same as in the Local build, but you can setup ROOT and Boost using StatAnalysis:

When using Alma Linux 9 (lxplus default)

```bash
setupATLAS
asetup StatAnalysis,0.3.0
```

When using CentOS 7 (lxplus7)

```bash
setupATLAS
asetup StatAnalysis,0.2.5
```

This will setup an appropriate version of ROOT (you can check the ROOT version with `root --version`)

## How to run the code:


#### Merging empty files and removing them

The RDataFrame cannot handle well files with empty trees, but one can obtain such files from GRID production.
In order to solve this problem, one can merge the empty files from grid with one non-empty file and then remove the original files which have been merged (to avoid double counting). There is a script to do this in the ```FastFrames``` repository:

You can run the script using the following command:

```
python3 python/merge_empty_grid_files.py --root_files_folder <path to the folder with ROOT files>
```

it will go recursivelly over all subfolders, merge all empty files belonging to the same DSID, campaign and type (fastsim/fullsim/data) with one non-empty file. After that, it will remove the original files in order to avoid double counting in the later steps. If all all files from the same DSID+campaign+type are empty, it will merge all of them together (FastFrames can handle this case).

#### Obtaining metadata files:

In order to run the code, you will firstly need to produce text files with metadata. There are 2 of them, the first one contains the list of all ROOT files belonging to the same sample (same DSID, campaign and simulation type - fastsim/fullsim).
The other file contains sum of weights for each sample and each systematic variation.

In order to produce the metadata files:

```bash
python3 python/produce_metadata_files.py --root_files_folder <path_to_root_files_folder> --output_path <path_to_output_folder>
```

where you have to specify the path to the folder with the ROOT files that you are going to reprocess. The second argument is optional, it is the path to the folder where metadata files will be stored.
By default they will be in the same folder as input ROOT files. In the config file used in the next steps, you will have to specify the addresses of these metadata files.

##### Building the metadata from files stored in the GRID:

---
**NOTE**

Using FastFrames in this way should be restricted to cases where the RSEs and the processing machines are at the same location. Otherwise, data reads will go through the internet and this will take a lot of bandwith and will make the processing very slow.

---

If the input files are stored in the GRID, in a particular Rucio Storage Element (RSE), FastFrames is also capable of building the metadata. Just use the same script from above but pass a different argument:

```bash
python3 produce_metadata_files.py --grid_datasets <path_to_text_files_with_grid_datasets> --output_path <path_to_output_folder>
```

The second argument is the path to a text file containing all the relevant GRID containers, one example is:

```
user.dbaronmo:user.dbaronmo.601229.PhPy8EG.DAOD_PHYS.e8514_s4162_r14622_p5980.Common-FTAG-NTuples-v0.3.0_output
user.dbaronmo:user.dbaronmo.601230.PhPy8EG.DAOD_PHYS.e8514_s4162_r14622_p5980.Common-FTAG-NTuples-v0.3.0_output
user.dbaronmo:user.dbaronmo.601348.PhPy8EG.DAOD_PHYS.e8514_s4162_r14622_p5980.Common-FTAG-NTuples-v0.3.0_output
```

The script will ask for the RSE where the files are stored. For the example containers you should provide:

```
UKI-NORTHGRID-MAN-HEP_LOCALGROUPDISK
```

---
**NOTE**

To get the list of all the GRID RSEs you can do:

```
rucio list-rses
```

---

#### Running histograming part:

To produce the histograms from your ROOT files, one has to set up a config. You can find example configs in ```test/configs/``` and you can find all available options also in the documentation webpage.
To run the histogramming part:

```bash
python3 python/FastFrames.py --config <path to your config> --step h
```

One can also use ```c``` instead of ```config```

If you do not want to run over all samples from your config, but just over a part of them, you can specify the list of these samples using optional argument ```samples```:

```bash
python3 FastFrames.py --config <path to your config> --step h --samples sample1,sample2,sample3
```

You can also specify ```max_event``` or ```min_event``` options from general part of the config using the command line.

#### Running ntuple part:

Running ntuple part is similar to running histogramming part, you just need to specify ```n``` step in terminal:

```bash
python3 FastFrames.py --config <path to your config> --step n
```

#### Other command line options:

In order to split the sample into multiple jobs, you can use the following command line option:

```bash
--split_n_jobs <N jobs total> --job_index <current job index>
```

In order to specify a different metadata file directory (containing `filelist.txt` and `sum_of_weights.txt`), you can use the following option (or `-i` instead of `--input_path`):

```bash
--input_path <path to directory>
```

In order to merge the output ROOT files from all jobs into one file, one can use the following command:

```bash
python3 python/merge_jobs.py --c <config address>
```

One can use the following options to override paths to the output ntuples/histograms from the config file:

```--output_path_histograms <path>```

```--output_path_ntuples <path>```

#### Submitting jobs to the HTCondor batch service

FastFrames provides a useful script to submit jobs to the [CERN HTCondor batch service](https://batchdocs.web.cern.ch/concepts/index.html). After loging into an Lxplus node go the directory you have FastFrames under:

```
cd <your_fastframes_path>
```
the relevant script, `condor_submit.py`, is under the `fastframes/python/` directory:
```
cd fastframes/python
```

To submit jobs run the script with the options that best adapt to your current workflow. If you do
```
python3 condor_submit.py --help
```
the following description will appear:
```
python3 condor_submit.py --help
usage: condor_submit.py [-h] [-c CONFIG] [--samples SAMPLES] [--step {h,n}] [--custom-class-path CUSTOM_CLASS_PATH] [--max-time MAX_TIME] [--dry-run] [--chicago]

options:
  -h, --help            show this help message and exit
  -c CONFIG, --config CONFIG
                        Path to the yml config file.
  --samples SAMPLES     A comma separated list of samples to run. One job is created per listed sample. Default: all samples listed in the yml config.
  --step {h,n}          Step to run: 'n' (ntuples) or 'h' (histograms). Default: 'h'
  --custom-class-path CUSTOM_CLASS_PATH
                        Path to the custom class used in the config file (if used). Default: None
  --max-time MAX_TIME   Maximum time for the job to run. Default: 1h
  --dry-run             Creates the execution and submission environment without sending the jobs to HTCondor. Useful for debugging.
  --chicago             Use this flag if you are running the jobs in the Chicago Analysis Facility.
```

Let's run an example containing a custom fastframes class - more details about the custom class in the next section. Here the custom class is located two levels up from the submission directory, i.e., at the same level than the fastframes source code.
```
python3 condor_submit.py -c ../../MyAnalysis/ConfigYML/sim_calib_config.yml --step h --custom-class-path ../../MyAnalysis
```

Now the scripts asks to confirm whether the following assumptions are true:
```
This script submits jobs to the HTCondor batch system from an lxplus-like machine...
Please read the following carefully... you are about to potentially submit a large number of jobs to the HTCondor batch system.
For a correct execution. This script assumes the following:
1.  The script is run from the fastframes/python directory
2.  The build and install directories of FastFrames are two levels up from the current directory, i.e, at the same level than the FastFrames source code.
3.  The metadata files are stored in a folder called metadata at the same level than the FastFrames source code.
4. (OPTIONAL) The path to the output files is an absolute path in EOS or AFS preferentially.
Are these assumptions correct? [y/n]
```

Essentially, the submission script expects the following directory hierarchy:
```
build (dir)
install (dir)
metadata (dir)
fastframes (dir)
    python (dir)
        condor_submit.py # You must be here when executing the script.
```
and your paths to the FastFrames outputs should be absolute paths in AFS or EOS. Therefore, in your general block your should have a config like:
```
general:
  input_filelist_path: "../../metadata/filelist.txt" # Notice how the metadata is stored two levels up from here inside the metadata directory.
  input_sumweights_path: "../../metadata/sum_of_weights.txt"
  output_path_histograms: "/afs/cern.ch/user/E/ExampleUser/public/results/" # Absolute path in AFS
  output_path_ntuples: "/eos/user/E/ExampleUser/results/" # Or, absolute path in EOS
```

After you acknowledge that the previous assumptions are true you will see the following message:
```
Submitting job(s)..
X job(s) submitted to cluster 12776566.
```
where X is the number of jobs submitted. This number is dependent on the arguments you provided when executing `condor_submit.py`. If you did not provide a comma separated list of samples the script will submit one job for each sample listed in your yml config.

To monitor the status of your jobs you can do `condor_q`. You will see a similar output to:
```
condor_q


-- Schedd: bigbird14.cern.ch : <137.138.44.75:9618?... @ 04/26/24 10:08:55
OWNER    BATCH_NAME      SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
dbaronmo ID: 12776566   4/26 10:05      _      _      2      2 12776566.0-1

Total for query: 8 jobs; 0 completed, 0 removed, 8 idle, 0 running, 0 held, 0 suspended
Total for dbaronmo: 8 jobs; 0 completed, 0 removed, 8 idle, 0 running, 0 held, 0 suspended
Total for all users: 22412 jobs; 2240 completed, 15 removed, 15542 idle, 4540 running, 75 held, 0 suspended
```

---
**NOTE**

* You can kill jobs if necessary by doing `condor_rm JOB_ID`.
* Be careful with how CPU cores you request per job. The more resources you request, the later your jobs will start executing.
* Make sure you compile the code to include new changes before submiting the jobs.
* If you are working in the Chicago AF, you need to provide the appropriate `--chicago` flag to produce the correct configuration for this cluster.

---

## Adding custom class for custom Define() call

When using RDataFrame, all variables ("columns" in RDataFrame language) used in the selection (Filter in RDataFrame language), variable definition or weights need to be defined.
RDataFrame allows to define variables using the `Define()` method on each RDataFrame node. There are two variations of the method:

```c++
auto newNode = node.Define("variable_GeV", "variable/1e3");
```

In the version above, the new variable, `variable_GeV` is created with a formula provided by a string. In the example, this takes a variable called `variable` and divides it by 1e3.
This version of Define() allows easy interface, but it has to be Just-In-Time (JIT) compiled.
Another version uses pure c++ code
```c++
auto newNode = node.Define("variable_GeV", [](const float value){return value/1e3}, {"variable"});
```

This version is functionally equivalent to the previous one, it also creates a new node called `variable_GeV`, by passing a c++ functor(lambda) and a list of varaibles it depends on as the third argument.
In this way, any new columns can be added. FastFrames allows users to only modify this part of the code while letting the rest of the code do the hard work.
Technically this is done by overriding the main class and leading the new class on runtime.

#### Example of custom class for custom define
An example of the custom class is provided [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/tree/main/examples/CustomVariables?ref_type=heads)

#### Compile the custom class
It is important that the new class needs to be compiled and linked against the base class.
This can be achieved by:
```
mkdir MyCustomFrame
cd MyCustomFrame
mkdir build install
```

And providing the new class in the new folder, e.g. you can copy the contents fo the linked folder:
```
cp -r * /your/path/MyCustomFrame/
```
or you can use the template for this [here](https://gitlab.cern.ch/atlas-amglab/FastFramesCustomClassTemplate)

If you want to use a different name than CustomFrame for the code, you need to change the CMakeLists.txt content appropriately (just renaming) and also the corresponding files and folder. Do not forget about Root/LinkDef.h!

Now you need to compile the code. For the cmake step, you need to tell the code where you want to install the library, so that ROOT can find it during run time and also where you installed FastFrames.
```
cd build
cmake -DCMAKE_PREFIX_PATH=~/Path/to/your/fastframes/install -DCMAKE_INSTALL_PREFIX=/where/you/want/to/install ../
```
E.g. if you installed FastFrames into /home/FastFranes/install and you want to install the custom library to /MyCustomFrame/install (default) do

```
cmake -DCMAKE_PREFIX_PATH=~/home/FastFrames/install -DCMAKE_INSTALL_PREFIX=../install ../
```

You can also adjust the CMakeLists.txt file to put the absolute path for the FastFrames install by adding

```
set (CMAKE_PREFIX_PATH "~/home/FastFrames/install" CACHE PATH )
```
And then you do not have to use -DCMAKE_PREFIX_PATH=~/home/FastFrames/install as an argument for the cmake call (you still need to use the -DCMAKE_INSTALL_PREFIX argument).

Now, compile and install the code

```
make
make install
```
Finally, you need to export set the `LD_LIBRARY_PATH` to tell ROOT where to look for the library:

```
source setup.sh
```

#### Using the custom class
To use the custom class, you need to set the name of the class in the config file in the general block: `custom_frame_name` to the name of the library, code will then search the `LD_LIBRARY_PATH` to find the appropriate library.
Have a look at how the new library can be used to add new columns: [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/blob/main/examples/CustomVariables/Root/CustomFrame.cc?ref_type=heads).
Note that there are many helper functions that only require you to provide the new columns for the nominal values and will be automatically copied for the systematic varaitions for you!
