# Fast Frames documentation

FastFrames is an histograming and ntuple reprocessing framework. The core code is written in C++ with a python interface around it.

## How to checkout and compile the code

#### Locally (assuming you have ROOT and Boost libraries installed)

To run the code, you firstly need to check out the repository and compile the C++ part of the package. In order to do so:

The following structure is suggested:
```
cd YourFavoriteFolder
mkdir build install

git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git

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

#### Using lxplus(-like) machine
The instructions are the same as in the Local build, but you can setup ROOT and Boost using StatAnalysis:
```
setupATLAS
asetup StatAnalysis,0.2,latest
```

This will setup an appropriate version of ROOT (you can check the ROOT version with `root --version`)


## How to run the code:

#### Obtaining metadata files:

In order to run the code, you will firstly need to produce text files with metadata. There are 2 of them, the first one contains the list of all ROOT files belonging to the same sample (same DSID, campaign and simulation type - AFII/FS).
The other file contains sum of weights for each sample and each systematic variation.

In order to produce the metadata files:

```
produce_metadata_files.py --root_files_folder <path_to_root_files_folder> --output_path <path_to_output_folder>
```

where you have to specify the path to the folder with the ROOT files that you are going to reprocess. The second argument is optional, it is the path to the folder where metadata files will be stored.
By default they will be in the same folder as input ROOT files. In the config file used in the next steps, you will have to specify the addresses of these metadata files.

#### Running histograming part:

To produce the histograms from your ROOT files, one has to set up a config. You can find example configs in ```test/configs/``` and you can find all available options also in the documentation webpage.
To run the histogramming part:

```
FastFrames.py --config <path to your config> --step h
```

If you do not want to run over all samples from your config, but just over a part of them, you can specify the list of these samples using optional argument ```samples```:

```
FastFrames.py --config <path to your config> --step h --samples sample1,sample2,sample3
```

You can also specify ```max_event``` or ```min_event``` options from general part of the config using the command line.

#### Running ntuple part:

Running ntuple part is similar to running histogramming part, you just need to specify ```n``` step in terminal:

```
FastFrames.py --config <path to your config> --step n
```
#### Other command line options:

In order to split the sample into multiple jobs, you can use the following command line option:

```
--split_n_jobs <N jobs total> --job_index <current job index>
```

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
