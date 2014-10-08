# mctraer

mctracer is short for Monte Carlo Ray traceing.

## installation on Ubuntu 14 LTS

### dependencies:
If on Ubuntu (14.04), you can install the dependencies using
synaptic, just install these:

* libopencv-dev
* libgtest-dev

gtest is a header only library, but in order to use it
one needs to build it first. See this thread on askubuntu
for details:
http://askubuntu.com/questions/145887/why-no-library-files-installed-for-google-test
```
#!bash
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/
```


* swig
* python-dev

### compile:
```
#!bash
mkdir build
cd build
cmake ..
make

```
## run mctracer
```
#!bash
cd mctracer/scenery
../../build/mctracer

```

## run Unit Tests