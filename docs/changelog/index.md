# Changelog

## FastFrames releases

### Upcoming release
- [issue #10](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/10): only read DSIDs from the PMG/TDP files that are actually needed.
- [issue #14](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/14): Adding ```define_custom_columns_truth``` option under general block in config and removing ```define_custom_columns_truth``` from truth block. Interface breaking change in ```MainFrame```, methods ```processTruthHistograms2D``` and ```addTruthVariablesToReco```.
- [issue #15](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/15): Fixing the issue with duplicate x-section info in TDP file

### 1.2.0 <small>May 15, 2024</small>
- Adjusting XSectionManager to be able to work with PMG x-section text files.
- Add the ability to modify automatic systematics in the TRexFitter config.
- When merging empty files, check if the trees are present in all files.
- [issue #8](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/8): add selection suffix to the Cutflow selections.
- Add the capability of building the metadata from files stored in the GRID.
- Add Sample and Region optional settings to trexfitter config.
- [issue #7](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/7): add a script to submit jobs to using an HTCondor batch system.

### 1.1.0 <small>April 25, 2024</small>
- [issue #6](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/6): add per-sample sum of weights option.
- Set AddDirectory to false.
- [issue #5](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/5): add 3D histogram support.

### 1.0.0 <small>April 17, 2024</small>
- First release for TopWorkshop 2024 tutorial.
