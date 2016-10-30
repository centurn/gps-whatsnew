#include "boost/filesystem.hpp"
#include <boost/algorithm/string/predicate.hpp>

#include "gps_data.h"
#include "gps_utils.h"
#include <iostream>


using namespace gps;
using namespace std;
using namespace boost::filesystem;

typedef std::vector<gps::Track> Tracks;
Tracks read_all_tracks(const std::string& dir){
    Tracks result;

    path current_dir(dir);
    if(!is_directory(current_dir)){
        cout << dir << " is not a directory" << endl;
        return result;
    }
    cout << "Reading tracks from '" << dir << "'\n\n";
    for (recursive_directory_iterator iter(current_dir), end;
         iter != end;
         ++iter)
    {
        if (boost::ends_with(iter->path().string(), ".gpx")){
            std::cout << iter->path() << "\n";
            bool loaded = gps::load_gpx(result, iter->path().c_str());
            if(!loaded)
                cout << "GPX Load failed" << endl;
        }
    }

    cout << result.size() << " tracks read" << endl << endl;
    return result;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cout << "Path name expected" << std::endl;
        return -1;
    }
    const char* source = argv[1];

    std::vector<gps::Track> tracks = read_all_tracks(source);

    for(Track track: tracks){
        if(track.points.size() <= 1 )
            continue;

        double track_len = 0;
        double track_len_simple = 0;
        auto it = begin(track.points);
        gps::Waypoint const* prev = &*it;
        for(auto endit = end(track.points); it != endit; ++it){
            gps::Waypoint const* cur = &*it;
            track_len += gps::distance_haversine(prev->lat, prev->lon, cur->lat, cur->lon);
            track_len_simple += gps::distance_flat(prev->lat, prev->lon, cur->lat, cur->lon);
            prev = cur;
        }
        std::cout << "Dummy distance for track '" << track.name << "' is "
                  << track_len << " km. 'Flat': " << track_len_simple << " km." << std::endl;
    }

    return 0;
}
