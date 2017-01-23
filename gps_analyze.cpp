#include "gps_utils.h"

#include "gps_data.h"
#include "gps_track_aggregator.h"

#include <iomanip>

namespace gps {

void dump_distances(const Tracks& data){
    for(Track track: data){
        if(track.points.size() <= 1 )
            continue;

        double track_len = 0;
        double track_len_simple = 0;
        double track_len_boost = 0;
        auto it = begin(track.points);
        gps::Waypoint const* prev = &*it;
        for(auto endit = end(track.points); it != endit; ++it){
            gps::Waypoint const* cur = &*it;
            track_len += gps::distance_haversine(prev->lat, prev->lon, cur->lat, cur->lon);
            track_len_simple += gps::distance_flat(prev->lat, prev->lon, cur->lat, cur->lon);
            track_len_boost += gps::distance(*prev, *cur);
            prev = cur;
        }
        std::cout << "Dummy distance for track '" << track.name << "' is "
                  << track_len << " km. 'Flat': " << track_len_simple << " km. "
                  << "Boost: " << track_len_boost << " km." << std::endl;
    }
}

double unique_dist(const Tracks& data){
    double result = 0;
    double overall_total = 0;
    TrackAggregator base("");
    for(auto const& track: data){
        if(track.points.size() <= 1 )
            continue;

        std::cout << "Track: '" << track.name << "' "
                  << std::put_time(std::localtime(&track.points[0].time), "%c %Z") << "..." << std::flush;

        auto whatsnew = base.add(track);

        double result_track = 0;
        for(const auto& i: whatsnew){
            result_track += segmentDistance(i);
        }

        double track_total = segmentDistance({track.points.cbegin(), track.points.cend()});
        std::cout << "length: "
                  << track_total << " km. 'New distance': " << result_track << " km. " << std::endl;
        result += result_track;
        overall_total += track_total;
    }
    std::cout << "Total overall dist: " << overall_total << " km." << std::endl;
    return result;
}

}
