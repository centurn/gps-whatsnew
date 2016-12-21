#include <iostream>
#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgm = boost::geometry::model;
namespace bgi = boost::geometry::index;

typedef bgm::point<double, 2, bg::cs::spherical_equatorial<bg::degree> > point;
typedef bgi::rtree<point, bgi::rstar<32, 9, 9, 32> > rtree;

double distance(point const& p1, point const& p2)
{
    return bg::distance(p1, p2) * 6371000.0;
}

struct less
{
    less(point const& qq) : q(qq) {}
    bool operator()(point const& l, point const& r)
    {
        return bg::comparable_distance(l, q) < bg::comparable_distance(r, q);
    }
    point q;
};

template <typename It>
void sort(It first, It last, point const& q)
{
    std::sort(first, last, less(q));
}

int main()
{
    std::vector<point> pts;
    pts.push_back(point(10.9325, 59.9226));
    pts.push_back(point(10.9352, 59.9285));
    pts.push_back(point(11.0821, 59.9182));
    pts.push_back(point(11.0890, 59.9097));
    pts.push_back(point(11.0971, 59.8969));
    pts.push_back(point(11.0999, 59.8924));
    pts.push_back(point(11.0995, 59.8895));
    pts.push_back(point(11.1550, 59.9251));
    pts.push_back(point(11.1571, 59.9247));

    pts.push_back(point(10.9402, 59.9343));
    pts.push_back(point(10.9351, 59.9364));
    pts.push_back(point(10.9466, 59.9316));
    pts.push_back(point(10.9204, 59.9402));
    pts.push_back(point(10.9157, 59.9400));
    pts.push_back(point(10.9428, 59.9455));
    pts.push_back(point(10.9479, 59.9477));
    pts.push_back(point(10.9469, 59.9492));

    // Randomize points
    for ( int i = 0 ; i < 30000 ; ++i )
    {
        double x = rand() / double(RAND_MAX);
        double y = rand() / double(RAND_MAX);
        point p(4 + x * 27, 50 + y * 21);
        pts.push_back(p);
    }

    // Create using pack create
    //rtree rt (pts);

    // Create using insert
    rtree rt;
    BOOST_FOREACH(point const& p, pts)
    {
        rt.insert(p);
    }

    point q(10.9325, 59.9226);

    sort(pts.begin(), pts.end(), q);

    std::cout << "Points:\n";
    int i = 0;
    BOOST_FOREACH(point const& p, pts)
    {
        std::cout << bg::wkt(p) << " - " << distance(p, q) << std::endl;
        if ( i++ > 20 )
            break;
    }

    std::cout << "Bounds:\n";
    std::cout << bg::wkt(rt.bounds()) << std::endl;
    std::cout << "Size:\n";
    std::cout << rt.size() << std::endl;

    std::vector<point> res;
    rt.query(bgi::nearest(q, 8), std::back_inserter(res));

    sort(res.begin(), res.end(), q);

    std::cout << "Nearest:\n";
    BOOST_FOREACH(point const& p, res)
    {
        std::cout << bg::wkt(p) << " - " << distance(p, q) << std::endl;
    }
}
