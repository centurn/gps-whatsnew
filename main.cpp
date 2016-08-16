#include "gps_data.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 1){
        std::cout << "Filename expected" << std::endl;
        return -1;
    }
    const char* source = argv[1];

    std::vector<gps::Track> tracks;
    bool result = gps::load_gpx(tracks, source);

    if(!result)
        return -1;

    return 0;
}
