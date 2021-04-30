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

Additionally, you can use -p=1 to use log printing.

### Files before/after

All of these files were compacted using ```./Compact -f <file name>```.
Their entropy was calculated using ```./Entropy -f <file name>```.

| File type                         | Details                                                                   | Size (bytes) | Entropy           | Metric Entropy    | Size (after) | Entropy (after)   | Metric Entropy (after) |
|-----------------------------------|---------------------------------------------------------------------------|--------------|-------------------|-------------------|--------------|-------------------|------------------------|
| HTML document in natural language | https://web.njit.edu/~cm395/theBeeMovieScript/                            |       56.302 | 1,43371519421E-08 | 1,01880607869E-22 |       32.778 | 9,28024437330E-09 |      6,59475886339E-23 |
| MP4 file                          | MP4 conversion of this video: https://www.youtube.com/watch?v=Ks7U9VP_XeI |    4.336.593 | 1,01502849870E-06 | 7,21269990141E-21 |    4.332.938 | 1,01445418599E-06 |      7,20900857527E-21 |
| Encrypted file                    | MP4 file encrypted with GPG (GnuPG 2.2.27, RSA 4096).                     |    4.288.174 | 1,00462489030E-06 | 7,13912133607E-21 |    4.288.175 | 1,00451898228E-06 |      7,13758715808E-21 |
| ZIP file                          | All of the above files, zipped (Zip 3.0).                                 |    8.597.597 | 1,95269928901E-06 | 1,38748052534E-20 |    8.597.598 | 1,95272573495E-06 |      1,38751882078E-20 |
