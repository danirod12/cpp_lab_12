#include <iostream>
#include <vector>
#include "FilterIterator.h"

template <typename Iterator>
void print(Iterator begin, Iterator end) {
    while (begin != end) {
        std::cout << *begin++ << " ";
    }
}

int main() {
    std::vector<int> vector;
    vector.reserve(150);
    for (int i = 1; i < 150; ++i) {
        vector.push_back(i);
    }

    print(vector.begin(), vector.end());

    std::function<bool(int)> func = [](int i) { return i % 10 == 0; };

    FilterIterator filterIterator(vector.begin(), vector.end(), func);

    std::cout << std::endl << std::endl;
    print(filterIterator, filterIterator.getEnd());
    return 0;
}
