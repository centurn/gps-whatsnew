#include "gps_data.h"
#include "gps_utils.h"

namespace gps {

namespace bgi = boost::geometry::index;
//using points_citer = decltype(Track::points.cbegin());
using points_citer = Points::const_iterator;

static constexpr double epsilon = 0.150; // 150 meters

double segment_distance(points_citer it, points_citer to){
    double result = 0;
    gps::Waypoint const* prev = &*it;
    for(; it != to; ++it){
        gps::Waypoint const* cur = &*it;
        result += gps::distance_boost(*prev, *cur);
        prev = cur;
    }

    return result;
}

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
            track_len_boost += gps::distance_boost(*prev, *cur);
            prev = cur;
        }
        std::cout << "Dummy distance for track '" << track.name << "' is "
                  << track_len << " km. 'Flat': " << track_len_simple << " km. "
                  << "Boost: " << track_len_boost << " km." << std::endl;
    }
}

typedef bgi::rtree<Waypoint, bgi::rstar<32>> rtree;

double unique_dist(const Tracks& data){
    double result = 0;
    double overall_total = 0;
    rtree tree;
    for(auto const& track: data){
        if(track.points.size() <= 1 )
            continue;

        double result_track = 0;
        auto left = track.points.cbegin();
        auto right = left;
        bool is_new_seg = true;
        for(auto endit = track.points.cend(); right != endit; ++right){
            gps::Waypoint const* cur = &*right;
            auto neighbor = tree.qbegin(bgi::nearest(*cur, 1));
            if(neighbor == tree.qend())
                continue;
            gps::Waypoint const* nearest = &*neighbor;
            bool is_new = distance_boost(*cur, *nearest) > epsilon;
            if(is_new != is_new_seg){
                if(is_new)
                    result_track += segment_distance(left, right);
                is_new_seg = is_new;
                left = right;
            }
        }
        if(is_new_seg)
            result_track += segment_distance(left, right);
        tree.insert(track.points.cbegin(), track.points.cend());
        double track_total = segment_distance(track.points.cbegin(), track.points.cend());
        std::cout << "Length of track '" << track.name << "' is "
                  << track_total << " km. 'New distance': " << result_track << " km. " << std::endl;
        result += result_track;
        overall_total += track_total;
    }
    std::cout << "Total overall dist: " << overall_total << " km." << std::endl;
    return result;
}

}
