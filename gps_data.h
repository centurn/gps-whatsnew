#ifndef GPS_DATA_H
#define GPS_DATA_H

#include <vector>
#include <string>

namespace gps{

struct Waypoint{

    double lat;
    double lon;
    //double ele;

    time_t time;
};

using Points = std::vector<Waypoint>;
// Part of the track (first, second]
using Segment = std::pair<Points::const_iterator, Points::const_iterator>;

struct Track{
    std::string sourceFile; // The file from which it's loaded
    std::string name; // One file could have several track. That's the name of particular one
    Points points;
};

using Tracks = std::vector<gps::Track>;

bool loadGPX(Tracks& dest, const char* filename);


}

#endif // GPS_DATA_H
