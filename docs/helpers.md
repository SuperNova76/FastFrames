# Description of helper functions

Several helper functions are provided for custom classes for Define() calls to add new columns.
These are provided [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/blob/main/FastFrames/DefineHelpers.h?ref_type=heads) and [here](https://gitlab.cern.ch/atlas-amglab/fastframes/-/blob/main/FastFrames/MainFrame.h?ref_type=heads).

Note that two distinct signatures exist for each helper function, one which uses `std::vector` for any array structures and another one that uses the [`ROOT::VecOps::RVec`](https://root.cern/doc/master/classROOT_1_1VecOps_1_1RVec.html) class instead.
The latter is a `std::vector`-like class which implements operators acting on entire array and works with methods operating on entire RVec arrays, as defined in [`ROOT::VecOps`](https://root.cern/doc/master/namespaceROOT_1_1VecOps.html).
Users familiar with python ecosystem libraries such as `numpy` and `pandas` may find the behavior somewhat familiar.

It is strongly recommended to stick to either `std::vector` or `Root::VecOps::RVec` accross the entire custom class to avoid column compatibility issues. Additionally, if using `Root::VecOps::RVec` **it is necessary** to set the following configuration option in the general section of YAML config:

```yaml
general:
    use_rvec: True
```

The different helper functions are described below:

## sortedPassedVector
This function takes a vector of TLorentzVectors (actually ROOT::Math::PtEtaPhiEVector) and one or two vectors of chars representing different selections.
The sizes of all vectors have to match. The function returns a new vector of TLorentzVectors only for objects that passed the selection (char == 1 for a given element) also sorted by the object pT.

## sortedPassedIndices
Similar to `sortedPassedVector` but instead returns indices of the original vector and not TLorentzVectors.

## vectorFromIndices
Takes a vector of any objects and a vector of indices (e.g. from `sortedPassedIndices`) and returns a new vector ordered by the indices in the vector passed as second argument.

## systematicDefine
Main workhorse of custom Define() calls. This function take RDataFrame node as the first argument, name of the new variable as the second argument, c++ functor (function defining the variable) as the third argument and a vector of strings representing the names of the columns the functor depends on as the fourth argument.
Code requires that the name of the new columns ends with `_NOSYS`.
The function will automatically call Define() to define systematically varied version of the new column.
The systematic uncertainties are automatically identified only copies for effective systematics will be made.

## systematicDefineNoCheck
Same as `systematicDefine` if the columns that the functor depends on, but if they do not exist the Define() call will be ignored.

## systematicStringDefine
Function that uses string to define the new variable instead of a functor. This is recommended for simple functions, but `systematicDefine` is recommended for complex defines.
Similarly to `systematicDefine`, this will also automatically make systematic copied for the effective systematics.

## numberOfObjects
Function that count the number of objects passing selection (vector of chars) and a given minimum pT passed to the function. Works with one or two selections and with LorentzVectors or directly with vector of floats for the object pt.
