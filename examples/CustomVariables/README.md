# Example of a class for adding custom variables

## How to build

Create a folder (can be outside of the main repository)

```
mkdir MyCustomFrame
cd MyCustomFrame
mkdir build install
```

Now copy the contents of this folder
```
cp -r * /your/path/MyCustomFrame/
```

If you want to use a different name than `CustomFrame` for the code, you need to change the `CMakeLists.txt` content appropriately (just renaming) and also the corresponding files and folder. Do not forget about `Root/LinkDef.h`!

Now you need to compile the code. For the cmake step, you need to tell the code where you want to install the library, so that ROOT can find it during run time and also where you installed `FastFrames`.

```
cd build
cmake -DCMAKE_PREFIX_PATH=~/Path/to/your/fastframes/install -DCMAKE_INSTALL_PREFIX=/where/you/want/to/install ../
```

E.g. if you installed `FastFrames` into `/home/FastFranes/install` and you want to install the custom library to `/MyCustomFrame/install` (default) do

```
cmake -DCMAKE_PREFIX_PATH=~/home/FastFrames/install -DCMAKE_INSTALL_PREFIX=../install ../
```

You can also adjust the `CMakeLists.txt` file to put the _absolute_ path for the `FastFrame`s install by adding
```
set (CMAKE_PREFIX_PATH "~/home/FastFrames/install" CACHE PATH )
```
And then you do not have to use `-DCMAKE_PREFIX_PATH=~/home/FastFrames/install` as an argument for the cmake call (you still need to use the `-DCMAKE_INSTALL_PREFIX` argument).

Now, compile and install the code
```
make
make install
```

Finally, you need to export `LD_LIBRARY_PATH` to tell ROOT where to look for the library, e.g.
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/MyCustomFrame/install/lib
```

You can add this to your .bashrc file.

## Creating empty class template
To simplify the process of preparing the custom class, you can have a look a this repostiory that allows you to clone and then rename an empty custom class template: [link](https://gitlab.cern.ch/atlas-amglab/FastFramesCustomClassTemplate). This repository is also added as a submodule to this repository.
