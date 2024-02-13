# TRExFitter config creation

The ```FastFrames``` package contains a python script which allows for automatic creation of config files for [TRExFitter](https://trexfitter-docs.web.cern.ch/).
Even thought most of the things are done automatically based on the FastFrames config file, there are parts of the config which cannot be
derived from FastFrames config and thus the user is advised to check the created config before using it.

## Creating config file for inclusive fit:

```python3 python/produce_trexfitter_config.py --c test/configs/config.yml --o config_trex.config --trex_settings test/configs/trex_settings.yml```

where ```--c``` denotes the config file for FastFrames, ```--o``` is the address of the output TRExFitter config and the last argument, ```--trex_settings``` is optional. It contains additional settings, which will be used in TRExFitter config and cannot be automatically obtained from FastFrames config, such as colors for individual samples, LaTeX names of regions, normalization factors, additional systematic uncertainties etc.

## Creating config file for unfolding:

Creating config file for unfolding is similar to the inclusive fit, however, one has to specify which variable is going to be unfolded at reco and truth level, what truth level is going to be used and which sample is the main signal sample. An example command:

```python3 python/produce_trexfitter_config.py --c test/configs/config.yml --o config_trex.config --u ttbar_FS:parton:Ttbar_MC_t_afterFSR_pt:jet_pt```

where the last argument denoted by ```--u``` is combination of 4 strings separated by ```:```. The meaning of the 4 strings is as follows:

    1) Name of the nominal signal sample

    2) Name of the truth block

    3) Name of the truth-level variable to be unfolded

    4) Name of the reco-level variable to be used for the unfolding

## yaml file with additional TRExFitter settings

This is an optional yaml file, which provides additional settings for ```TRExFitter``` config, which cannot be automatically deduced from fastframes config, such as colors and titles used for individual samples, normalization uncertainties, norm. factors, type of the fit, used regions etc. The example of this file can be found in ```test/configs/trex_settings.yml```.

The overview of the supported blocks and options inside these blocks:

#### Fit block:

The following options are allowed, there override the default values. The meaning of these options (as well for all the other TRExFitter options), ca be found in the [TRExFitter config documentation](https://trexfitter-docs.web.cern.ch/trexfitter-docs/settings/).

  ```FitType:```

  ```FitRegion:```

  ```POIAsimov:```

  ```FitBlind:```

### Unfolding block:

The following options are supported, they override options in ```Unfolding``` block:

```UnfoldNormXSec```

```DivideByBinWidth```

```DivideByLumi```

```LogX```

```LogY```

#### samples block:

This allows a user to override some of the options for a given sample from config (it does not add a new sample).

```name:``` this specifies the name of the sample - for which the block will be applied. It must match the name from the ```fastframes``` config.

```Color:``` this will override both ```FillColor``` and ```LineColor``` options in TRExFitter config

Options to override:

```Title:```

```Type:```

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