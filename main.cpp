#include "boost/filesystem.hpp"
#include <boost/algorithm/string/predicate.hpp>

#include "gps_data.h"
#include "gps_utils.h"
#include "gps_analyze.h"
#include <iostream>


using namespace gps;
using namespace std;
using namespace boost::filesystem;


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

    // Sort chronologically.
    // note: addressing points[0] is safe because we discard empty tracks
    std::sort(begin(result), end(result), [](auto const& l, auto const& r){
        return l.points[0].time < r.points[0].time;
    });
    return result;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cout << "Path name expected" << std::endl;
        return -1;
    }
    const char* source = argv[1];

    auto tracks = read_all_tracks(source);

    auto unique = unique_dist(tracks);
    std::cout << "Total new dist: " << unique << std::endl;

    return 0;
}
