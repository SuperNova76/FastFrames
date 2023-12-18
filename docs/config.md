# Config file settings

The config file consists of several blocks.
Available options for each block are documented bellow. Using an option which is not supported will result in error message and program termination.
The options are case sensitive. Example config files can be found in ```test/configs/```.

## `general` block settings
| **Option** | **Value type**   |  **Function** |
| ---------- | ------------     | ------------- |
| debug_level                   | string | Supported values: ERROR, WARNING, DEBUG, INFO and VERBOSE. Default value is ```WARNING``` |
| input_filelist_path           | string | Path to the filelist text file produced by ```produce_metadata_files.py``` script |
| input_sumweights_path         | string | Path to the sum_of_weights text file produced by ```produce_metadata_files.py``` script   |
| output_path_histograms        | string | Path to the output histograms. Default value is empty string, i.e. current directory will be used. |
| output_path_ntuples           | string | Path to the output ntuples. Default value is empty string, i.e. current directory will be used.  |
| default_sumweights            | string | Default sum of weights to be used (can be overridden from Sample block). Default value is ```NOSYS```.        |
| default_event_weights         | string | Default weight of events - i.e. product of mc_weights and all scale factors. The term corresponding to luminosity, sum of weights and x-section will be added automatically  |
| default_reco_tree_name        | string |Default name of the reco-level tree. It can be overwritten in Sample block for a given sample.   |
| xsection_files                | list of strings | List of x-section files to use (example of these files can be found in ```data/```). Default value is ``` ["data/XSection-MC16-13TeV.data"]``` . The same DSID cannot be defined multiple times in the same file, nor in multiple files.    |
| create_tlorentz_vectors_for   | list of strings | List of objects (i.e. "jet", "el", "mu" ...) for which TLorentzVectors will be created for each systematic variations. They will always be sorted by pT (default is empty list) |
| reco_to_truth_pairing_indices | list of strings   | List of branches which should be used to pair reco-level to truth-level trees. Default is ``` ["eventNumber"]```|
| custom_frame_name             | string |If you define your own custom class for the analysis which inherits from the base class, this is the place where you should define its name. Default is empty string: i.e. base class will be used   |
| automatic_systematics         | bool | If set to true, the list of systematic uncertainties in config will be ignored and all uncertainties present in the input ROOT file will be used. Default is ```False```.   |
| nominal_only                  | bool | Run nominal only. Default is ```False```  |
| number_of_cpus                | int  | Number of CPUs to use for multithreading. Default is ```1``` |
| min_event                     | int  | If defined, it will process only events with entry index larger or equal than this |
| max_event                     | int  | If defined, it will process only events with entry index smaller than this |
| luminosity                    | dict | Dictionary of luminosity values, where key is MC campaign (for example ```mc20d```) and value is luminosity for that campaign. See example config file. Default values for some MC campaigns are defined already in the code, but they can be overridden from here.  |
| define_custom_columns         | list of dicts | Default list of custom columns (branches) to create in data-frame (can be overriden in sample block for a given sample). Each custom column has to have 2 options: ```name``` and ```definition```. |
| exclude_systematics  | list of strings | List of systematic uncertainties to skip when automatic systematic option is selected. It supports regular expressions. It can be overriden for individual samples. |

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


## `regions` block settings
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| name          | string            | Region name   |
| selection     | string            | Selection     |
| variables     | list of dicts     | List of variables defined for the region  |
| histograms_2d     | list of dicts     | List of 2D histograms between 2 reco-level variables to produce. The dict must have 2 keys: ```x``` and ```y``` for variables on x and y axes. |

####   `Variable` block inside of the `region` block
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| name          | string            | Variable name   |
| title         | string            | Title of the histogram. One can specify also title of x and y axis, using a sting like this: ```"histo title;X axis title;Y axis title"```
| definition    | string            | Definition of the variable. All branches affected by systematic uncertainties must have ```_NOSYS``` suffix     |
| binning       | dict              | Binning of the variable   |

####   `Binning` block inside of the `variable` block
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
| simulation_type   | string            | Allowed options: "data", "fullsim", "AFII" |
| event_weights     | string            | Event weight to use. If defined, it will replace ```default_event_weights``` from general block   |
| selection_suffix  | string            | Additional selection to use for this sample, for example to split based on flavor composition |
| regions           | list of strings   | List of regions where the sample should be defined. If not specified, use all regions. |
| exclude_regions   | list of strings   | If specified, all regions except for these will be added for the sample. Cannot be used together with options ```regions```   |
| truth             | list of dicts     | List of truth levels which should be used for the given sample    |
| variables         | list of strings   | If specified, only histograms for these variables will be produced for the sample. Regular expressions are supported. |
| exclude_variables | list of strings   | If specified, histograms containing these variables will not be produced for the sample. Regular expressions are supported. This option cannot be combined with ```variables``` |
| exclude_systematics  | list of strings | Overrides default from General block for the given sample |

#### `truth` block inside of the `sample` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| name              | string            | name of the truth level (i.e. particle) |
| produce_unfolding | bool              | If set to true, migration matrices and corrections will be produced   |
| truth_tree_name   | string            | Name of the truth-level tree to be used, i.e. ```truth``` or ```particleLevel```  |
| selection         | string            | Selection |
| event_weight      | string            | Event weight to use for the truth level. Terms corresponding to x-section, luminosity and sum of weights will be added automatically |
| match variables   | list of dicts     | Pair of variables (reco - truth) to be used for the unfolding. The dictionary has to have 2 keys: ```reco``` and ```truth``` for corresponding names of the variables. The truth variable must be defined in this truth block and ```reco``` variable must be defined in at least one region  |
| variables         | list of dicts     | The same as ```variable``` block for region |
| define_custom_columns | list of dicts | The same as ```define_custom_columns ``` in general block |

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

#### `variation` block inside of `systematics` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| up                | string            | Name of the up variation. The code will check for branches with this suffix and replace ```_NOSYS``` ones by those    |
| down              | string            | The same as ```up``` but for down variation   |
| sum_weights_up    | string            | Sum of weights to use for up variation    |
| sum_weights_down  | string            | Sum of weights to use for down variation    |
| weight_suffix_up  | string            | If specified, the overall weight will be multiplied by this scale factor for up variation. Can be used to define bootstraps. |
| weight_suffix_down| string            | Similar to ```weight_suffix_up``` |


#### `numbering_sequence` block inside of `systematics` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| replacement_string| string            | If the string is found in ```up``` or ```down``` options (as well as in corresponding sum of weights or weight_suffix) for the systematic variations, it will be replaced by the number. This will add new systematics for each value from min to max (including), where the replacement string will be replaced by the integer number (see lines bellow) |
| min               | int               | Minimal value of the number in the name that should be used for the replacement.   |
| max               | int               | Maximal value of the number in the name that should be used for the replacement.   |

