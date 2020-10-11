#include "../prj.lab/kdtree.h"
#include <iostream>
#include <chrono>

std::ostream& operator<<(std::ostream& out, const Point& pt) {
    out << '(';
    for (size_t i = 0; i < 3; ++i) {
        if (i > 0)
            out << ", ";
        out << pt.getCoordByIdx(i);
    }
    out << ')';
    return out;
}


const Point& bruteClosestPoint(const Point& pt, const std::vector<Point>& inputVector) {
    double bestDist = 0;
    Point best = Point();
    double d;
    int i;
    for (i = 0; i < inputVector.size(); ++i){

        d = pt.getDistance(inputVector[i]);
        if ((d < bestDist) || (bestDist == 0)) {
            bestDist = d;
            best = inputVector[i];
        }

    }
    //std::cout << best << " " << std::sqrt(bestDist) << std::endl;
    return best;

}




void test1() {

    std::cout << "test1" << std::endl;
  //  Point points[] = { { 2, 3 , 5}, { 5, 4, 1}, { 9, 6, 8 }, { 4, 7, 1 }, { 8, 1, 2 }, { 7, 2, 4 } };
    auto createVectorStart = std::chrono::high_resolution_clock::now();
    std::vector<Point> points;
    points.resize(1000000);

    for (int i = 0; i < points.size(); ++i){
        points[i] = Point{1.0f * i, 2.0f * i,3.0f * i };
    }
    auto createVectorFinish = std::chrono::high_resolution_clock::now();
    auto createVectorDuration = std::chrono::duration_cast<std::chrono::microseconds>(createVectorFinish - createVectorStart);

    auto buildTreeStart = std::chrono::high_resolution_clock::now();
    KDTree tree(std::begin(points), std::end(points));
    auto buildTreeFinish = std::chrono::high_resolution_clock::now();
    auto buildTreeDuration = std::chrono::duration_cast<std::chrono::microseconds>(buildTreeFinish - buildTreeStart);

    auto searchTreeStart = std::chrono::high_resolution_clock::now();
    Point n = tree.closestPoint({9, 2, 1});
    auto searchTreeFinish = std::chrono::high_resolution_clock::now();
    auto searchTreeDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(searchTreeFinish - searchTreeStart);

    auto bruteForceStart = std::chrono::high_resolution_clock::now();
    Point m = bruteClosestPoint({9, 2, 1}, points);
    auto bruteForceFinish = std::chrono::high_resolution_clock::now();
    auto bruteForceDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(bruteForceFinish - bruteForceStart);

    auto buildTree8Start = std::chrono::high_resolution_clock::now();
    KDTree tree8(std::begin(points), std::end(points), 8);
    auto buildTree8Finish = std::chrono::high_resolution_clock::now();
    auto buildTree8Duration = std::chrono::duration_cast<std::chrono::microseconds>(buildTree8Finish - buildTree8Start);

    auto searchTree8Start = std::chrono::high_resolution_clock::now();
    Point kN = tree8.kClosestPoints({9, 2, 1}, 8);
    auto searchTree8Finish = std::chrono::high_resolution_clock::now();
    auto searchTree8Duration = std::chrono::duration_cast<std::chrono::nanoseconds>(searchTree8Finish - searchTree8Start);

    std::cout << "closestPointFromIdxStep point: " << n << '\n';
    std::cout << "brute force closet Point found: " << m << '\n';
    std::cout << "KDTree & bruteForce resulting point  " << kN << '\n';
    std::cout << "getDistance: " << tree.distance() << '\n';
    std::cout << "nodes visited: " << tree.totalVisited() << '\n';
    std::cout << "get Distance by bruteForce: " << std::sqrt(m.getDistance({9, 2, 1})) << '\n' << '\n' << '\n';

    std::cout << "process Vector duration: " << createVectorDuration.count() << " microseconds" <<'\n';
    std::cout << "brute force duration: " << bruteForceDuration.count() << " nanoseconds" <<'\n';
    std::cout << "total bruteForce duration: " << createVectorDuration.count() + bruteForceDuration.count()/1000 << " microseconds" <<'\n' << '\n';

    std::cout << "build tree duration: " << buildTreeDuration.count() << " microseconds" <<'\n';
    std::cout << "search tree duration: " << searchTreeDuration.count() << " nanoseconds" <<'\n';
    std::cout << "total tree duration: " << buildTreeDuration.count() + searchTreeDuration.count()/1000 << " microseconds" <<'\n' << '\n';

    std::cout << "build tree for tree&bruteForce duration: " << buildTree8Duration.count() << " microseconds" <<'\n';
    std::cout << "kdTree & bruteForce search tree duration: " << searchTree8Duration.count() << " nanoseconds" <<'\n';
    std::cout << "total tree&bf duration: " << buildTree8Duration.count() + searchTree8Duration.count()/1000 << " microseconds" <<'\n' << '\n' << '\n';

}


void test2() {

    //  Point points[] = { { 2, 3 , 5}, { 5, 4, 1}, { 9, 6, 8 }, { 4, 7, 1 }, { 8, 1, 2 }, { 7, 2, 4 } };

    std::cout << "test2" << std::endl;

    auto createVectorStart = std::chrono::high_resolution_clock::now();
    std::vector<Point> points;
    points.resize(100000);

    for (int i = 0; i < points.size(); ++i){
        points[i] = Point{1.0f * i, 0, 0 };
    }
    auto createVectorFinish = std::chrono::high_resolution_clock::now();
    auto createVectorDuration = std::chrono::duration_cast<std::chrono::microseconds>(createVectorFinish - createVectorStart);

    auto buildTreeStart = std::chrono::high_resolution_clock::now();
    KDTree tree(std::begin(points), std::end(points));
    auto buildTreeFinish = std::chrono::high_resolution_clock::now();
    auto buildTreeDuration = std::chrono::duration_cast<std::chrono::microseconds>(buildTreeFinish - buildTreeStart);

    auto searchTreeStart = std::chrono::high_resolution_clock::now();
    Point n = tree.closestPoint({9, 2, 1});
    auto searchTreeFinish = std::chrono::high_resolution_clock::now();
    auto searchTreeDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(searchTreeFinish - searchTreeStart);

    auto bruteForceStart = std::chrono::high_resolution_clock::now();
    Point m = bruteClosestPoint({9, 2, 1}, points);
    auto bruteForceFinish = std::chrono::high_resolution_clock::now();
    auto bruteForceDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(bruteForceFinish - bruteForceStart);

    auto buildTree8Start = std::chrono::high_resolution_clock::now();
    KDTree tree8(std::begin(points), std::end(points), 8);
    auto buildTree8Finish = std::chrono::high_resolution_clock::now();
    auto buildTree8Duration = std::chrono::duration_cast<std::chrono::microseconds>(buildTree8Finish - buildTree8Start);

    auto searchTree8Start = std::chrono::high_resolution_clock::now();
    Point kN = tree8.kClosestPoints({9, 2, 1}, 8);
    auto searchTree8Finish = std::chrono::high_resolution_clock::now();
    auto searchTree8Duration = std::chrono::duration_cast<std::chrono::nanoseconds>(searchTree8Finish - searchTree8Start);

    std::cout << "closestPointFromIdxStep point: " << n << '\n';
    std::cout << "brute force closet Point found: " << m << '\n';
    std::cout << "KDTree & bruteForce resulting point  " << kN << '\n';
    std::cout << "getDistance: " << tree.distance() << '\n';
    std::cout << "nodes visited: " << tree.totalVisited() << '\n';
    std::cout << "get Distance by bruteForce: " << std::sqrt(m.getDistance({9, 2, 1})) << '\n' << '\n' << '\n';

    std::cout << "process Vector duration: " << createVectorDuration.count() << " microseconds" <<'\n';
    std::cout << "brute force duration: " << bruteForceDuration.count() << " nanoseconds" <<'\n';
    std::cout << "total bruteForce duration: " << createVectorDuration.count() + bruteForceDuration.count()/1000 << " microseconds" <<'\n' << '\n';

    std::cout << "build tree duration: " << buildTreeDuration.count() << " microseconds" <<'\n';
    std::cout << "search tree duration: " << searchTreeDuration.count() << " nanoseconds" <<'\n';
    std::cout << "total tree duration: " << buildTreeDuration.count() + searchTreeDuration.count()/1000 << " microseconds" <<'\n' << '\n';

    std::cout << "build tree for tree&bruteForce duration: " << buildTree8Duration.count() << " microseconds" <<'\n';
    std::cout << "kdTree & bruteForce search tree duration: " << searchTree8Duration.count() << " nanoseconds" <<'\n';
    std::cout << "total tree&bf duration: " << buildTree8Duration.count() + searchTree8Duration.count()/1000 << " microseconds" <<'\n' << '\n' << '\n';

}




int main() {
    try {
        test1();
        std::cout << '\n';
        test2();
        std::cout << '\n';
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
