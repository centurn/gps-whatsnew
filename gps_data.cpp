#include "pugixml/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <locale>

#include "gps_data.h"

namespace gps{

bool load_gpx(std::vector<Track>& dest, const char* filename){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);

    if(!result){
        std::cout << "XML [" << filename << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << result.description() << "\n";
        std::cout << "Error offset: " << result.offset << " (error at [..." << (filename + result.offset) << "]\n\n";
        return false;
    }

    for(auto trk: doc.child("gpx").children("trk")){
        std::string name = trk.child_value("name");
        std::cout << name << std::endl;
        dest.push_back(Track{std::move(name), {}});
        Track& track = dest.back();

        std::istringstream istr;
        istr.imbue(std::locale("C"));

        for(auto trkseg: trk.children("trkseg")){
            for(auto trkpt: trkseg.children("trkpt")){
                track.points.push_back(Waypoint{});
                Waypoint& point = track.points.back();

                istr.str(trkpt.attribute("lat").value());
                istr.seekg(0);
                istr >> point.lat;

                istr.str(trkpt.attribute("lon").value());
                istr.seekg(0);
                istr >> point.lon;
            }
        }
    }
    return true;
}

} // namespace gps
