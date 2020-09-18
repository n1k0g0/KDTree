#include "../prj.lab/kdtree.h"



double Point::distance(const Point& pt) const {
    double dist = 0;
    for (size_t i = 0; i < 3; ++i) {
        double d = get(i) - pt.get(i);
        dist += d * d;
    }
    return dist;
}







void KDTree::nearest(KDTree::Node *root, const Point &point, int index) {
    if (root == nullptr)
        return;
    ++visited;
    double d = root->chosenPoint.distance(point);
    if (best == nullptr || d < bestDist) {
        bestDist = d;
        best = root;
    }
    if (bestDist == 0)
        return;
    double dx = root->chosenPoint.get(index) - point.get(index);
    index = (index + 1) % 3;
    nearest(dx > 0 ? root->leftChild : root->rightChild, point, index);
    if (dx * dx >= bestDist)
        return;
    nearest(dx > 0 ? root->rightChild : root->leftChild, point, index);

}

const Point& KDTree::nearest(const Point& pt) {
    if (root == nullptr)
        throw std::logic_error("tree is empty");
    best = nullptr;
    visited = 0;
    bestDist = 0;
    nearest(root, pt, 0);
    return best->chosenPoint;
}



std::ostream& operator<<(std::ostream& out, const Point& pt) {
    out << '(';
    for (size_t i = 0; i < 3; ++i) {
        if (i > 0)
            out << ", ";
        out << pt.get(i);
    }
    out << ')';
    return out;
}


void test1() {

    std::vector<Point> inVector = {{1, 3, 4},
                      {2, 2, 5},
                      {3, 2, 4},
                      {4, 1, 5},
                      {5, 1, 4},
                      };



    KDTree tree(inVector);


    Point n = tree.nearest({10, 2, 5});

    std::cout << "Input data:\n";
    std::cout << "closest Point: " << n << '\n';
    std::cout << "distance: " << Point({10,2,5}).distance(n) << '\n';
}

int main() {
    std::cout << "here";
    try {
        test1();
        std::cout << '\n';

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}