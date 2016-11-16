#include "gps_utils.h"
#include "gps_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>

namespace bg = boost::geometry;
BOOST_GEOMETRY_REGISTER_POINT_2D(gps::Waypoint, double, cs::spherical_equatorial<bg::degree>, lon, lat)

namespace gps{

double distance_boost(const Waypoint& p1, const Waypoint& p2){
    return bg::distance(p1, p2) * 6371.0;
}

// Source: http://rosettacode.org/wiki/Haversine_formula#C
static const double R = 6371;
static const double TO_RAD = (3.1415926536 / 180);
double distance_haversine(double th1, double ph1, double th2, double ph2)
{
    double dx, dy, dz;
    ph1 -= ph2;
    ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;

    dz = sin(th1) - sin(th2);
    dx = cos(ph1) * cos(th1) - cos(th2);
    dy = sin(ph1) * cos(th1);
    return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

double distance_flat(double lat1, double lon1, double lat2, double lon2) {
    double lat_avg = (lat1 + lat2) / 2 * TO_RAD;
    double dx = 111.3 * cos(lat_avg) * (lon1 - lon2);
    double dy = 111.3 * (lat1 - lat2);
    return sqrt(dx * dx + dy * dy);
};

}// namespace gps
