#ifndef GPS_UTILS_H
#define GPS_UTILS_H

namespace gps{

double distance_haversine(double lat1, double lon1, double lat2, double lon2);

double distance_flat(double lat1, double lon1, double lat2, double lon2);

}

#endif // GPS_UTILS_H
