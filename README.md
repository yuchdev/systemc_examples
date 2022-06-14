# SystemC From Ground Up Exercises

## Building SystemC from scratch

TODO

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
```

## Notes

Don't forget to add '-s compiler.libcxx=libstdc++11' to the conan call or changing compiler.libcxx,
otherwise you might have run into GCC ABI incompatibility, or add

```
compiler.libcxx=libstdc++11
```

to `$HOME/.conan/profiles/default`
