# merlict-development-kit
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit)


scientific photon propagation and ray tracing in complex sceneries
![img](Readme/fact_and_spider_web.jpg)

In this development-kit, all the sub-projects of the merlict-raytracer are build and tested together.

### dependencies
* git
* g++ >= 6
* cmake
* libopencv-dev >= 2.4.8

### make
```
git clone https://github.com/cherenkov-plenoscope/merlict_development_kit.git
cd merlict_development_kit
mkdir build
```
The `/build` directory is best located side by side to the `/merlict` directory.

```bash
cd build
cmake ..
make -j 8
```

## run
merlict has several executeables. To interacively explore a scenery use `merlict-show`.
```bash
cd ../merlict_viewer/apps/examples
../../../build/merlict-show --scenery fact.json
```

## test
Run the unit tests in the merlict_development_kit base directory to ensure your build is fine.

```bash
./build/merlict-test
```
