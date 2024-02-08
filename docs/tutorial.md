# Tutorial

The following tutorial demonstrates how to process a ROOT ntuple produced by [TopCPToolkit](https://topcptoolkit.docs.cern.ch/).
The tutorial covers the initial setup, compilation, adding custom variables, config preparation, running the code as well as automatically generating configuration file for [TRExFitter](https://gitlab.cern.ch/TRExStats/TRExFitter).
For the code documentation, please see [doxygen](https://atlas-project-topreconstruction.web.cern.ch/fastframesdoxygen/).

## Setup
The tutorial setup assumes you are working on an Alma Linux 9 machine with an access to cvmfs, such as lxplus machines.

After logging in to this machine, create a new folder called FastFramesTutorial:

```
mkdir FastFramesTutorial
cd FastFramesTutorial
```

Now, clone the repository using ssh:
```
git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git FastFrames
```

And create the input, build and install folders
```
mkdir build install input
```

Now, download the ROOT file produced by TopCPToolkit:
```
cd input
cp /eos/user/r/ravinab/TopCPToolkit/example_files/tutorial_file.root .
cd ../
```

The ROOT file is obtained by running a single-lepton ttbar selection on a ttbar non-allhadronic file using the 2022-like setup (mc23 campaign).

### Compilation
Run the cmake step:
```
cmake -DCMAKE_INSTALL_PREFIX=../install/ ../FastFrames/
```

Compile the code
```
make -jN # where N is the number of CPUs you can use, e.g. -j4
```

Install the code
```
make install
```

And, finnaly, setup the paths with:
```
source setup.sh
```

## Generating file metadata
First we need to generate the metedata needed for futher processing.
A set of python scripts is provided that will search a given folder and all of it subfolders for ROOT files, inspect them and then create two text files.
The first file contains the list of all the ROOT files, the paths to the individual files, the DSID (6 digit unique identifier for each MC sample) and the simulation type (ful lsimulation or fast simulation).
The second file contains the total sumweights for each weight for a given DSID, campaign and the simulation type.
To generate these files, do
```
cd FastFrames
python python/produce_metadata_files.py --root_files_folder ../input/
```

You can inspect the new txt files in `../input/` 

## Adding custom variables
As the format of the ROOT file makes direct histogramming difficult, it is very likely you will need to use your own code to add more variables/columns to the input file.
FastFrames allows seamless extension of the code code by providing a short custom class that allows users to add new columns in a convenient way.

### Skeleton setup
Change directory to the folder where `build`, `install` etc folders are:
```
cd ../
```

A helper repository is provided that contain a skeleton code needed for the custom class.
Clone it with
```
git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/FastFramesCustomClassTemplate.git TutorialClass
```

Where the second argument is the name of the folder you want to generate (the name of the class).
Now do the renaming using the scripts provided:
```
cd TutorialClass
./renameFiles.sh TutorialClass
```

Now, you can remove the script as well as the `.git` folder
```
rm renameFiles.sh
rm -rf .git
```
To make it easier to turn the folder into your own repository (recommended).
You can also let us know if you have your own repostiory and we can add it to the list of custom classes so that the new users can have a look how other analyses use it, see [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/tree/main/examples?ref_type=heads). 

Now, let us compile the custom class and link to the main FastFrames code
```
mkdir build install
cd build
cmake -DCMAKE_PREFIX_PATH=/afs/cern.ch/user/t/tdado/FastFramesTutorial/install -DCMAKE_INSTALL_PREFIX=../install ../ # UPDATE THE PATH TO THE FASTFRAMES INSTALL
```

Compile the code
```
make
```

Finally, set the path with
```
source setup.sh
```

The last step, setting the paths is needed in every new shell. The paths are needed for the FastFrames code to find your custom class.

### Adding leading jet pT variable
Now we have everything prepared to start adding some new variables that we can use for plotting.
Note that in RDataFrame, every variable you want to plot needs to be added via `Define()`. This includes the variables for applying selection or weights.
In this example, we will add a leading jet pT variable so that we can plot it later on.

The changes relevant for histogramming go into `defineVariables` method of your class. This is the method we will modify.
There are two general ways in RDataFrame how to add varaibles via Define(). The first one uses strings to define a formula the second approach uses c++ pointer to functions (functors). The c++ version leads to a better performance as it will be compiled, while the string version will be only Just-In-Time(JIT) compiled.

The following conceptual steps are needed to add a leading jet pT variable:

1. Take the vector of jet pT (`jet_pt_<SYSTNAME>`) and a vector of jet selections (`jet_select_or_<SYSTNAME>`) and take only the jets that pass the selection
2. Sort the vector based on jet pT - the vectors are never sorted - they cannot be as systematic variations could change the order
3. Take the first element and store this as a new variable

The steps needed could be implemented using RDataFrame's Define() functions, but this would have to be repeated for all systematic variation making it very inconvenient. Fort his, FastFrames has a method that does this for you! You only need to define this for the nominal variables, everything else will be automatically provided for you. The relevant function is `MainFrame::systematicDefine`. Let us now add the leading jet pT variable. The following code can be added to the `defineVariables` method:
```c++
  auto SortedTLVs = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                       const std::vector<char>& selected) {
    return DefineHelpers::sortedPassedVector(fourVec,selected);
  };

  auto LeadingTLV = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec) {
    return fourVec.empty() ? ROOT::Math::PtEtaPhiEVector{-999, -999, -999, -999} : fourVec.at(0);
  };

  auto tlvPtGEV = [](const ROOT::Math::PtEtaPhiEVector& tlv) {
    return tlv.pt()/1.e3;
  };

  // add sorted passed jet TLV vector
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "sorted_jet_TLV_NOSYS",
                                         SortedTLVs,
                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS"});

  // add leading jet TLV
  mainNode = MainFrame::systematicDefine(mainNode,
                                        "jet1_TLV_NOSYS",
                                        LeadingTLV,
                                        {"sorted_jet_TLV_NOSYS"});

  mainNode = MainFrame::systematicDefine(mainNode,
                                         "jet1_pt_GEV_NOSYS",
                                         tlvPtGEV,
                                         {"jet1_TLV_NOSYS"});
```

The code above does what we need, but in a slightly more convenient/efficient way. Let us go throught the different parts

```c++
  auto SortedTLVs = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec,
                       const std::vector<char>& selected) {
    return DefineHelpers::sortedPassedVector(fourVec,selected);
  };
```

The above code snipper defined a lambda (`std::function`) that takes a vector of lorentz vectors, a vector of chars and then returns a sorted vector (based on pT) for selected elements (where the char is == 1). The code uses a helper function `DefineHelpers::sortedPassedVector` defined in FastFrames, see [this](https://gitlab.cern.ch/atlas-amglab/fastframes/-/blob/main/Root/DefineHelpers.cc?ref_type=heads).

```c++
  auto LeadingTLV = [](const std::vector<ROOT::Math::PtEtaPhiEVector>& fourVec) {
    return fourVec.empty() ? ROOT::Math::PtEtaPhiEVector{-999, -999, -999, -999} : fourVec.at(0);
  };
```

The above code takes a vector of lorentz vectors and of it is not empty it returns the first element.

```c++
  auto tlvPtGEV = [this](const ROOT::Math::PtEtaPhiEVector& tlv) {
    return tlv.pt()/1.e3;
  };
```
The above code takes a lorentz vector and then returns the pT cimponent divided by 1000.

```c++
  // add sorted passed jet TLV vector
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "sorted_jet_TLV_NOSYS",
                                         SortedTLVs,
                                         {"jet_TLV_NOSYS", "jet_select_or_NOSYS"});
```
The above code adds a new variable `sorted_jet_TLV_NOSYS` based on the `SortedTLVs` lambda (function) and it relies on the two columns(variables): `jet_TLV_NOSYS` and `jet_select_or_NOSYS`, wher the first variable is provided by FastFrames (configurable) for convenience.

```c++
  // add leading jet TLV
  mainNode = MainFrame::systematicDefine(mainNode,
                                        "jet1_TLV_NOSYS",
                                        LeadingTLV,
                                        {"sorted_jet_TLV_NOSYS"});
```
The above code adds a variable `jet1_TLV_NOSYS` based on lambda `LeadingTLV` and it relies on the varaible we have just created, `sorted_jet_TLV_NOSYS`.

```c++
  mainNode = MainFrame::systematicDefine(mainNode,
                                         "jet1_pt_GEV_NOSYS",
                                         tlvPtGEV,
                                         {"jet1_TLV_NOSYS"});
```

The above code adds a variable `jet1_pt_GEV_NOSYS` based on lambda `tlvPtGEV` and as an input uses `jet1_TLV_NOSYS`. 
This is the varaible we wanted to plot.

You could add the variable directly in one lambda however, the presented aprpoach has multiple advantanges:

* If you want to also add leading jet eta, you can use the `jet1_TLV_NOSYS` without rerunning the other steps that would have added computatin overhead (the filtering and sriting would have to be rerun)
* You can also easily look at the second leading jet using `sorted_jet_TLV_NOSYS`

Please check also other helper functions, they are quite useful!
  
!!! tip "Using standard Define()"
    You can still use the standard RDataFrame Define(). This will add the varaibles only for the nominal copy, but if you want to only plot the histogram for nominal only this is okay.

!!! tip "Debugging"
    Debugging in these custom functions can be tricky. Standard printout methods work, but due to multithreading, the outputs might be shuffled. It is recommended to switch to just one thread when debugging.

## Preparing the config file and running

We are now ready to write the configuration file that will steer the processing of the ntuple into histograms.
Go back to the main FastFrames repository
```
cd ../../FastFrames/
```

And create the following `tutorial_config.yml` file:

```yaml
general:
  debug_level: DEBUG

  # paths
  input_filelist_path: "../input/filelist.txt"
  input_sumweights_path: "../input/sum_of_weights.txt"
  output_path_histograms: "../output/"

  # weights
  default_sumweights: "NOSYS"
  default_event_weights: "weight_mc_NOSYS * weight_beamspot * weight_pileup_NOSYS * weight_jvt_effSF_NOSYS * weight_btagSF_DL1dv01_FixedCutBEff_85_NOSYS * globalTriggerEffSF_NOSYS * weight_leptonSF_tight_NOSYS"

  default_reco_tree_name: "reco"

  # path to the file with cross-sections
  xsection_files: ["data/XSection-MC16-13TeV.data"]

  # name of the custom class
  custom_frame_name: "TutorialClass"

  # automatic systematics?
  automatic_systematics: True
  nominal_only: False

  # which objects to create lorentz vectors for
  create_tlorentz_vectors_for: ["jet"]

  number_of_cpus: 4


# define regions
regions:
  - name: "Electron"
    selection: "pass_ejets_NOSYS"
    variables:
      - name: "jet1_pt_GEV_NOSYS"
        title : "Leading jet p_{T}; Leading jet p_{T} [GeV];Events"
        definition: "jet_pt_GeV_NOSYS"
        binning:
          min: 0
          max: 300
          number_of_bins: 10
      - name: "met_met"
        title : "MET;E_{T}^{miss} [MeV];Events"
        definition: "met_met_NOSYS"
        binning:
          bin_edges: [0,20000,60000,80000,140000,250000]

  - name: "Muon"
    selection: "pass_mujets_NOSYS"
    variables:
      - name: "jet1_pt_GEV_NOSYS"
        title : "Leading jet p_{T}; Leading jet p_{T} [GeV];Events"
        definition: "jet_pt_GeV_NOSYS"
        binning:
          min: 0
          max: 300
          number_of_bins: 10
      - name: "met_met"
        title : "MET;E_{T}^{miss} [MeV];Events"
        definition: "met_met_NOSYS"
        binning:
          bin_edges: [0,20000,60000,80000,140000,250000]


# define samples
samples:
  - name: "ttbar_FS"
    dsids: [601229]
    campaigns: ["mc21a"]
    simulation_type: "fullsim"
```

Let us now go through the some of the less intuitive configurations options.

```yaml
  # weights
  default_sumweights: "NOSYS"
  default_event_weights: "weight_mc_NOSYS * weight_beamspot * weight_pileup_NOSYS * weight_jvt_effSF_NOSYS * weight_btagSF_DL1dv01_FixedCutBEff_85_NOSYS * globalTriggerEffSF_NOSYS * weight_leptonSF_tight_NOSYS"
```
The above block sets the weights for normalisation. `default_sumweights` tell the code which sumweights to use for normalisation. `default_event_weights` tells the code what the formula for the event weights is. Note that the code will automatically add the luminosity, cross-section and sumweights values.

```yaml
  # path to the file with cross-sections
  xsection_files: ["data/XSection-MC16-13TeV.data"]
```
The above block tells the code where to look for the file that contains the cross-section for files

```yaml
  # name of the custom class
  custom_frame_name: "TutorialClass"
```
The above block tells the code what is the name of the custom class. This is needed to find the correct libraries!

```yaml
  # automatic systematics?
  automatic_systematics: True
  nominal_only: False
```
The above block is optional and can be used to tell the code to process all systematics or only run nominal even when you provide systematics in the config file.

```yaml
  # which objects to create lorentz vectors for
  create_tlorentz_vectors_for: ["jet"]
```
The above block tells the code to generate the `TLV` lorentz vector variables that we used in the custom class. You can set this for different objects like electron `el` or muons `mu`.

The region block for one region:
```yaml
  - name: "Electron"
    selection: "pass_ejets_NOSYS"
    variables:
      - name: "jet1_pt_GEV_NOSYS"
        title : "Leading jet p_{T}; Leading jet p_{T} [GeV];Events"
        definition: "jet_pt_GeV_NOSYS"
        binning:
          min: 0
          max: 300
          number_of_bins: 10
      - name: "met_met"
        title : "MET;E_{T}^{miss} [MeV];Events"
        definition: "met_met_NOSYS"
        binning:
          bin_edges: [0,20000,60000,80000,140000,250000]
```
specifies the region definition. It has the name of the region (to be used in the output) and a selection. The selection can be a formula but only simple formuale are encouraged (e.g. simple AND or OR).
Then, each region has a list of variables for which histograms will be created.
The variables have a name (to be used in the output), title (the format is `title: x axis title: y axis title`), definition which is a name of the column, i.e. it cannot be a formula. and the bining.

The sample definition:
```yaml
# define samples
samples:
  - name: "ttbar_FS"
    dsids: [601229]
    campaigns: ["mc21a"]
    simulation_type: "fullsim"
```

Defines the list of samples. Each sample requires a name (that will be used in the file name for each sample), list of DSIDs (these will be combined!), list of campaigns (these will be combined) and the simulation type.

Please, check the list of all config options [here](https://atlas-project-topreconstruction.web.cern.ch/fastframesdocumentation/config/).
As well as a config file used in out CI tests [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/blob/main/test/configs/config.yml?ref_type=heads).

The yaml format allows to use some "tricks". E.g. it is possible to copy a block of settings (such as variables):
  
```yaml
Defaults: &defaults
  Company: foo
  Item: 123

Computer:
  <<: *defaults
  Price: 3000 
```

### Histogram processing (1D)

### Histogram processing (unfolding-like)

### Ntuple processing

## Generating the TRExFitter config file
