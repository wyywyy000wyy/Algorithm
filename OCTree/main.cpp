#include "OCTree.h"


int main()
{
	OCTree<int> tree(Vector3(), 100.f);

	tree.Add(1, Vector3(1, 1, 1));
	tree.Add(2, Vector3(2, 2, 2));

	return 0;
}