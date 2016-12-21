#ifndef GPS_ANALYZE_H
#define GPS_ANALYZE_H

#include "gps_data.h"

namespace gps {

double unique_dist(const Tracks& data);

void dump_distances(const Tracks& data);

}


#endif // GPS_ANALYZE_H
