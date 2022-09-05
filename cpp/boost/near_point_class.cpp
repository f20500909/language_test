#include <iostream>
#include <functional>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

typedef std::tuple<double, char, std::string>  pointTest;
class A{

};

typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> point;
typedef std::pair<point, A> item;
typedef std::tuple<point, A,A,A,point> item2;

boost::geometry::index::rtree<item2, boost::geometry::index::quadratic<16> > testData;

//尽量用唯一id来遍历
class SpatialSvg {
public:
    SpatialSvg() {
        setDistanceThreshold(5);
    }

    void insert(const item &item) {
        rtree.insert(item);
    }

    //插入点 其标识码为点的序列
    void insert(const point &p) {
        insert(p, count++);
    }

    void insert(const point &p, const int id) {
        item item = std::make_pair(p, id);
        insert(item);
    }

    void setDistanceThreshold(int dis) {
        distanceThreshold = dis;
    }

    std::vector<item> getVaildPoint() {
        std::vector<item> res;
        point sought = point(0, 0);

        auto _rule = [&](item const &v) {
            return boost::geometry::distance(v.first, sought) < distanceThreshold;
        };

        rtree.query(boost::geometry::index::satisfies(_rule), std::back_inserter(res));
        return res;
    }

    //获取邻近点位
    std::vector<item> getNearItem(point& p, int distance) {
        std::vector<item> res;

        auto _rule = [&](item const &v) { return boost::geometry::distance(v.first, p) < distance; };

        rtree.query(boost::geometry::index::satisfies(_rule), std::back_inserter(res));
        return res;
    }

    std::vector<item> getPattern(point& p){

    }


    void test() {
        insert(point(2, 2));
        insert(point(3, 2));
        insert(point(6, 2));
        insert(point(2, 8));
        insert(point(8, 2));
        insert(point(2, 9));

        for (auto it = rtree.begin(); it != rtree.end(); ++it) {
            std::cout << it->second << std::endl;
        }


    }

    inline int getCount(){
        return count;
    }

    int distanceThreshold;
    int count;
    boost::geometry::index::rtree<item, boost::geometry::index::quadratic<16> > rtree;
};

int main(int argc, char *argv[]) {

    SpatialSvg s;
    s.test();

    return 0;
}