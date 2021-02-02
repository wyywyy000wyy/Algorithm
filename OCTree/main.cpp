#include "OCTree.h"


int main()
{
	OCTree<int> tree(Vector3(), 10.f);

	auto node1 = tree.Add(1, Vector3(1, 1, 1));
	auto node2 = tree.Add(2, Vector3(2, 2, 2));
	auto node3 = tree.Add(3, Vector3(3, 3, 3));
	//auto node4 = tree.Add(4, Vector3(4, 4, 4));
	//auto node5 = tree.Add(5, Vector3(5, 5, 5));
	//auto node6 = tree.Add(6, Vector3(6, 6, 6));

	tree.Remove(node3);

	return 0;
}