# Changelog

## FastFrames releases

### Upcoming release

### 3.1.0 <small>July 30, 2024</small>
- [issue #53](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/53): Add options to control the compression setting when creating ntuples.
- [issue #52](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/52): Skip not-defined variables for a sample when creating formula for a variable.
- [issue #51](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/51): Create log directories automatically when using condor submission. Check that paths in the config are relative to the submission position.
- [issue #43](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/43): Add a workaround for the bug in ROOT until it is fixed upstream.
- [issue #50](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/50): Read ONNX model only once at the beginning.
- [issue #49](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/49): Add an option to use simple formuale in a variable definition in the config.

### 3.0.0 <small>July 18, 2024</small>
- [issue #48](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/48): Fix condor_submit.py script to work without a custom FF class.
- [issue #47](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/47): Move order of TLV creation before the custom define calls.
- [issue #44](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/44): Adding an option to specify the name of the listOfSystematics histogram.
- [issue #46](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/46): Fix the problem of not being able to use custom-defined variables as weights when running the ntuple step.
- [issue #45](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/45): Pass Sample objects to the custom class defines. *THIS BREAKS USERS INTERFACE. CHECK THE ASSOCIATED MERGE REQUEST WITH DETAILS ON WHAT NEEDS TO BE UPDATED*.
- [issue #42](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/42): Make ntupling code recognise also variables added via Define() call.
- [issue #41](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/41): Adding script for checking duplicate events in input root files.
- [issue #40](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/40): Adding the possibility to select campaigns as command line input.
- [issue #39](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/39): Fixing issue with inconsistent set of branches in empty input files in merging script.
- [issue #38](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/38): Implementing a new config block for performing k-fold inference on simple ONNX models.
- [issue #19](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/19): Adding support for non-default sumweights histograms.

### 2.2.0 <small>July 1, 2024</small>
- [issue #37](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/37): Making ```MainFrame::defineVariablesNtupleTruth``` non const, for compatibility with ```MainFrame::defineVariablesTruth```. This is interface breaking change (this inteface was added quite recently in [issue #34](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/34))
- [MR !330](https://gitlab.cern.ch/atlas-amglab/fastframes/-/merge_requests/330): Fixing compiler warning originating from onnxruntime library.
- [issue #36](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/36): Adding command line options for specifying path to the output ntuples/histograms.
- [issue #35](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/35): Fixed a problem with `merge_empty_grid_files.py` script which sometimes will generate empty trees with entries.
- [issue #34](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/34): Adding support for truth-level ntuples: selection, branch filtering and custom variables.
- [MR !313](https://gitlab.cern.ch/atlas-amglab/fastframes/-/merge_requests/313): Added wrapper for ONNX + updated tutorial
- [issue #33](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/33): Separate subfolders can be used to individual regions. Please note that this is not supported by TRExFitter.
- [issue #31](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/31): Checking for duplicate DSIDs in sample block, making x-section manager less verbose, removing ```mc23c``` default lumi equal to 1.0 and fixing interface breaking change in StringOperations.
- [issue #30](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/30): Making x-section manager less verbose.
- [issue #29](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/29): Adding support for merging DSIDs from multiple samples into one sample (useful for fake lepton contribution).
- [issue #28](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/28): Adding support for job submission in the Chicago Analysis Facility.
- [issue #26](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/26): Adding support for regular expressions when using the '''exclude_regions''' and '''regions''' options in the samples block.
- [issue #25](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/25): Adding custom options to TRExFitter region block - all options defiend in region block of ```trex_settings``` text file will be copied to the output ```TRExFitter``` config file.

- [issue #27](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/27): Make ntuple processing respect `nominal_only flag`.

### 2.1.0 <small>June 12, 2024</small>
- [issue #24](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/24): Allowing select samples for TRExFitter using trex-fitter settings file. Allowing to override any variable-related option in TRExFitter config
- [issue #23](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/23): Allowing a user to use also samples defined by ```numbering_sequence``` via CLI options
- [issue #18](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/18): Allowing a user to specify more settings for TRExFitter config
- [issue #17](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/17): Excluded regions were not propagated to the generated TRExFitter configuration file
- Several improvemenets for PMG file reading: [issue #15](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/15), [issue #16](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/16), [issue #21](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/21)
- Several fixes for processing multiple truth selections on the same tree: [issue #20](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/20) [issue #22](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/22)

### 2.0.0 <small>May 22, 2024</small>
- [issue #10](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/10): Only read DSIDs from the PMG/TDP files that are actually needed.
- [issue #12](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/12): Fixing issues with processing multiple truth trees.
- [issue #14](https://gitlab.cern.ch/atlas-amglab/fastframes/-/issues/14): Adding ```define_custom_columns_truth``` option under general block in config and removing ```define_custom_columns_truth``` from truth block. Interface breaking change affecting custom classes: method ```defineVariablesTruth``` takes string representing the truth tree as a second argument instead of taking the ```Truth``` object.
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
