#include <iostream>
#include <random>
#include "../../prj.lab/Homography/homography.h"
#include "eigen/3.3.8_1/include/eigen3/Eigen/Core"
#include "eigen/3.3.8_1/include/eigen3/Eigen/Dense"

int main() {
    Homograph g;
    auto dist = g.transformProjectively();
    auto figure = g.makeTriangle(25);

    std::cout << "triangle points: " << std::endl;

    for (int i = 0; i < figure.size(); ++i) {
        std::cout << figure[i][0] << " " << figure[i][1] << std::endl;
    }

    std::cout << "distortion:" << std::endl;
    std::cout << dist.matrix() << std::endl << std::endl;
    std::cout << "distorted triangle" << std::endl;

    std::vector<Eigen::Vector2d> projectivelyTransformedFigure;
    for (int i = 0; i < figure.size(); ++i) {
        auto temp = dist * figure[i].homogeneous();
        projectivelyTransformedFigure.emplace_back(temp[0], temp[1]);
        std::cout << temp[0] << " " << temp[1] << std::endl;
    }

    Homograph a;
    auto approximation = a.approximate(figure, projectivelyTransformedFigure);

    std::cout << "affine-approximated transformation:" << std::endl;
    std::cout << approximation.matrix() << std::endl;
    std::cout << "applying affine transformation" << std::endl;

    std::vector<Eigen::Vector2d> affineTransformedFigure;
    for (int i = 0; i < figure.size(); ++i) {
        auto temp = approximation * figure[i].homogeneous();
        affineTransformedFigure.emplace_back(temp[0], temp[1]);
        std::cout << temp[0] << " " << temp[1] << std::endl;
    }

    return 0;
}
