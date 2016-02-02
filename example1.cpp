#include "bst_imp.h"
#include <iostream>

int main() {
#define bst ((((((BST<int>() + 3) + 4) + 1) + 2) + 7) + 3)
    std::cout << bst << std::endl; // prints 1 2 3 3 4 7
    std::cout << bst.size() << std::endl; //prints 6
    std::cout << bst.min() << std::endl; // prints 1
    std::cout << bst.max() << std::endl; // prints 7
    std::cout << bst.height() << std::endl; // prints 4
    std::cout << spine(bst).height() << std::endl; // prints 6
    std::cout << bst.find(4) << std::endl; // prints 4 7
    //std::cout << bst.find(11) << std::endl; //prints nothing (possibly one space)
    std::cout << max_diff(bst) << std::endl; //prints 3

    return 0;
}

