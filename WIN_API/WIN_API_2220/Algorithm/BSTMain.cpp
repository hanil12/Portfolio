#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

#include "BinarySearchTree.h"

// ����Ž��Ʈ��
// - Ž���뵵�� ������� Ʈ��

int main()
{
	BinarySearchTree tree;

	tree.Insert(20);
	tree.Insert(30);
	tree.Insert(40);
	tree.Insert(50);
	tree.Insert(52);
	tree.Insert(54);
	tree.Insert(70);
	tree.Insert(85);

	Node* min = tree.Min(tree._root);
	Node* search = tree.Search(tree._root, 70);
	Node* prev = tree.Previous(search);

	tree.Delete(tree.Search(tree._root, 30));
	tree.PrintTree(tree._root);
	

	return 0;
}