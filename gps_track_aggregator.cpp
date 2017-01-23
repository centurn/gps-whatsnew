#include "gps_track_aggregator.h"

#include "gps_data.h"
#include "gps_utils.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <utility>


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
using std::string;

//BOOST_GEOMETRY_REGISTER_POINT_2D(gps::Waypoint, double, cs::spherical_equatorial<bg::degree>, lon, lat)

namespace gps {

using Rtree = bgi::rtree<Waypoint, bgi::rstar<32>>;
using points_citer = Points::const_iterator;
using SourceKey = std::pair<string, string>; // name, source file
using SourcesSet = std::unordered_set<SourceKey, boost::hash<SourceKey>>;

static constexpr double epsilon = 0.150; // 150 meters

struct TrackAggregator::Impl{
    explicit Impl(const string& dir)
        : cacheDir(dir)
    {}

    string cacheDir;
    SourcesSet sources;
    Rtree rtree;
};

TrackAggregator::TrackAggregator(const std::string& cacheDir)
    : pimpl(new Impl(cacheDir))
{
}

TrackAggregator::~TrackAggregator() = default;

WhatsNew TrackAggregator::add(Track const& track) {
    WhatsNew result;
    pimpl->sources.emplace(track.name, track.sourceFile);

    if(track.points.size() <= 1 )
        return result;

    auto& tree = pimpl->rtree;
    auto left = track.points.cbegin();
    auto right = left;
    bool is_new_seg = true;
    for(auto endit = track.points.cend(); right != endit; ++right){
        gps::Waypoint const* cur = &*right;
        auto neighbor = tree.qbegin(bgi::nearest(*cur, 1));
        if(neighbor == tree.qend())
            continue;
        gps::Waypoint const* nearest = &*neighbor;
        bool is_new = distance(*cur, *nearest) > epsilon;
        if(is_new != is_new_seg){
            if(is_new_seg)
                result.emplace_back(std::make_pair(left, right));
            is_new_seg = is_new;
            left = right;
        }
    }
    if(is_new_seg)
        result.emplace_back(std::make_pair(left, right));
    tree.insert(track.points.cbegin(), track.points.cend());

    return result;
}

bool TrackAggregator::contains(const Track& track) const{
    return pimpl->sources.find(std::make_pair(track.name, track.sourceFile))
            != pimpl->sources.end();
}

}
