#include "bst_imp.h"
#include <iostream>

int main() {
	//BST<int> tree({2,1});
	//std::cout << tree.min() << std::endl;
	BST<int> tree2(2, BST<int>(1, BST<int>(), BST<int>()), BST<int>());
	std::cout << tree2.min() << std::endl;
    return 0;
}

