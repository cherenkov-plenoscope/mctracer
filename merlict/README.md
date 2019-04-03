# merlict
It is time for a little bit merlict in your life. :wink:

Merlict is a C++ library to simulate the propagation of photons in complex sceneries.
It can simulate geometric optics, but no wave-mechanics.

- reflection
- refraction
- absorbtion on a surface
- absorbtion in a medium
- fresnel reflection
- fresnel transmission

## C++11 library

Merlict is written in C++11 and only depends on the C++ standard-library.

## structure

Merlict is a non-sequential raytracer. This means that the user only has to set up the scenery, but does not have to provide policies for the propagation of photons. All surface-entites are organized in a tree-like-data-structure for efficient photon-surface-intersection-tests. In general, the computing-complexity in merlict scales with O(m*log(n)), where m is the number of photons, and n is the number of surface-entities.

### naming
The term merlict is short for the german term 'mehr Licht' which translates to 'more light', and was supposingly the last words by Johann Wolfgang Goethe. The name was suggested by Maximilian Noethe.
