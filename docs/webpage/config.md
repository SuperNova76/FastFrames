# Config file settings

The config file consists of several blocks.
Available options for each block are documented bellow. Using an option which is not supported will result in error message and program termination.
The options are case sensitive.

## `general` block settings
| **Option** | **Value type** | **Function** |
| ---------- | ------------ | ------------ |
| debug_level                   | string | Supported values: ERROR, WARNING, DEBUG, INFO and VERBOSE |
| input_filelist_path           | string | Path to the filelist text file produced by ```produce_metadata_files.py``` script |
| input_sumweights_path         | string | Path to the sum_of_weights text file produced by ```produce_metadata_files.py``` script   |
| output_path_histograms        | string | Path to the output histograms |
| output_path_ntuples           | string | Path to the output ntuples    |
| default_sumweights            | string | Default sum of weights        |
| default_event_weights         | string | Default weight of events - i.e. product of mc_weights and all scale factors. The term corresponding to luminosity, sum of weights and x-section will be added automatically  |
| default_reco_tree_name        | string | Default name of the reco-level tree   |
| xsection_files                | list of strings | List of x-section files to use (example of these files can be found in ```data/```)       |
| create_tlorentz_vectors_for   | list of strings | List of objects (i.e. "jet", "el", "mu" ...) for which TLorentzVectors will be created for each systematic variations. The will always be sorted by pT |
| reco_to_truth_pairing_indices | list of strings  | List of branches which should be used to pair reco-level to truth-level trees. |
| custom_frame_name             | string    | If you define your own custom class for the analysis which inherits from the base class, this is the place where you should define its name   |
| automatic_systematics         | bool | If set to true, the list of systematic uncertainties in config will be ignored and all uncertainties present in the input ROOT file will be used   |
| nominal_only                  | bool | Run nominal only   |
| number_of_cpus                | int  | Number of CPUs to use for multithreading |
| luminosity                    | dict | Dictionary of luminosity values, where key is MC campaign (for example ```mc20d```) and value is luminosity for that campaign. See example config file .   |
| define_custom_columns         | list of dicts | Default list of custom columns to create in data-frame (can be overriden in sample block for a given sample). Each custom column has to have 2 options: ```name``` and ```definition``` |


## `ntuples` block settings
This block is optional.
| **Option** | **Value type** | **Function** |
| ---------- | ------------ | ------------ |
| samples           | list of strings   | List of samples to produce ntuples for (by default, all samples will be used)  |
| exclude_samples   | list of strings   | If specified, all samples except for these will be used. Cannot be used together with ```samples``` option.
| branches          | list of strings   | List of branches to keep in the output tree   |
| exclude_branches  | list of strings   | List of branches to drop from the output tree |
| selection         | string            | Only the events passing this selection will be saved in the output    |
| regions           | list of strings   | Only the events passing the selection in at least one of the regions will be saved. Cannot be used together with ```selection``` option |
| copy_trees        | list of strings   | Trees to be copied from input to output ntuples   |
| histograms_2d     | list of dicts     | List of 2D histograms between 2 reco-level variables to produce. The dict must have 2 keys: ```x``` and ```y``` for variables on x and y axes. |


## `regions` block settings
| **Option**    | **Value type**    | **Function** |
| ------------- | ----------------- | ------------ |
| name          | string            | Region name   |
| selection     | string            | Selection     |
| variables     | list of dicts     | List of variables defined for the region  |

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
| campaigns         | list of strings   | list of campaigns for which this sample isi defined   |
| simulation_type   | string            | Allowed options: "data", "mc", "AFII" |
| event_weights     | string            | Event weight to use. If defined, it will replace ```default_event_weights``` from general block   |
| selection_suffix  | string            | Additional selection to use for this sample, for example to split based on flavor composition |
| regions           | list of strings   | List of regions where the sample should be defined. If not specified, use all regions. |
| exclude_regions   | list of strings   | If specified, all regions except for these will be added for the sample. Cannot be used together with options ```regions```   |
| truth             | list of dicts     | List of truth levels which should be used for the given sample    |

#### `truth` block inside of the `sample` block

| **Option**        | **Value type**    | **Function** |
| ----------------- | ----------------- | ------------ |
| name              | string            | name of the truth level (i.e. particle) |
| produce_unfolding | bool              | If set to true, migration matrices and corrections will be produced   |
| truth_tree_name   | string            | Name of the truth-level tree to be used, i.e. ```truth``` or ```particleLevel```  |
| selection         | string            | Selection |
| event_weight      | string            | Event weight to use for the truth level. Terms corresponding to x-section, luminosity and sum of weights will be added automatically |
| match variables   | list of dicts     | Pair of variables (reco - truth) to be used for unfolding. The dictionary has to have 2 keys: ```reco``` and ```truth``` for corresponding names of the variables. The truth variable must be defined in this truth block and ```reco``` variable must be defined in at least one region  |
| variables         | list of dicts     | The same as ```variable`````` block for region |
| define_custom_columns | list of dicts | The same as ```define_custom_columns ``` in general block |


