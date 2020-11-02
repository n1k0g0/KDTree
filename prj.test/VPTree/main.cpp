#include<iostream>
#include<vector>
#include<random>
#include<chrono>
#include<cassert>
#include<algorithm>
#include"../prj.lab/vp3.h"
#define RANGEMAX 10
#ifdef NDEBUG
std::default_random_engine g(std::random_device {}());
#else
std::default_random_engine g(0);
#endif

std::uniform_real_distribution<float> d(0, RANGEMAX);
std::vector<Point> randomPoints(float);

std::vector<Point> randomPoints(float num)
{
    std::vector<Point> points;
    points.reserve(num);
    for (decltype(num) i = 0; i < num; i++)
    {
        points.push_back(Point({d(g), d(g), d(g)}));
    }

    return points;
}

int main(int argNum, char** args) {
    int particleNum = 4;
    double radius = 1;
    if (argNum >= 2)
        particleNum = atoi(args[1]);
    std::vector<Point> points = randomPoints(particleNum);


    auto t1 = std::chrono::high_resolution_clock::now();
    VPTree vTree = VPTree(points);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto buildTreeDuration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();

    auto results = std::vector<std::vector<Point>>(particleNum);
    auto t3 = std::chrono::high_resolution_clock::now();
    for (decltype(points.size()) i = 0; i < points.size(); i++) {
        vTree.search(points[i], radius, results[i]);
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    auto searchTreeDuration = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3).count();

    int pairsum = 0;
    for (auto &r : results)
        pairsum += r.size();

    std::cout << "build tree duration: " << buildTreeDuration << " seconds" << std::endl;
    std::cout << "search tree duration: " << searchTreeDuration << " seconds" << std::endl;
    std::cout << "total tree duration: " << (searchTreeDuration + buildTreeDuration) << " seconds" << std::endl << std::endl;

    int bruteSearchNum = 0;
    auto directResult = std::vector<std::vector<Point>>(particleNum);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (decltype(points.size()) i = 0; i < points.size(); i++) {
        std::copy_if(points.begin(), points.end(), std::back_inserter(directResult[i]), [&](const Point &p1) -> bool {
            return points[i].inRange(p1, radius);
        });
    }
    auto t6 = std::chrono::high_resolution_clock::now();
    auto bruteSearchTime = std::chrono::duration_cast<std::chrono::duration<double>>(t6 - t5).count();

    std::cout << "brute search" << std::endl;
    std::cout << "search time: " << bruteSearchTime << " seconds" << std::endl;

    return 0;

}



    /*std::vector<Point> points;
    points.resize(1000000);
    for (int i = 0; i < points.size(); ++i){
        points[i] = Point{1.0f * i, 2.0f * i,3.0f * i };
    }
    auto vTree = VPTree(points);
    double radius = 0.1 * 10.0f;


    int particleNum = 4;
    auto results = std::vector<std::vector<Point>>(particleNum);
    for (decltype(points.size()) i = 0; i < points.size(); i++)
    {
        vTree.search(points[i], radius, results[i]);
    }
    int pairsum = 0;
    for (auto & r : results)
        pairsum += r.size();

    results[0][0].printPoint();
}*/

