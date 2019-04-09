%module merlict

%{
    #include "merlict.h"
%}

%include "std_vector.i"
%include "std_string.i"

%rename(function_mean) merlict::function::mean;
%rename(numeric_mean) merlict::numeric::mean;

%rename(segmented_imaging_reflector_Config) merlict::segmented_imaging_reflector::Config;
%rename(lens_maker_Config) merlict::lens_maker::Config;

%include "merlict.h"

namespace std
{
  %template(VectorOfInts) vector<int>;
  %template(VectorOfDoubles) vector<double>;
  %template(VectorOfVectorOfDoubles) vector< vector<double>>;
  %template(VectorOfSensors) vector< merlict::sensor::Sensor* >;
  %template(VectorOfPhotons) vector< merlict::Photon >;
  %template(VectorOfPhotonArrivals) vector< merlict::sensor::PhotonArrival >;
}
