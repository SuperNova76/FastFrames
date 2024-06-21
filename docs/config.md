# Config file settings

The config file consists of several blocks.
Available options for each block are documented bellow. Using an option which is not supported will result in error message and program termination.
The options are case sensitive. Example config files can be found in ```test/configs/```.

## `general` block settings
| **Option** | **Value type**   |  **Function** |
| ---------- | ------------     | ------------- |
| debug_level                   | string | Supported values: ERROR, WARNING, DEBUG, INFO and VERBOSE. Default value is ```WARNING``` |
| input_filelist_path           | string | Path to the filelist text file produced by ```produce_metadata_files.py``` script. Can be overwritten with the command-line option `--input_path DIRECTORY` |
| input_sumweights_path         | string | Path to the sum_of_weights text file produced by ```produce_metadata_files.py``` script. Can be overwritten with the command-line option `--input_path DIRECTORY`  |
| output_path_histograms        | string | Path to the output histograms. Default value is empty string, i.e. current directory will be used. |
| output_path_ntuples           | string | Path to the output ntuples. Default value is empty string, i.e. current directory will be used.  |
| default_sumweights            | string | Default sum of weights to be used (can be overridden from Sample block). Default value is ```NOSYS```.        |
| default_event_weights         | string | Default weight of events - i.e. product of mc_weights and all scale factors. The term corresponding to luminosity, sum of weights and x-section will be added automatically. For data, by default this is set to `1.`  |
| default_reco_tree_name        | string |Default name of the reco-level tree. It can be overwritten in Sample block for a given sample.   |
| xsection_files                | list of strings | List of x-section files to use. Both TopDataPreparation and PMG formats are supported. In case of PMG file, the x-section is defined mulltiple times, it will take a value for the latest e-tag. The same DSIDs defined in TopDataPreparation cannot be defined in PMG file, nor multiple times in TopDataPreparation files (except for the case when their values are the same).    |
| create_tlorentz_vectors_for   | list of strings | List of objects (i.e. "jet", "el", "mu" ...) for which TLorentzVectors will be created for each systematic variations. They will always be sorted by pT (default is empty list) |
| use_rvec                      | bool | Whether vector-like variables defined in FastFrames should be made as ```ROOT::VecOps::RVec``` (if set ```True```), or using ```std::vector```  (default is ```False```, i.e. use ```std::vector```) |
| reco_to_truth_pairing_indices | list of strings   | List of branches which should be used to pair reco-level to truth-level trees. Default is ``` ["eventNumber"]```|
| custom_frame_name             | string |If you define your own custom class for the analysis which inherits from the base class, this is the place where you should define its name. Default is empty string: i.e. base class will be used   |
| automatic_systematics         | bool | If set to true, the list of systematic uncertainties in config will be ignored and all uncertainties present in the input ROOT file will be used. Default is ```False```. Can be overriden for given samples.  |
| nominal_only                  | bool | Run nominal only. Default is ```False```. It cannot be true if ```automatic_systematics``` is also true. Can be overriden for given samples. |
| number_of_cpus                | int  | Number of CPUs to use for multithreading. Default is ```1``` |
| min_event                     | int  | If defined, it will process only events with entry index larger or equal than this |
| max_event                     | int  | If defined, it will process only events with entry index smaller than this |
| cap_acceptance_selection      | bool | If set to ```True```, it will keep acceptance and efficiency in interval [0,1]. Default is ```True``` |
| luminosity                    | dict | Dictionary of luminosity values, where key is MC campaign (for example ```mc20d```) and value is luminosity for that campaign. See example config file. Default values for some MC campaigns are defined already in the code, but they can be overridden from here.  |
| define_custom_columns         | list of dicts | Default list of custom columns (branches) to create in data-frame (can be overriden in sample block for a given sample). Each custom column has to have 2 options: ```name``` and ```definition```. |
| define_custom_columns_truth   | list of dicts | Simmilar option to ```define_custom_columns```, but for truth-level trees. Each custom column has to have 3 options: ```name```,  ```definition``` and ```truth_tree``` (this specified for which tree the custom defined should be used). Can be overriden in sample block for a given sample.|
| exclude_systematics  | list of strings | List of systematic uncertainties to skip when automatic systematic option is selected. It supports regular expressions. It can be overriden for individual samples. |
| custom_options | dict | Optional block, which can be used to define additional options, which can then be used from the user's defined custom class (CustomFrame). All values in this block must be convertible to string |
| config_define_after_custom_class | bool | Define custom columns from config after running column definitions from CustomClass. Default is ```False``` |

## `ntuples` block settings

This block is optional.

| **Option** | **Value type** | **Function** |
| ---------- | -------------- | ------------ |
| samples           | list of strings   | List of samples to produce ntuples for (by default, all samples will be used).  |
| exclude_samples   | list of strings   | If specified, all samples except for these will be used. Cannot be used together with ```samples``` option. |
| branches          | list of strings   | List of branches to keep in the output tree. Regular expressions are allowed.   |
| exclude_branches  | list of strings   | List of branches to drop from the output tree. Regular expressions are allowed. |
| selection         | string            | Only the events passing this selection will be saved in the output. By default, the selection is not applied.  |
| regions           | list of strings   | Only the events passing the selection in at least one of the regions will be saved. Cannot be used together with ```selection``` option |
| copy_trees        | list of strings   | Trees to be copied from input to output ntuples.  |

## `cutflows` block settings

This block is optional.

| **Option** | **Value type** | **Function** |
| ---------- | -------------- | ------------ |
| name              | string   | Name of the cutflow.  |
| samples           | list of strings   | If specified, the cutflow will be provided only for the listed samples. If not provided, all samples will be used. |

####   `selections` block inside of the `cutflow` block
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| selection     | string            | Selection to be applied for the cutflow. The order of the selection will follow the order in the config file. If a given sample uses `selection_suffix`, it will be added to the selection string |
| title         | string            | Title for the selection. Will be used as the bin name in the cutflow histogram. |

## `regions` block settings
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| name          | string            | Region name   |
| selection     | string            | Selection     |
| variables     | list of dicts     | List of variables defined for the region  |
| histograms_2d     | list of dicts     | List of 2D histograms between 2 reco-level variables to produce. The dict must have 2 keys: ```x``` and ```y``` for variables on x and y axes. ```numbering_sequence``` block is supported (see details bellow). |
| histograms_3d     | list of dicts     | List of 3D histograms between 3 reco-level variables to produce. The dict must have 3 keys: ```x```, ```Y``` and ```z``` for variables on x, y and z axes. |

####   `variable` block inside of the `region` block
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| name          | string            | Variable name   |
| title         | string            | Title of the histogram. One can specify also title of x and y axis, using a sting like this: ```"histo title;X axis title;Y axis title"```
| definition    | string            | Definition of the variable. All branches affected by systematic uncertainties must have ```_NOSYS``` suffix     |
| binning       | dict              | Binning of the variable   |
| is_nominal_only | bool            | If set to true, only histogram for NOSYS will be produced. Default is ```False``` |
| type          | string            | Allows to tell the code to define the c++ template arguments for the histograms. This prevents JITing thus saving some CPU time and memory. Allowed options are "char", "bool", "int", "long long int","unsigned int", "unsigned long", "unsigned long long int", "float", "double". The vector version of all of these types (except for "vector\<bool>") are also supported - one example is "vector\<float>". ROOT::RVec<type> are also supported, see [here](https://root.cern/doc/master/classROOT_1_1VecOps_1_1RVec.html). If not provided the JITed version will be used. |
| numbering_sequence| list of dicts     | It can be used to automatically add more variables in one block, if they differ by a single value (for example index). More information can be found bellow in ```numbering_sequence``` block description.

####   `binning` block inside of the `variable` block
User has 2 options how to define the binning. Either specify bin edges for irregular binning, or specify number of bins and range of the x-axis for regular binning.

| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| min           | int or float      | x-axis minimum   |
| max           | int or float      | x-axis maximum   |
| number_of_bins| int               | number of bins on x-axis  |
| bin_edges     | list of ints or floats | bin edges for irregular binning |

## `samples` block settings

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| name              | string            | name of the sample |
| dsids             | list of ints      | list od DSIDs corresponding to this sample    |
| campaigns         | list of strings   | list of campaigns for which this sample is defined   |
| simulation_type   | string            | Allowed options: "data", "fullsim", "fastsim" |
| sum_weights       | string            | Nominal sum of weights for this sample - it will also be used for systematic uncertainties that does not define their own some of weights. This option overrides ```default_sumweights``` from general block. |
| event_weights     | string            | Event weight to use. If defined, it will replace ```default_event_weights``` from general block. Expects double as a type for the weight. For data, this defaults to `1.`.   |
| selection_suffix  | string            | Additional selection to use for this sample, for example to split based on flavor composition |
| regions           | list of strings   | List of regions where the sample should be defined. If not specified, use all regions. Regular expressions are supported. |
| exclude_regions   | list of strings   | If specified, all regions except for these will be added for the sample. Cannot be used together with options ```regions```. Regular expressions are supported.   |
| truth             | list of dicts     | List of truth levels which should be used for the given sample    |
| variables         | list of strings   | If specified, only histograms for these variables will be produced for the sample. Regular expressions are supported. |
| exclude_variables | list of strings   | If specified, histograms containing these variables will not be produced for the sample. Regular expressions are supported. This option cannot be combined with ```variables``` |
| exclude_systematics  | list of strings | Overrides default from General block for the given sample |
| automatic_systematics         | bool | Overrides value for this option from general block.  |
| nominal_only                  | bool | Overrides value for this option from general block.  |
| numbering_sequence| list of dicts     | It can be used to automatically add more samples in one block, if they differ by selection, weights etc. More information can be found bellow in ```numbering_sequence``` block description.
| define_custom_columns       | list of dicts  | Overrides this option from general block |
| define_custom_columns_truth | list of dicts  | Overrides this option from general block |
| included_samples | list of strings | This option can be used to merge DSIDs, campaigns and simulation types from other samples into one sample. It can be usuful for example to get one root file for fake lepton contribution, which is originating from multiple nominal samples. This option cannot be used together with ```dsids```, ```campaigns``` and ```simulation_type```. This option will automatically add all unique samples from "included samples" to this sample. |

#### `truth` block inside of the `sample` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| name              | string            | name of the truth level (i.e. particle) |
| produce_unfolding | bool              | If set to true, migration matrices and corrections will be produced   |
| truth_tree_name   | string            | Name of the truth-level tree to be used, i.e. ```truth``` or ```particleLevel```  |
| selection         | string            | Selection |
| event_weight      | string            | Event weight to use for the truth level. Terms corresponding to x-section, luminosity and sum of weights will be added automatically |
| match variables   | list of dicts     | Pair of variables (reco - truth) to be used for the unfolding. The dictionary has to have 2 keys: ```reco``` and ```truth``` for corresponding names of the variables. The truth variable must be defined in this truth block and ```reco``` variable must be defined in at least one region. ```numbering_sequence``` block is supported (see details bellow).  |
| variables         | list of dicts     | The same as ```variable``` block for region |
| pair_reco_and_truth_trees | bool      | Should be truth and reco-level trees be paired? This is needed when truth level variables is needed in reco-level tree, for example to prepare migration matrices or apply reweighting in reco tree, based on a parton-level variable. The default value is ```False``` if ```match variables``` block is empty, otherwise it is ```True``` |

## `systematics` block settings

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| variation         | dict              | Defines up and down variations and how they should be calculated (see the block bellow) |
| numbering_sequence| dict              | Can be used to add systematics with similar names, differing just by the number of the nuisance parameter  (see description bellow)|
| samples           | list of strings   | List of samples where systematic should be used. By default it will be used everywhere except for data    |
| exclude_samples   | list of strings   | If specified, the systematic will be used for all samples except for these and data. Cannot be used together with ```samples``` option. |
| campaigns         | list of strings   | List of campaigns where the systematic should be used |
| regions           | list of strings   | List of regions where the systematic should be used   |
| exclude_regions   | list of strings   | If specified, the systematics will be used in all regions except for these. The option cannot be combined with option ```regions```   |
| numbering_sequence| list of dicts     | It can be used to automatically add more systematic uncertainties in one block, if they differ by a single value (for example index). More information can be found bellow in ```numbering_sequence``` block description.

#### `variation` block inside of `systematics` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| up                | string            | Name of the up variation. The code will check for branches with this suffix and replace ```_NOSYS``` ones by those    |
| down              | string            | The same as ```up``` but for down variation   |
| sum_weights_up    | string            | Sum of weights to use for up variation    |
| sum_weights_down  | string            | Sum of weights to use for down variation    |
| weight_suffix_up  | string            | If specified, the overall weight will be multiplied by this scale factor for up variation. Can be used to define bootstraps. |
| weight_suffix_down| string            | Similar to ```weight_suffix_up``` |


## `numbering_sequence` block inside of `systematics`, `Variable` or `histograms_2d` blocks

In order to automatically add multiple variables, systematic uncertainties or 2D histograms, one can use ```numbering_sequence``` block.
It contains list of dictionaries, each of them having the values defined in the table before. If you want to add for example pT of first 4 jets in an event, you can use just one block and put ```numbering_sequence``` inside it to achieve it. You can also use it when producing migration matrices or 2D histograms. If you define multiple dictionaries inside the ```numbering_sequence``` block, it will create a nested loop over all combinations.

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| replacement_string| string            | If the string is found in ```up``` or ```down``` options (as well as in corresponding sum of weights or weight_suffix) for the systematic variations, it will be replaced by the number. This will add new systematics for each value from min to max (including), where the replacement string will be replaced by the integer number (see lines bellow). Alternativelly, one can specify directly the list of values to be used for replacement |
| min               | int               | Minimal value of the number in the name that should be used for the replacement.   |
| max               | int               | Maximal value of the number in the name that should be used for the replacement.   |
| values            | list of strings   | This can be used instead of ```min``` and ```max``` options, to define a sequence of values to use for replacement, for example ```["170", "172.5", "175"]``` |
