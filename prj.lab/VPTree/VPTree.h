#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <tgmath.h>
#include <queue>

#ifndef VPTREE1_VPTREE_H
#define VPTREE1_VPTREE_H


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
private:
    std::array<float, 3> coordinates;
};





class VPTree {

private:


    struct Node {


        Node(const Point& inputPoint) : pivotPoint(inputPoint),
                                        leftChildNode(nullptr), rightChildNode(nullptr){}

        float getPointCoordByIdx(size_t index) const {
            return pivotPoint.getCoordByIdx(index);
        }

        double distance(const Point& pt) const {
            return pivotPoint.getDistance(pt);
        }

        Point pivotPoint;
        double threshold;
        Node* leftChildNode;
        Node* rightChildNode;

    };



    struct DistanceComparator
    {
        const Point item;
        DistanceComparator(const Point item ) : item(item) {}
        bool operator()(const Point a, const Point b) {
            return item.getDistance(a) < item.getDistance(b);
        }
    };











    Node* buildTree(size_t begin, size_t end, size_t index){
        if (end <= begin)
            return nullptr;
        size_t middle = begin + (end - begin) / 2;
        std::nth_element(&nodeVector[begin], &nodeVector[middle], &nodeVector[0] + end, DistanceComparator(nodeVector[index].pivotPoint));
        index = (index + 1) % 3;
        nodeVector[middle].leftChildNode = buildTree(begin, middle, index);
        nodeVector[middle].rightChildNode = buildTree(middle + 1, end, index);
        return &nodeVector[middle];
    }


    Node* buildFromPoints( int lower, int upper )
    {
        if ( upper == lower ) {
            return NULL;
        }

        Node node(nodeVector[lower]);

        if ( upper - lower > 1 ) {

            int i = (int)((double)rand() / RAND_MAX * (upper - lower - 1) ) + lower;
            std::swap( nodeVector[lower], nodeVector[i] );
            int median = ( upper + lower ) / 2;


            std::nth_element(
                    nodeVector.begin() + lower + 1,
                    nodeVector.begin() + median,
                    nodeVector.begin() + upper,
                    DistanceComparator( nodeVector[lower].pivotPoint ));

            node.threshold = nodeVector[lower].pivotPoint.getDistance(nodeVector[median].pivotPoint);
            node.pivotPoint = nodeVector[lower].pivotPoint;
            node.leftChildNode = buildFromPoints( lower + 1, median );
            node.rightChildNode = buildFromPoints( median, upper );
        }

        return &node;
    }




    Node* kNearestBuildTree(size_t begin, size_t end, size_t step, size_t k){
        if (end <= begin)
            return nullptr;
        size_t middle = begin + (end - begin) / 2;
        std::nth_element(&nodeVector[begin], &nodeVector[middle], &nodeVector[0] + end, DistanceComparator(nodeVector[middle].pivotPoint));
        ++step;
        size_t index = step % 3;
        if (step < log2 (nodeVector.size() / k)){
            nodeVector[middle].leftChildNode = kNearestBuildTree(begin, middle, step, k);
            nodeVector[middle].rightChildNode = kNearestBuildTree(middle + 1, end, step, k);

        }
        return &nodeVector[middle];
    }





    void closestPointFromIdxStep(Node* tRoot, const Point& point, int index){
        if (tRoot == nullptr)
            return;
        ++visited;
        double d = tRoot->distance(point);
        if (d < bestDist || best == nullptr) {
            bestDist = d;
            best = tRoot;
        }

        double dx = tRoot->getPointCoordByIdx(index) - point.getCoordByIdx(index);
        index = (index + 1) % 3;
        closestPointFromIdxStep(dx > 0 ? tRoot->leftChildNode : tRoot->rightChildNode, point, index);

        if (bestDist <= dx * dx) // this here added
            return;
        closestPointFromIdxStep(dx > 0 ? tRoot->rightChildNode : tRoot->leftChildNode, point, index);
    };


    void kClosestPointsFromIdxStep(Node* tRoot, const Point& point, int k, int step, size_t begin, size_t end){
        if (tRoot == nullptr)
            return;
        ++visited;

        double d = tRoot->distance(point);
        if (d < bestDist || best == nullptr) {
            bestDist = d;
            best = tRoot;
        }

        double dx = tRoot->getPointCoordByIdx(step % 3) - point.getCoordByIdx(step % 3);


        ++step;
        size_t index = step % 3;


        if (step < log2 (nodeVector.size() / k)){
            kClosestPointsFromIdxStep(dx > 0 ? tRoot->leftChildNode : tRoot->rightChildNode, point, k, step, begin, begin + (end - begin) / 2);
            if (bestDist <= dx * dx) // this here added
                return;
            kClosestPointsFromIdxStep(dx > 0 ? tRoot->rightChildNode : tRoot->leftChildNode, point, k, step, begin + (end - begin) / 2 + 1, end);

        }



        if ((step >= log2(nodeVector.size() / k)) && (step < log2(nodeVector.size()))){

            int i;
            for (i = 0; i < end - begin; ++i){

                d = point.getDistance(nodeVector[i + begin].pivotPoint);
                if ((d < bestDist) || (bestDist == 0)) {
                    bestDist = d;
                    best = &nodeVector[i + begin];
                }

            }
        }


        return;

    };




    std::vector<Node> nodeVector;
    Node* root = nullptr;
    Node* best = nullptr;
    double bestDist = 0;
    size_t visited = 0;
    double _tau;


public:

    VPTree(const VPTree&) = delete;
    VPTree& operator=(const VPTree&) = delete;


    template<typename iterator>
    VPTree(iterator begin, iterator end) : nodeVector(begin, end) {
        root = buildTree(0, nodeVector.size(), 0);
    }

    template<typename iterator>
    VPTree(iterator begin, iterator end, int k) : nodeVector(begin, end) {
        root = kNearestBuildTree(0, nodeVector.size(), 0, k);
    }


    size_t totalVisited() const { return visited; }

    double distance() const { return std::sqrt(bestDist); }


    const Point& closestPoint(const Point& pt) {
        if (root == nullptr)
            throw std::logic_error("tree is empty");
        best = nullptr;
        visited = 0;
        bestDist = 0;
        closestPointFromIdxStep(root, pt, 0);
        return best->pivotPoint;
    }

    const Point& kClosestPoints(const Point& pt, int k) {
        if (root == nullptr)
            throw std::logic_error("tree is empty");
        best = nullptr;
        visited = 0;
        bestDist = 0;
        kClosestPointsFromIdxStep(root, pt, k, 0, 0, nodeVector.size());
        return best->pivotPoint;
    }


};



#endif //VPTREE1_VPTREE_H