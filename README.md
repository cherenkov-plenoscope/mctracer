# mctraer

mctracer is short for Monte Carlo Ray Traceing.

## installation on a fresh Ubuntu 14 LTS Desktop
Installation tested on a fresh Ububtu 14.04 LTS VM on 19 Sep 2015.

### tools needed to get and build mctracer:
* git
* g++ >= 4.8
* cmake

### dependencies of mctracer:
* libopencv-dev >= 2.4.0
* libgtest-dev

opencv can be installed with apt-get when running Ubuntu >= 14. Otherwise one has to install it locally by hand. This is easiest done by getting it from the git repo:

```
https://github.com/itseez/opencv
```

then, for instance
```
#!bash
cd ~/opencv
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make

```
Last thing is to export $OpenCV_DIR to the CMAKE_INSTALL_PREFIX, if it is not /usr/local e.g.:
```
export OpenCV_DIR=<CMAKE_INSTALL_PREFIX>
```
This ensures that CMakeLists.cmake finds the correct version of OpenCV.


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

### set up directories and get mctracer:
```
#!bash
mkdir yourDirectorieToPlayWithMctracer
cd yourDirectorieToPlayWithMctracer
git clone https://Relleums@bitbucket.org/Relleums/mctracer.git
mkdir build
```
The /build directory is best located side by side to the /mctracer directory.

|-> yourDirectorieToPlayWithMctracer
	|-> mctracer
	|-> build

### compile:
```
#!bash
cd build
cmake ../mctracer
make

```

## run mctracer:
```
#!bash
cd mctracer/scenery
../../build/mctracer --geometry hans.xml --render

```

## run Unit Tests:
The unit tests are meant to run in the mctracer/test directorie because they are looking for files in there to do IO tests.

```
#!bash
cd mctracer/test
../../build/mctracerTest

```