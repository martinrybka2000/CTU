# CTU

## Zadanie rekrutacyjne CUT - cpu usage tracker, Maj 2022

## Usage

For launching the program clone the repository and open the /build directory  
There you can find a compiled version to lauch it:
```
./cut
```
  
For compiling type:  
```
cmake ..
```
```
make
```
```
./cut
```
If an error occures after `cmake ..` then try to install the [ncurses](https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/) library  

For building tests go into /test dir and type:
```
mkdir build && cd build
```
```
cmake ..
```
```
make
```
```
ctest
```

