# Introduction to SystemC: Tutorial Project

## Preface

Given exercises mostly were taken from the book of David Black "SystemC: From Ground Up" 2nd Edition.
However, project includes instructions how to set up the SystemC working environment with manual build or using Conan,
to be able to concentrate on studying framework and not spending much time for initial configuration.

## Building SystemC from scratch

We use SystemC version 2.3.4, which could be cloned directly from the Accellera official repo

```
git clone --depth 1 --branch 2.3.4_pub_rev_20190614 https://github.com/accellera-official/systemc.git
mkdir build-cmake
cd build-cmake
cmake .. -DCXXFLAGS="-std=c++17"
cmake --build .
make
touch ../docs/tlm/ChangeLog
sudo make install
```

## Using conan
Building the project from source is simple. 
First you need to install [conan.io](https://conan.io/) 
according to the [instructions](http://docs.conan.io/en/latest/installation.html) and setup the Minres remote:

```
pip install --user conan
conan profile new default --detect
export PATH=${PATH}:$HOME/.local/bin
```

Now install required packages from the conanfile (SystemC, CCI and SCV library), 
build it from sources if required, build the project and run it:

```
mkdir build-cmake
cd build-cmake
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
bin/systemc_example
```

If you would like to analyze the recording output further just download the latest release of 
[SCViewer](https://github.com/Minres/SCViewer/releases) to open the transaction_example.txlog.

## Notes

Don't forget to add '-s compiler.libcxx=libstdc++11' to the conan call or changing compiler.libcxx,
otherwise you might have run into GCC ABI incompatibility, or add
```
compiler.libcxx=libstdc++11
```

to `$HOME/.conan/profiles/default`
