#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#endif

#include <iostream>
#include <vector>
#include <random>
#include "/usr/local/Cellar/eigen/3.3.8_1/include/eigen3/Eigen/Dense"

class Homograph {
public:
    Homograph() = default;
    std::vector<Eigen::Vector2d> makeTriangle(int numberOfPoints){

        enum sides {
            LEFT,
            RIGHT,
            BOTTOM
        };

        double sideLength = randomDouble(lowerRandomBound, upperRandomBound);
        if (numberOfPoints < 3) {
            throw std::invalid_argument("Not enough points provided (less than 4)");
        }

        Eigen::Vector2d top = Eigen::Vector2d(0, sideLength * sqrt(3) / 2);
        Eigen::Vector2d left = Eigen::Vector2d(-sideLength / 2, 0);
        Eigen::Vector2d right = Eigen::Vector2d(sideLength / 2, 0);

        std::vector<Eigen::Vector2d> trianglePoints(numberOfPoints);

        trianglePoints[0] = top;
        trianglePoints[1] = left;
        trianglePoints[2] = right;

        for (int i = 3; i < numberOfPoints; ++i) {

            int currentSide = randomInt(0, 3);
            Eigen::Vector2d currentPoint;

            double shift = randomDouble(0, sideLength);
            double shift_x = shift / 2;
            double shift_y = shift * sqrt(3) / 2;

            if (currentSide == LEFT){
                currentPoint = top + Eigen::Vector2d(-shift_x, -shift_y);
            } else if (currentSide == RIGHT){
                currentPoint = top + Eigen::Vector2d(shift_x, -shift_y);
            } else if (currentSide == BOTTOM){
                currentPoint = left + Eigen::Vector2d(randomDouble(0, sideLength), 0);
            }
            /*switch (currentSide) {
                case LEFT:
                    currentPoint = top + Eigen::Vector2d(-shift_x, -shift_y);
                    break;

                case RIGHT:
                    currentPoint = top + Eigen::Vector2d(shift_x, -shift_y);
                    break;

                case BOTTOM:
                    currentPoint = left + Eigen::Vector2d(randomDouble(0, sideLength), 0);
                    break;
            }*/


            trianglePoints[i] = currentPoint;
        }
        return trianglePoints;

    }


    Eigen::Transform<double, 2, Eigen::Projective> transformProjectively(){
        Eigen::Transform<double, 2, Eigen::Projective> transformation = Eigen::Transform <double, 2, Eigen::Projective>::Identity();

        transformation.rotate(randomDouble(-M_PI, M_PI));
        transformation.scale(randomDouble(0, 5));

        //correcting the transformation so that it is not affine
        Eigen::Vector3d projectiveDistortion = Eigen::Vector3d(randomDouble(-5e-2, 5e-2), randomDouble(-5e-2, 5e-2), randomDouble(-5e-2, 5e-2));
        transformation.matrix().row(2) += projectiveDistortion;

        return transformation;
    }

private:
    double lowerRandomBound = 0;
    double upperRandomBound = 1;

    double randomDouble(double lowerBound, double upperBound){
        randomEngine.seed(std::random_device{}());
        std::uniform_real_distribution<double> randomDistribution(lowerBound, upperBound);
        return randomDistribution(randomEngine);
    }

    int randomInt(int lowerBound, int upperBound){
        randomEngine.seed(std::random_device{}());
        std::uniform_int_distribution<int> randomDistribution(lowerBound, upperBound);
        return randomDistribution(randomEngine);
    }

    std::mt19937 randomEngine;

public:

    Eigen::Transform<double, 2, Eigen::Affine> approximate(std::vector<Eigen::Vector2d> undistorted,
                                                           std::vector<Eigen::Vector2d> distorted){
        if (undistorted.size() != distorted.size()) {
            throw std::invalid_argument("Two arrays of vectors must be of the same size!");
        }

        int sourceSize = undistorted.size();

        Eigen::VectorXd apprVector = Eigen::VectorXd::Zero(2 * sourceSize);
        Eigen::MatrixXd apprMatrix = Eigen::MatrixXd::Zero(2 * sourceSize, 6);

        for (int i = 0; i < sourceSize; ++i) {
            apprMatrix(i, 0) = undistorted[i](0);
            apprMatrix(i, 1) = undistorted[i](1);
            apprMatrix(i, 2) = 1;
            apprMatrix(i + sourceSize, 3) = undistorted[i](0);
            apprMatrix(i + sourceSize, 4) = undistorted[i](1);
            apprMatrix(i + sourceSize, 5) = 1;
            apprVector(i) = distorted[i](0);
            apprVector(i + sourceSize) = distorted[i](1);
        }

        Eigen::VectorXd solution = apprMatrix.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(apprVector);
        Eigen::Matrix3d affine = Eigen::Matrix3d::Zero();

        affine(0, 0) = solution(0);
        affine(0, 1) = solution(1);
        affine(0, 2) = solution(2);
        affine(1, 0) = solution(3);
        affine(1, 1) = solution(4);
        affine(1, 2) = solution(5);
        affine(2, 2) = 1;

        auto approximation = Eigen::Transform<double, 2, Eigen::Affine>(affine);
        return approximation;
    }
private:
};
