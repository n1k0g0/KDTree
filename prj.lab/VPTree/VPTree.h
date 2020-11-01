#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <queue>
#include <limits>
#include <array>


class Point {
public:

    Point() = default;
    Point(std::array<float, 3>) {}
    Point(std::initializer_list<float> list) {
        std::copy_n(list.begin(), coordinates.size(), coordinates.begin());
    }
    ~Point() = default;
    float getCoordByIdx(size_t index) const {
        return coordinates[index];
    }

    double getDistance(const Point& pt) const {
        double dist = 0;
        for (size_t i = 0; i < coordinates.size(); ++i) {
            double d = getCoordByIdx(i) - pt.getCoordByIdx(i);
            dist += d * d;
        }
        return dist;
    }
    bool operator==(const Point& pt) const{
        return (getCoordByIdx(0) == pt.getCoordByIdx(0)) && (getCoordByIdx(1) == pt.getCoordByIdx(1)) && (getCoordByIdx(2) == pt.getCoordByIdx(2));
    }
    bool inRange(const Point& p, double radius) const{
        return getDistance(p) < radius;
    }
    void printPoint() const{
        for (size_t i = 0; i < 3; ++i) {
            if (i > 0)
                std::cout << ", ";
            std::cout << getCoordByIdx(i);
        }
    }
private:
    std::array<float, 3> coordinates;
};


class VPTree{
public:
    VPTree(std::vector<Point>& points)
            : pointsNumber(points.size()) {
        if (pointsNumber == 0) {
            // empty point set, return empty tree
            root      = nullptr;
            leftChildNode   = nullptr;
            rightChildNode  = nullptr;
            leaf = true;
            borderDistance = std::numeric_limits<double>::infinity();
        }
        else if (pointsNumber == 1) {
            root = new Point(points[0]);
            leftChildNode = nullptr;
            rightChildNode = nullptr;
            leaf = true;
            borderDistance = std::numeric_limits<double>::infinity();
        }
        else {
            root = new Point(points[0]);
            leaf = false;
            decltype(points.size()) firstIdx = 1;
            decltype(points.size()) lastIdx  = points.size() - 1;
            decltype(points.size()) middIdx  = (firstIdx + lastIdx) / 2;
            auto firstIter = points.begin() + firstIdx;
            auto middIter  = points.begin() + middIdx;
            std::nth_element( firstIter, middIter, points.end(), [&](const Point & p1, const Point & p2)->bool {
                return root->getDistance(p1) < root->getDistance(p2);
            });
            borderDistance = root->getDistance(points[middIdx]);
            decltype(middIdx) leftLen   = middIdx - firstIdx + 1;
            decltype(middIdx) rightLen  = lastIdx - middIdx;
            std::vector<Point> left, right;
            std::copy_n(firstIter, leftLen,  std::back_inserter(left));
            std::copy_n(middIter + 1, rightLen, std::back_inserter(right));
            leftChildNode  = left.empty() ? nullptr : new VPTree(left);
            rightChildNode = right.empty() ? nullptr : new VPTree(right);
        }
    }
    ~VPTree() {
        delete root;
        delete leftChildNode;
        delete rightChildNode;
    }
    bool isLeaf() const {
        return leaf;
    }
    void runThroughTree() const {
        if (root) {
            root->printPoint();
            if (leftChildNode)
                leftChildNode->runThroughTree();
            if (rightChildNode)
                rightChildNode->runThroughTree();
        }
        else
            std::cout << "empty tree" << std::endl;
    }
    const VPTree* closestNode(Point& pt) const {
        VPTree* node = nullptr;
        if (pt == (*root))
            return this;
        else if (leaf)
            node = nullptr;
        else {
            double distance = root->getDistance(pt);
            if (distance > borderDistance)
                return rightChildNode->closestNode(pt);
            else
                return leftChildNode->closestNode(pt);
        }
        return node;
    }
    void search(const Point& pt, double radius, std::vector<Point>& foundPoints) const {
        double distance = root->getDistance(pt);
        if (0 < distance && distance <= radius)
            foundPoints.push_back(*root);
        if (leaf) {
            return;
        }
        else if (distance > borderDistance) {
            if (rightChildNode)
                rightChildNode->search(pt, radius, foundPoints);
            if (distance - borderDistance < radius && leftChildNode) {
                leftChildNode->search(pt, radius, foundPoints);
            }
            else {
            }
        }
        else
        {
            if (leftChildNode)
                leftChildNode->search(pt, radius, foundPoints);
            if (distance + radius > borderDistance && rightChildNode) {
                rightChildNode->search(pt, radius, foundPoints);
            }
            else {
            }
        }
    }
private:
    Point* root;
    double borderDistance;
    bool leaf;
    size_t pointsNumber;
    VPTree* leftChildNode;
    VPTree* rightChildNode;
};

