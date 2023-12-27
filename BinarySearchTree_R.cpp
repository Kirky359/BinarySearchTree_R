#include <set>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

long long generateRandLong()
{
	int quantity = rand() % 5 + 11;
	long long int longNumber = 0;
	int n;
	for (int i = 0; i < quantity; i++) {
		n = rand() % 10;
		longNumber += n * pow(10, i);
		if (i + 1 == quantity) {
			n = (rand() % 9) + 1;
			longNumber += n * pow(10, i);
		}
	}
	return longNumber;
}

struct Data
{
	unsigned int rank;
	int exp;
	int winrate;
	char name;
	Data() {
		char abc[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
		name = abc[rand() % 26];
		rank = rand() % 7201;
		exp = rand() % 3001;
		winrate = rand() % 101;
	}

	bool operator<(const Data& second) const
	{
		if (winrate != second.winrate)
		{
			return winrate < second.winrate;
		}

		else if (rank != second.rank)
		{
			return rank < second.rank;
		}
		return exp < second.exp;
	}

	bool operator==(const Data& second) const
	{
		return (rank == second.rank && exp == second.exp && winrate == second.winrate);
	}

};

struct Node {
	Data data;
	Node* left = nullptr;
	Node* right = nullptr;

};


struct BinarySearchTree {
public:
	int size;
	Node* root;

	BinarySearchTree() {
		size = 0;
		root = nullptr;
	}

	void insert(Data& object) {
		Node* newObject = new Node;
		newObject->data = object;
		/*cout << "Inserted:" << object.winrate << " " << object.exp << " " << object.rank<< " " << object.name << endl;*/
		if (size == 0) {
			root = newObject;
			size++;
			return;
		}
		Node* current = root;

		while (true) {
			if (object < current->data) {
				if (current->left == nullptr) {
					current->left = newObject;
					size++;
					break;
				}
				else {
					current = current->left;
				}
			}
			else if (current->data < object) {
				if (current->right == nullptr) {
					current->right = newObject;

					size++;
					break;
				}
				else {
					current = current->right;
				}
			}
			else {
				delete newObject;
				return;
			}
		}
	}

	int Size() {
		return size;
	}


	void erase(Data& data) {
		if (eraseIn(root, data)) {
			size--;
		}
	}
	bool find(Data& data) {
		return findIn(root, data);
	}

	void print() {
		inorderTraversal(root);
	}

	int height() {
		return treeHeight(root) - 1;
	}

	int findInRange(Data& minObject, Data& maxObject) {
		int count = 0;
		findInRange(root, minObject, maxObject, count);
		return count;
	}

	~BinarySearchTree() {
		clearHelper(root);
		root = nullptr;
	}
private:

	Node* findInorderSuccessor(Node* current) {

		current = current->right;
		while (current->left != nullptr) {
			current = current->left;
		}
		return current;
	}

	bool eraseIn(Node*& current, Data& data) {
		if (current == nullptr) {
			return false;
		}
		bool erased = false;
		if (data < current->data) {
			erased = eraseIn(current->left, data);
		}
		else if (current->data < data) {
			erased = eraseIn(current->right, data);
		}
		else {
			if (current->left == nullptr) {
				Node* temp = current->right;
				delete current;
				current = temp;
				return true;
			}
			else if (current->right == nullptr) {
				Node* temp = current->left;
				delete current;
				current = temp;
				return true;
			}

			Node* successor = findInorderSuccessor(current);
			swap(current->data, successor->data);
			erased = eraseIn(current->right, successor->data);
		}
	}




	bool findIn(Node* current, Data& data) {
		if (current == nullptr) {
			return false;
		}
		else if (data < current->data) {
			return findIn(current->left, data);
		}
		else if (current->data < data) {
			return findIn(current->right, data);
		}
		return true;
	}
	void inorderTraversal(Node* current) {
		if (current == nullptr) {
			return;
		}
		inorderTraversal(current->left);
		cout << "Printed:" << current->data.winrate << " " << current->data.exp << " " << current->data.rank << " " << current->data.name << endl;
		inorderTraversal(current->right);
	}

	int treeHeight(Node* current) {
		if (current == nullptr) {
			return 0;
		}
		int leftHeight = treeHeight(current->left);
		int rightHeight = treeHeight(current->right);
		return max(leftHeight, rightHeight) + 1;
	}

	void findInRange(Node* current, Data& minObject, Data& maxObject, int& count) {
		if (current == nullptr) {
			return;
		}

		if (minObject < current->data) {
			findInRange(current->left, minObject, maxObject, count);
		}

		if (!(current->data < minObject) && (current->data < maxObject)) {

			count++;
		}

		if (current->data < maxObject) {
			findInRange(current->right, minObject, maxObject, count);
		}
	}




	void clearHelper(Node* node) {
		if (node == nullptr) {
			return;
		}
		clearHelper(node->left);
		clearHelper(node->right);
		delete node;
	}
};

bool testBinarySearchTree()
{
	srand(time(NULL));
	const int iters = 80000;
	const int keysAmount = iters * 2;
	const int itersToRangeQueries = 1000;
	vector<Data> data(keysAmount);
	vector<Data> dataToInsert(iters);
	vector<Data> dataToErase(iters);
	vector<Data> dataToFind(iters);
	vector<pair<Data, Data>> dataToRangeQueries;
	for (int i = 0; i < iters; i++)
	{
		dataToInsert[i] = data[generateRandLong() % keysAmount];
		dataToErase[i] = data[generateRandLong() % keysAmount];
		dataToFind[i] = data[generateRandLong() % keysAmount];
	}
	for (int i = 0; i < itersToRangeQueries; i++)
	{
		Data minData = Data();
		Data maxData = Data();
		if (maxData < minData)
		{
			swap(minData, maxData);
		}
		dataToRangeQueries.push_back({ minData, maxData });
	}
	BinarySearchTree myTree;
	clock_t myStart = clock();
	for (int i = 0; i < iters; i++)
	{
		myTree.insert(dataToInsert[i]);
	}
	int myInsertSize = myTree.Size();
	int myTreeHeight = myTree.height();
	int optimalTreeHeight = log2(myInsertSize) + 1;
	for (int i = 0; i < iters; i++)
	{
		myTree.erase(dataToErase[i]);
	}
	int myEraseSize = myInsertSize - myTree.Size();
	int myFoundAmount = 0;
	for (int i = 0; i < iters; i++)
	{
		if (myTree.find(dataToFind[i]))
		{
			myFoundAmount++;
		}
	}
	clock_t myEnd = clock();
	float myTime = (float(myEnd - myStart)) / CLOCKS_PER_SEC;

	set<Data> stlTree;
	clock_t stlStart = clock();
	for (int i = 0; i < iters; i++)
	{
		stlTree.insert(dataToInsert[i]);
	}
	int stlInsertSize = stlTree.size();
	for (int i = 0; i < iters; i++)
	{
		stlTree.erase(dataToErase[i]);
	}
	int stlEraseSize = stlInsertSize - stlTree.size();
	int stlFoundAmount = 0;
	for (int i = 0; i < iters; i++)
	{
		if (stlTree.find(dataToFind[i]) != stlTree.end())
		{
			stlFoundAmount++;
		}
	}
	clock_t stlEnd = clock();
	float stlTime = (float(stlEnd - stlStart)) / CLOCKS_PER_SEC;
	clock_t myRangeStart = clock();
	int myRangeFoundAmount = 0;
	for (int i = 0; i < itersToRangeQueries; i++)
	{
		myRangeFoundAmount += myTree.findInRange(dataToRangeQueries[i].first,
			dataToRangeQueries[i].second);
	}
	clock_t myRangeEnd = clock();
	float myRangeTime = (float(myRangeEnd - myRangeStart)) / CLOCKS_PER_SEC;
	clock_t stlRangeStart = clock();
	int stlRangeFoundAmount = 0;
	for (int i = 0; i < itersToRangeQueries; i++)
	{
		const auto& low = stlTree.lower_bound(dataToRangeQueries[i].first);
		const auto& up = stlTree.upper_bound(dataToRangeQueries[i].second);
		stlRangeFoundAmount += distance(low, up);
	}
	clock_t stlRangeEnd = clock();
	float stlRangeTime = (float(stlRangeEnd - stlRangeStart)) / CLOCKS_PER_SEC;
	cout << "My BinaryTree: height = " << myTreeHeight << ", optimal height = " <<
		optimalTreeHeight << endl;
	cout << "Time: " << myTime << ", size: " << myInsertSize << " - " << myEraseSize <<
		", found amount : " << myFoundAmount << endl;
	cout << "Range time: " << myRangeTime << ", range found amount: " << myRangeFoundAmount
		<< endl << endl;
	cout << "STL Tree:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlInsertSize << " - " << stlEraseSize <<
		", found amount: " << stlFoundAmount << endl;
	cout << "Range time: " << stlRangeTime << ", range found amount: " <<
		stlRangeFoundAmount << endl << endl;
	if (myInsertSize == stlInsertSize && myEraseSize == stlEraseSize &&
		myFoundAmount == stlFoundAmount && myRangeFoundAmount == stlRangeFoundAmount)
	{
		cout << "The lab is completed" << endl;
		return true;
	}
	cerr << ":(" << endl;
	return false;
}


int main()
{
	BinarySearchTree BST;
	srand(time(NULL));
	/*for (int i = 0; i < 10; i++) {
		Data data;
		BST.insert(data);
	}
	cout << endl;
	BST.print();
	cout << "height:" << BST.height() << endl;
	cout << "size:" << BST.Size() << endl;
	Data data1;
	cin >> data1.winrate;
	cin >> data1.exp;
	cin >> data1.rank;
	cin >> data1.name;
	cout << endl;
	BST.erase(data1);
	cout << "Print after erase:" << endl;
	BST.print();
	cout << "height:" << BST.height() << endl;
	cout << "size:" << BST.Size() << endl;*/
	testBinarySearchTree();

}



