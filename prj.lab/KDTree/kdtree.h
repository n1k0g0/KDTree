#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>



class Point {
public:
    Point() : coordinates() {}
    Point(std::initializer_list<float> list) {
        std::copy_n(list.begin(), 3, coordinates.begin());
    }
    float get(size_t index) const {
        return coordinates[index];
    }
    double distance(const Point& pt) const;
private:
    std::array<float, 3> coordinates;
};

std::vector<Point> inputVector = {};



class KDTree {
private:
    struct Node {
        Node() : leftChild(0), rightChild(0), pointNumbersAroundChosenPoint(0){
            chosenPoint = Point({0,0,0});
        }
        Node(Point inputPoint) : leftChild(0), rightChild(0), pointNumbersAroundChosenPoint(0){
            chosenPoint = inputPoint;
        }
        Node(Point inputPoint, std::vector<int> inputNumbersOfPoints, Node inputLeftChild, Node inputRightChild) {
            pointNumbersAroundChosenPoint.clear();
            for (int i = 1; i <= inputNumbersOfPoints.size(); ++i){
                pointNumbersAroundChosenPoint.push_back(inputNumbersOfPoints[i]);
            }
            leftChild = &inputLeftChild;
            rightChild = &inputRightChild;
            chosenPoint = inputPoint;
        }



        Point chosenPoint;
        std::vector<int> pointNumbersAroundChosenPoint; //bounding box
        Node* leftChild;
        Node* rightChild;
    };










    Node buildTree(std::vector<int> numbersOfInputNodes, int depth){
        int axis = depth % 3;
        switch (axis){
            case 0: std::sort(numbersOfInputNodes.begin(), numbersOfInputNodes.end(), comparePointsByX);
            case 1: std::sort(numbersOfInputNodes.begin(), numbersOfInputNodes.end(), comparePointsByY);
            default: std::sort(numbersOfInputNodes.begin(), numbersOfInputNodes.end(), comparePointsByZ);
        }


        if (numbersOfInputNodes.size() == 1){
            std::cout << 'Here';
            return Node(inputVector[numbersOfInputNodes[0]]);
        }

        std::vector<int> smallerPointsNumbers;
        int i = 0;
        for (i = 0; i < numbersOfInputNodes.size() / 2; ++i){
            smallerPointsNumbers.push_back(numbersOfInputNodes[i]);
        }

        std::vector<int> biggerPointsNumbers;
        for (; i < numbersOfInputNodes.size(); ++i){
            biggerPointsNumbers.push_back(numbersOfInputNodes[i]);
        }

        int medianPointNumber = biggerPointsNumbers[0];


        return Node(inputVector[medianPointNumber],
                numbersOfInputNodes,
                buildTree(smallerPointsNumbers, depth + 1),
                buildTree(biggerPointsNumbers, depth + 1));

    }
    void nearest(Node* root, const Point& point, int index);






    Node* root = nullptr;
    Node* best = nullptr;
    double bestDist = 0;
    size_t visited = 0;

public:
    static bool comparePointsByX (int i, int j) {
        return inputVector[i].get(0) < inputVector[j].get(0);
    }
    static bool comparePointsByY (int i, int j) {
        return inputVector[i].get(1) < inputVector[j].get(1);
    }
    static bool comparePointsByZ (int i, int j) {
        return inputVector[i].get(2) < inputVector[j].get(2);
    }


    KDTree(std::vector<Point> inputPoints){


        std::vector<int> numbersOfPointsInBoundingBox = {};
        for (int i = 0; i < inputPoints.size(); ++i){
            inputVector.push_back(inputPoints[i]);
            numbersOfPointsInBoundingBox.push_back(i);
        }

        *root = buildTree(numbersOfPointsInBoundingBox, 0);
    }



    KDTree(std::vector<Point> inputPoints, int depth){

        std::vector<int> numbersOfPointsInBoundingBox;
        numbersOfPointsInBoundingBox.clear();
        for (int i = 0; i < inputPoints.size(); ++i){
            inputVector.push_back(inputPoints[i]);
            numbersOfPointsInBoundingBox.push_back(i);
        }
        int axis = depth % 3;
        *root = buildTree(numbersOfPointsInBoundingBox, depth);



    }

    const Point& nearest(const Point& pt);


};
