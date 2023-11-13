# Fast Frames documentation

FastFrames is an histograming and ntuple reprocessing framework. The core code is written in C++ with a python interface around it.

## How to checkout and compile the code

To run the code, you firstly need to check out the repository and compile the C++ part of the package. In order to do so:

```
git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git

cd fastframes

mkdir build

cd build

cmake ../.

make # alternatively "make -jN", where N is number of CPUs you want to use for compilation

cd ..
```

At this point the code should be compiled. Make sure you are using at least 6.28 version of the ROOT.

## How to run the code:

#### Obtaining metadata files:

In order to run the code, you will firstly need to produce text files with metadata. There are 2 of them, the first one contains the list of all ROOT files belonging to the same sample (same DSID, campaign and simulation type - AFII/FS).
The other file contains sum of weights for each sample and each systematic variation.

In order to produce the metadata files:

```
python3 python/produce_metadata_files.py --root_files_folder <path_to_root_files_folder> --output_path <path_to_output_folder>
```

where you have to specify the path to the folder with the ROOT files that you are going to reprocess. The second argument is optional, it is the path to the folder where metadata files will be stored.
By default they will be in the same folder as input ROOT files. In the config file used in the next steps, you will have to specify the addresses of these metadata files.

#### Running histograming part:

To produce the histograms from your ROOT files, one has to set up a config. You can find example configs in ```test/configs/``` and you can find all available options also in the documentation webpage.
To run the histogramming part:

```
python3 python/FastFrames.py --config <path to your config> --step h
```

If you do not want to run over all samples from your config, but just over a part of them, you can specify the list of these samples using optional argument ```samples```:

```
python3 python/FastFrames.py --config <path to your config> --step h --samples sample1,sample2,sample3
```

#### Running ntuple part:

Running ntuple part is similar to running histogramming part, you just need to specify ```n``` step in terminal:

```
python3 python/FastFrames.py --config <path to your config> --step n
```

