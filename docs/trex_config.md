# TRExFitter config creation

The ```FastFrames``` package contains a python script which allows for automatic creation of config files for ```TRExFitter```.
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