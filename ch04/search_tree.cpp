#include"search_tree.h"
#include<stdexcept>
#include<random>
#include<ctime>
#include<queue>

SearchTree::SearchTree(const SearchTree& st)
{
	Copy(st.root, root);
}

SearchTree& SearchTree::operator=(SearchTree st)
{
	std::swap(root, st.root);
	return *this;
}

SearchTree::~SearchTree()
{
	MakeEmpty(root);
}

void  SearchTree::Copy(Tree to_copy,Tree& dest)
{
	if (to_copy) {
		dest = new TreeNode(to_copy->data);
		Copy(to_copy->left,dest->left);
		Copy(to_copy->right,dest->right);
	}
}

void SearchTree::MakeEmpty(Tree& root)
{
	if (root) {
		MakeEmpty(root->left);
		MakeEmpty(root->right);
		delete root;
		root = nullptr;
	}
}

void SearchTree::MakeEmpty()
{
	MakeEmpty(root);
}

Position SearchTree::Find(int x, Tree root)
{
	if (root == nullptr)
		return nullptr;
	if (x < root->data)
		return Find(x, root->left);
	else if (x > root->data)
		return Find(x, root->right);
	else
		return root;
}

Position SearchTree::Find(int x)
{
	return Find(x, root);
}

Position SearchTree::FindMin(Tree root)
{
	if (root == nullptr)
		return nullptr;
	else if (root->left == nullptr)
		return root;
	else
		return FindMin(root->left);
}

int SearchTree::Min()
{
	if (!IsEmpty()) {
		Position p = FindMin(root);
		return p->data;
	}
	throw std::runtime_error("Empty tree");
	return 0;
}

Position SearchTree::FindMax(Tree root)
{
	if (root == nullptr)
		return nullptr;
	else if (root->right == nullptr)
		return root;
	else
		return FindMax(root->right);
}

int SearchTree::Max()
{
	if (!IsEmpty()) {
		Position p = FindMax(root);
		return p->data;
	}
	throw std::runtime_error("Empty tree");
	return 0;
}

Tree SearchTree::Insert(int x, Tree root)
{
	if (root == nullptr)
		root = new TreeNode(x);
	else if (x < root->data)
		root->left = Insert(x, root->left); 
				//need to update root->left because the parameter 
	            //is just a copy of the pointer. The following is the same.
	else if (x > root->data)
		root->right = Insert(x, root->right);
	return root;  //don't forget this line
}

Tree SearchTree::Insert(int x)
{
	root = Insert(x, root);
	return root;
}

Tree SearchTree::Delete(int x, Tree root)   //use the minimum of right subtree as substitute
{
	if (root == nullptr)
		throw std::runtime_error("Data not found");
	else if (x < root->data)
		root->left = Delete(x, root->left);
	else if (x > root->data)
		root->right = Delete(x, root->right);
	else if (root->left && root->right) {  //has two children
		Position temp = FindMin(root->right);   //replace with smallest in right subtree
		root->data = temp->data;
		root->right = Delete(root->data, root->right);
	}
	else {  //has one or zero children
		Position temp = root;
		if (root->left == nullptr)
			root = root->right;
		else if (root->right == nullptr)
			root = root->left;
		delete temp;
	}
	return root;
}


Tree SearchTree::Delete(int x)
{
	root = Delete(x, root);
	return root;
}

int SearchTree::Retrieve(Position p)
{
	if (p)
		return p->data;
	throw std::runtime_error("Null pointer");
	return 0;
}

std::istream& operator>>(std::istream& is, SearchTree& st)
{
	int elem;
	while (is >> elem) 
		st.Insert(elem);
	return is;
}

std::ostream& operator<<(std::ostream& os, const SearchTree& st)
{
	st.Print(os, st.root);
	return os;
}

void SearchTree::Print(std::ostream& os, const Tree& root) const
{
	if (root) {
		os << root->data << " ";
		Print(os, root->left);
		Print(os, root->right);
	}
}

int RandomInt(int lower, int upper)
{
	std::default_random_engine e(time(NULL));
	std::uniform_int_distribution<unsigned> u(lower, upper);
	return u(e);
}

Tree SearchTree::MakeRandomSearchTree(int lower, int upper)
{
	Tree root = nullptr;
	int value;
	if (lower <= upper) {
		value = RandomInt(lower, upper);
		root = new TreeNode(value);
		root->left = MakeRandomSearchTree(lower, value - 1);
		root->right = MakeRandomSearchTree(value + 1, upper);
	}
	return root;
}

void SearchTree::MakeRandomSearchTree(int N)
{
	root = MakeRandomSearchTree(1, N);
}

Tree SearchTree::PerfectTree(int lower, int upper)
{
	Tree root = nullptr;
	if (lower <= upper) {
		int value = (lower + upper) / 2;
		root = new TreeNode(value);
		root->left = PerfectTree(lower, value - 1);
		root->right = PerfectTree(value + 1, upper);
	}
	return root;
}

void SearchTree::PerfectTree(int height)
{
	root = PerfectTree(1, static_cast<int>(std::pow(2, height + 1)) - 1);
}


//Tree SearchTree::PerfectTree(int height, int& elem)
//{
//	Tree root = nullptr;
//	if (height >= 0) {
//		root = new TreeNode();
//		root->left = PerfectTree(height - 1, elem);
//		root->data = ++elem;
//		root->right = PerfectTree(height - 1, elem);
//	}
//	return root;
//}
//
//void SearchTree::PerfectTree(int height)
//{
//	int value = 0;
//	root = PerfectTree(height, value);
//}

void SearchTree::PrintRange(Tree t, int lower, int upper)
{
	if (t) {
		if (lower < t->data)
			PrintRange(t->left, lower, upper);
		if (t->data >= lower && t->data <= upper)
			std::cout << t->data << " ";
		if (t->data < upper)
			PrintRange(t->right, lower, upper);
	}
}

void SearchTree::PrintRange(int lower, int upper)
{
	PrintRange(root, lower, upper);
}

void SearchTree::LevelOrderTraverse()
{
	std::queue<Tree> q;
	q.push(root);
	while (!q.empty()) {
		std::cout << q.front()->data << " ";
		if (q.front()->left)
			q.push(q.front()->left);
		if (q.front()->right)
			q.push(q.front()->right);
		q.pop();
	}
}