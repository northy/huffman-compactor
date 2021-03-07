## Dependencies

To compile the repository you need:

- C++ >=20 compiler
- CMake >=3.0

The repository contains FireCPP as a submodule.

## Instructions

To clone and compile the repository you need to:

```sh
git clone https://github.com/northy/huffman-compactor.git --recurse-submodules
cd huffman-compactor && mkdir build && cd build && cmake ..
make -j
```

The executables take the -f argument, which takes a file path as input (except for Decompact, that also takes the tree file path as input with -t).

Additionally, you can use -p=0 to supress log printing.
