#ifndef GPS_TRACK_AGGREGATOR_H
#define GPS_TRACK_AGGREGATOR_H

#include <memory>
#include <string>
#include <vector>

#include <gps_data.h>
namespace gps{

struct Track;

// A list of segments that were never visited before
using WhatsNew = std::vector<Segment>;

class TrackAggregator{
public:
    TrackAggregator(const std::string& cacheDir);
    ~TrackAggregator();

    // Add to collection of tracks. Info on new segments are returned.
    // If the pair (file name, track name) was already added to the cache, then do nothing
    WhatsNew add(const Track& track);

    bool contains(const Track& track) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl; // minimize boost geometry includes
};

}

#endif // GPS_TRACK_AGGREGATOR_H
