#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>


typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> point;
typedef std::pair<point, unsigned> value;


class Spatial{
public:
    Spatial(){
        setRule();
    }
    void insert(point& p){

    }

    void setRule(){

    }


};

//利用boost 从点集中选择邻近点
int main(int argc, char *argv[]) {
    boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16> > rtree;

    //点集
    rtree.insert(std::make_pair(point(1, 2), 1));
    rtree.insert(std::make_pair(point(2, 2), 2));
    rtree.insert(std::make_pair(point(34, 3), 3));
    rtree.insert(std::make_pair(point(5, 2), 4));
    rtree.insert(std::make_pair(point(6, 2), 5));
    rtree.insert(std::make_pair(point(1, 6), 6));
    rtree.insert(std::make_pair(point(1, 6), 7));

    // search for nearest neighbours
    std::vector<value> res;
    //目标点 (0,0)
    point sought = point(0, 0);
    //目标函数 即是求所有点中到（0，0）点距离小于5的点
    auto func = [&](value const &v) {
        return boost::geometry::distance(v.first, sought) < 5;
    };
    rtree.query(boost::geometry::index::satisfies(func), std::back_inserter(res));

    for (size_t i = 0; i < res.size(); i++) {
        auto singleRes = res[i];
        float x = singleRes.first.get<0>();
        float y = singleRes.first.get<1>();
        std::cout << "Select point: " << singleRes.second << std::endl;
        std::cout << "x: " << x << ",y: " << y << std::endl;
    }

    return 0;
}