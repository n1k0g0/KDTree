#include "kdtree.h"




std::ostream& operator<<(std::ostream& out, const Point& pt) {
    for (size_t i = 0; i < 3; ++i) {
        out << pt.get(i);
    }
    return out;
}

void test1() {

    Point points[] = {{1, 3, 4},
                      {2, 2, 5},
                      {3, 2, 4},
                      {4, 1, 5},
                      {5, 1, 4},
                      };
    
}

int main() {
    try {
        test1();
        std::cout << '\n';

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}