# mctracer

mctracer is short for Monte Carlo Ray Tracing.

## installation on a fresh Ubuntu 14 LTS Desktop
Installation tested on a fresh Ububtu 14.04 LTS VM on 19 Sep 2015.

### tools needed to get and build mctracer:
* git
* g++ >= 4.8
* cmake

### dependencies of mctracer:
* libopencv-dev >= 2.4.8

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
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/your/favorite/path ..
make && make install

```
Last thing is to export $OpenCV_DIR to the CMAKE_INSTALL_PREFIX, if it is not /usr/local e.g.:
```
export OpenCV_DIR=<CMAKE_INSTALL_PREFIX>
```
This ensures that CMakeLists.txt finds the correct version of OpenCV.

### set up directories and get mctracer:
```
#!bash
mkdir yourDirectorieToPlayWithMctracer
cd yourDirectorieToPlayWithMctracer
git clone https://Relleums@bitbucket.org/Relleums/mctracer.git
mkdir build
```
The /build directory is best located side by side to the /mctracer directory.

- yourDirectoryToPlayWithMctracer
	- mctracer
	- build

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
../../build/mctracer --geometry fact.xml --render

```

## run Unit Tests:
The unit tests are meant to run in the mctracer/test directory because they are looking for files in there to do IO tests.

```
#!bash
cd mctracer/test
../../build/mctracerTest

```

----


# Upcoming: Python support #

## Install ##

```
#!bash
git clone <URL>
cd mctracer
mkdir build && cd build
cmake .. && make
cd ..
source this_pytracer.sh
```

The last line simply sets your PYTHONPATH correctly, if you want this to be persistent, edit your `.bashrc` accordingly.

## Try it out ##

### Trace some *corsika* photons through an XML scenery.

```
#!bash
python pytracer/propagate.py ./scenery/fact.xml ./test/telescope.dat
```
outputs just a PNG. Have a look at the bottom of `propagate.py` in order to learn how to do more with the output.

### Fly through a scene, written in Python

```
#!bash
python pytracer/fact_scene.py
```
**note:** there's a bug, click on any black surface and seg-fault!

### Trace *corsika* photons through a Python scene.

!Missing!

# Epoch 160310: making calibration files #

./build/mctPlenoscopeCalibration --config Main/Examples/PlenoscopeCalibration/lightfield_calibration_input_card.xml --output Main/Examples/PlenoscopePropagation/

The results are 4:

 * sub_pixel_positions.txt (~26M)
 * overview.txt (~2k)
 * the_big_lebowsky.lftc (huge)
 * sub_pixel_statistics.txt (50MB)  <-- this is needed and checked in.

# Plenoscope: Making NSB events #

mkdir out
./build/mctPlenoscopeNsbPropagation -c Main/Examples/PlenoscopePropagation/propagation_input_card.xml -o ./out/

enjoy