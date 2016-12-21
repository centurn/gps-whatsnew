#ifndef GPS_DATA_H
#define GPS_DATA_H

#include <vector>
#include <string>

namespace gps{

struct Waypoint{

    double lat;
    double lon;
    //double ele;

    //time_t time;
};

typedef std::vector<Waypoint> Points;

struct Track{
    std::string name;
    Points points;
};

typedef std::vector<gps::Track> Tracks;

bool load_gpx(Tracks& dest, const char* filename);


}

#endif // GPS_DATA_H
