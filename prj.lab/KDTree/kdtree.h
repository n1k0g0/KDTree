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







class KDTree {
public:
private:
    struct Node {
        Node(const Point& pt) : nodePoint(pt), leftChild(nullptr), rightChild(nullptr) {}

        double distance(const Point& pt) const {
            return nodePoint.distance(pt);
        }

        Point nodePoint;
        Node* leftChild;
        Node* rightChild;
    };
    Node* rootNode = nullptr;
    Node* nodeBestDist = nullptr;
    float bestDist = 0;
    size_t visitedTotal = 0;
    std::vector<Node> nodes;




    Node* buildTree(size_t begin, size_t end, size_t index);

    void closest(Node* root, const Point& point, size_t index);

public:

    KDTree (std::vector<Node> inputNodes);

    size_t visited() const { return visitedTotal; }
    double distance() const { return std::sqrt(bestDist); }
    const Point& closest(const Point& pt);
};
