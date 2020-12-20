#include <random>
#include <iostream>
#include "../prj.lab/homography.h"
#include "/usr/local/Cellar/eigen/3.3.8_1/include/eigen3/Eigen/Core"
#include "/usr/local/Cellar/eigen/3.3.8_1/include/eigen3/Eigen/Dense"

int main() {
    Homograph g;
    auto dist = g.transformProjectively();
    auto square = g.makeTriangle(20);

    std::cout << "triangle points: " << std::endl;

    for (int i = 0; i < square.size(); ++i) {
        std::cout << square[i][0] << " " << square[i][1] << std::endl;
    }

    std::cout << "distortion:" << std::endl;
    std::cout << dist.matrix() << std::endl << std::endl;
    std::cout << "distorted triangle" << std::endl;

    std::vector<Eigen::Vector2d> transformedSquare;
    for (int i = 0; i < square.size(); ++i) {
        auto temp = dist * square[i].homogeneous();
        transformedSquare.emplace_back(temp[0], temp[1]);
        std::cout << temp[0] << " " << temp[1] << std::endl;
    }

    Homograph a;
    auto approximation = a.approximate(square, transformedSquare);

    std::cout << "affine-approximated transformation:" << std::endl;
    std::cout << approximation.matrix() << std::endl;
    std::cout << "applying affine transformation" << std::endl;

    std::vector<Eigen::Vector2d> affineTransformedSquare;
    for (int i = 0; i < square.size(); ++i) {
        auto temp = approximation * square[i].homogeneous();
        affineTransformedSquare.emplace_back(temp[0], temp[1]);
        std::cout << temp[0] << " " << temp[1] << std::endl;
    }

    return 0;
}
