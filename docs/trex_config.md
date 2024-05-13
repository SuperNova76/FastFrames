# TRExFitter config creation

The ```FastFrames``` package contains a python script which allows for automatic creation of config files for [TRExFitter](https://trexfitter-docs.web.cern.ch/).
Even though most of the things are done automatically based on the FastFrames config file, there are parts of the config which cannot be
derived from FastFrames config and thus the user is advised to check the created config before using it.

## Creating config file for inclusive fit:

```python3 python/produce_trexfitter_config.py --c test/configs/config.yml --o config_trex.config --trex_settings test/configs/trex_settings.yml```

where ```--c``` denotes the config file for FastFrames, ```--o``` is the address of the output TRExFitter config and the last argument, ```--trex_settings``` is optional. It contains additional settings, which will be used in the TRExFitter config and cannot be automatically obtained from the FastFrames config, such as colors for individual samples, LaTeX names of regions, normalization factors, additional systematic uncertainties etc.

## Creating config file for unfolding:

Creating the config file for unfolding is similar to the inclusive fit, however, one has to specify which variable is going to be unfolded at reco and truth level, what truth level is going to be used and which sample is the main signal sample. An example command:

```python3 python/produce_trexfitter_config.py --c test/configs/config.yml --o config_trex.config --u ttbar_FS:parton:Ttbar_MC_t_afterFSR_pt:jet_pt```

where the last argument denoted by ```--u``` is combination of 4 strings separated by ```:```. The meaning of the 4 strings is as follows:

    1) Name of the nominal signal sample

    2) Name of the truth block

    3) Name of the truth-level variable to be unfolded

    4) Name of the reco-level variable to be used for the unfolding

## yaml file with additional TRExFitter settings

This is an optional yaml file, which provides additional settings for the ```TRExFitter``` config, which cannot be automatically deduced from the fastframes config, such as colors and titles used for individual samples, normalization uncertainties, norm. factors, type of the fit, used regions etc. The example of this file can be found in ```test/configs/trex_settings.yml```.

The overview of the supported blocks and options inside these blocks:

## Selecting regions

In order to use only some ```region + variable``` pairs from the ```fastframes``` config in the resulting ```TRExFitter``` config, one can use ```--regions``` flag with comma separated list of ```region + variable``` pairs to be used. The ```region + variable``` syntax is ```region_name + "_" variable_name``` - these are the names of "regions" in the resulting TRExFitter config. Regular expressions are supported.

Few examples:
```
python3 python/produce_trexfitter_config.py --config test/configs/config_testing.yml --regions *met_met

python3 python/produce_trexfitter_config.py --config test/configs/config_testing.yml --regions Electron_jet_pt,Muon_met_met
```

#### Job block:

All set options in the yaml config are included in the TRExFitter config, but the following options have default values set:

```Name```
```HistoPath```
```Lumi```
```ImageFormat```
```ReadFrom```
```HistoChecks```

If unfolding is run, default values for the following options are set:

```AcceptancePath``` ```MigrationPath``` ```SelectionEffPath```

If no unfolding is run, a default value for the following option is set:

```POI```

#### Fit block:

The following options are allowed, there override the default values. The meaning of these options (as well for all the other TRExFitter options), can be found in the [TRExFitter config documentation](https://trexfitter-docs.web.cern.ch/trexfitter-docs/settings/).

  ```FitType:```

  ```FitRegion:```

  ```POIAsimov:```

  ```FitBlind:```

  ```UseMinos:```

### Unfolding block:

The following options are supported, they override options in ```Unfolding``` block:

```UnfoldNormXSec```

```DivideByBinWidth```

```DivideByLumi```

```LogX```

```LogY```

#### samples block:

This allows a user to override some of the options for a given sample from config (it does not add a new sample). Samples can be excluded via the `General` block.

```name:``` this specifies the name of the sample - for which the block will be applied. It must match the name from the ```fastframes``` config.

```Color:``` this will override both ```FillColor``` and ```LineColor``` options in TRExFitter config

Options to override:

```Title:```

```Type:```

If provided by the user, the following option will be added to the sample definition in the TRExFitter config:

```Template:```

#### Systematics block:

This adds new systematic uncertainty to the TRExFitter config file

```name:``` specifies the name of the systematic uncertainty. All other options will be copied to the TRExFitter config. An example of Systematics block with luminosity and x-section uncertainty:

```
Systematics:
    - name: "Luminosity"
      Title: "Luminosity"
      Type: "OVERALL"
      OverallUp: 0.0083
      OverallDown: -0.0083
      Category: "Instrumental"

    - name: "Wjets_xsec"
      Title: "W+jets cross-section"
      Type: "OVERALL"
      OverallUp: 0.1
      OverallDown: -0.1
      Samples: "Wjets"
      Category: "Theoretical"
```
The `numbering_sequence` is supported for the fields `name`, `Title`,`HistoFolderNameUp` and `HistoFolderNameDown`.

#### NormFactors block:

Similarly to ```Systematics``` block, the name identifies the norm. factor, which will be added to the config, the other options will be copied to the ```TRExFitter``` config.

An example of such block:

```
NormFactors:
    - name: "mu_signal"
      Title: "#mu(signal)"
      Nominal: 1
      Min: -100
      Max: 100
      Samples: "ttbar_FS"
```

#### CustomBlocks block:

In some cases it might be usefull to define some additional blocks in TRExFitter config, for example ghost samples. One can use ```CustomBlocks``` block to achieve this.
In order to add custom samples, just add ```Samples:``` under ```CustomBlocks``` in the trex-fitter settings file and then define list of custom samples there. Each sample must have ```name``` which will be
used in TRExFitter config. The other options will just be copied to TRExFitter config, without checking if that option is implemented. It is up to the user to provide reasonable keys and values.

#### Morphing block:

Defining a yaml `Morphing` block, will add `Morphing` settings to the TRExFitter config. All the passed options will be copied to the TRExFitter config.

An example of such a block:

```
Morphing:
  FitFunction: QUADRATIC
```

#### General block:

Through the `General` yaml block, in the `exclude_samples` option, a list with strings of samples to exclude from the TRExFitter config can be passed (regex is supported).

An example of such a block:

```
General:
  exclude_samples: ["ttbar_dilep_Gt2p0_top_FS","bb4l_FS"]
```

#### Region block:

Through the `Region` yaml block, the `Type` of the region can be specified. The region block needs a `name` field, which corresponds to automatically generated name for the region ```region_name + "_" variable_name``` (regex is supported).

An example of such a block:

```
Regions:
  - name: "Signal_emu_jet_pt_sum"
    Type: "VALIDATION"
  - name: "Signal_emu_jet1.*"
    Type: "VALIDATION"
```