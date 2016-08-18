#include "gps_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace gps{

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

}// namespace gps
