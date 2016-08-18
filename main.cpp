#include "gps_data.h"
#include "gps_utils.h"
#include <iostream>

using namespace gps;

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cout << "Filename expected" << std::endl;
        return -1;
    }
    const char* source = argv[1];

    std::vector<gps::Track> tracks;
    bool result = gps::load_gpx(tracks, source);

    for(Track track: tracks){
        if(track.points.size() <= 1 )
            continue;

        double track_len = 0;
        auto it = begin(track.points);
        gps::Waypoint const* prev = &*it;
        for(auto endit = end(track.points); it != endit; ++it){
            gps::Waypoint const* cur = &*it;
            track_len += gps::distance_haversine(prev->lat, prev->lon, cur->lat, cur->lon);
            prev = cur;
        }
        std::cout << "Dummy track distance for track '" << track.name << "' is " << track_len << "km." << std::endl;
    }
    if(!result)
        return -1;

    return 0;
}
