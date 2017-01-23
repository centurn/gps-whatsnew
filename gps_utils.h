#ifndef GPS_UTILS_H
#define GPS_UTILS_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include "gps_data.h"

namespace bg = boost::geometry;
BOOST_GEOMETRY_REGISTER_POINT_2D(gps::Waypoint, double, cs::spherical_equatorial<bg::degree>, lon, lat)

namespace gps{

double distance(const Waypoint& p1, const Waypoint& p2);

double distance_haversine(double lat1, double lon1, double lat2, double lon2);

double distance_flat(double lat1, double lon1, double lat2, double lon2);

double segmentDistance(const Segment& seg);

}

#endif // GPS_UTILS_H
