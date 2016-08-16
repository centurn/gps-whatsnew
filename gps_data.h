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

struct Track{
    std::string name;
    std::vector<Waypoint> points;
};

bool load_gpx(std::vector<Track>& dest, const char* filename);

}

#endif // GPS_DATA_H
