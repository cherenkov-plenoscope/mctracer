#include "SimpleTDCQDC.h"

namespace DigitalTDCQDC{

    TimeAndCount find(const Config config, const vector<double> &arrival_moments) {

        TimeAndCount tac;
        tac.time = 0.0;
        tac.count = 0.0;
        return tac;
    }
}//TdC QdC