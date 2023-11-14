# FastFrames


## Install requirements (locally)

```
sudo apt-get install python3-dev gcc cmake build-essential python3-pip libboost-all-dev
```

## Get the code
Create a new folder for the repository, e.g.
```
mkdir FastFrames
cd FastFrames
mkdir build install
```

To get the code, use the following command
```
git clone ssh://git@gitlab.cern.ch:7999/atlas-amglab/fastframes.git
```

## Compile the code
```
cd build
```

Now you need to tell the code where you want to install the library, if you use the recommended structure, then do
```
cmake -DCMAKE_INSTALL_PREFIX=../install ../fastframes
```

But you can replace `../install` with any folder that you want to use for the installation

Now compile the code
```
make -j
make install
```

Now you need to set the `LD_LIBRARY_PATH` variable to the folder where you installed the library, .e.g assume the library is in `/home/FastFranes/install`, then do
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/FastFrames/install/lib
```

You can add the export to your `.bashrc` file so this will be done automatically for you every time a new shell is created.

## Documentation:

The package documentation can be found here: https://cern.ch/fastframes

Doxygen documentation can be found here: https://atlas-project-topreconstruction.web.cern.ch/fastframesdoxygen/
