# merlict-development-kit

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

scientific photon propagation and ray tracing in complex sceneries
![img](Readme/fact_and_spider_web.jpg)

In this development-kit, all the sub-projects of the merlict-raytracer are build and tested together.

### dependencies
* git
* g++ >= 5.3
* cmake
* libopencv-dev >= 2.4.8

### make
```
git clone https://github.com/cherenkov-plenoscope/mctracer.git
mkdir build
```
The /build directory is best located side by side to the /mctracer directory.

```bash
cd build
cmake ../mctracer
make
```

## run
The mctracer has several executeables. To interacively explore a scenery use mctShow.
```bash
merlict-show --scenery fact.json
```

## test
Run the unit tests in the mctracer/Tests directory to ensure your build is fine.

```bash
cd mctracer
../build/merlict-test
```
