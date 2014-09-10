# mctraer

mctracer is short for Monte Carlo Ray traceing.

## installation on Ubuntu 14 LTS

### dependencies:
If on Ubuntu (14.04), you can install the dependencies using
synaptic, just install these two:
* libopencv-dev
* libgtest-dev

gtest is a header only library, but in order to use it
one needs to build it first. See this thread on askubuntu
for details:
http://askubuntu.com/questions/145887/why-no-library-files-installed-for-google-test

### compile:
```
#!bash
mkdir build
cd build
cmake ..
make

```




- Google Test 'gtest' for the unit tests
- OpenCV Computer Vision Library for GUI visualisation
- SWIG for python rapping
- cmake
- make
- gcc >= 4.7 for C++11 support

## run mctracer

## run Unit Tests