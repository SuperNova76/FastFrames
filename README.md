# FastFrames

FastFrames is a package aimed at processing ntuples produced by [TopCPToolkit](https://topcptoolkit.docs.cern.ch/) into histograms or ntuples.
FastFrames rely on ROOT's [RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html) to do the event loop processing.
The code allows users to define their own columns in a minimal way while using all the functionality of the processing.

## Documentation

The package documentation, including detailed instruction how to compile and run the code, can be found [here](https://cern.ch/fastframes).
Doxygen documentation for the code can be found [here](https://atlas-project-topreconstruction.web.cern.ch/fastframesdoxygen/).

## tWZ analysis config

The current config used for the tWZ analysis is `tWZ_test_config.yml`. The output ntuples are then used as inputs into the custom event loop in [gitlab](https://gitlab.cern.ch/atlas-sa-uct/uctanalysistop/-/tree/thobani_dev).

## Support
Support mattermost channel is available. Please, first join the Top Analysis team: [link](https://mattermost.web.cern.ch/signup_user_complete/?id=95983da3f25882a52b0e389f0b042150&md=link&sbr=su) and then join the `Fast Frames support channel`.
