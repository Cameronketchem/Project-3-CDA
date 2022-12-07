#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <time.h>

using namespace std;

int maximum(int value1, int value2) {
	if (value1 > value2) {
		return value1;
	}
	else {
		return value2;
	}
}

class bookNode {
public:
	string root;
	vector<string> additionalInfo;
	bookNode* left = NULL;
	bookNode* right = NULL;
	int height = 0;

	bookNode(string book, string author, string year) {
		vector <string> temp;
		temp.push_back(author);
		temp.push_back((year));
		temp.push_back("0");
		root = book;
		additionalInfo = temp;
	}
	//sets height based on whichever bookNode child has the highest height value
	void setHeight(int value1, int value2) {
		int max = maximum(value1, value2);
		height = 1 + max;
	}
};

//checks height of bookNode and whether it is NUll or not.
int height(bookNode* N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

//checks the balance of the bookbookNode based on the child situation
int balance(bookNode* current) {
	if (current == NULL) {
		return 0;
	}
	if (current->left == NULL && current->right == NULL) {
		return 0;
	}
	else if (current->left != NULL && current->right == NULL) {
		return (current->left->height);
	}
	else if (current->left == NULL && current->right != NULL) {
		return (-current->right->height);
	}
	else if (current->left != NULL && current->right != NULL) {
		return (current->left->height - current->right->height);
	}
}

//left left rotation
bookNode* leftLeft(bookNode* root) {
	bookNode* leftNode = root->left;
	bookNode* secondNode = leftNode->left;
	root->left = NULL;
	root->right = NULL;

	leftNode->right = root;
	leftNode->left = secondNode;

	root->setHeight(0, 0);
	leftNode->setHeight(height(leftNode->left), height(leftNode->right));
	secondNode->setHeight(height(secondNode->left), height(secondNode->left));
	return leftNode;
}

//right right rotation
bookNode* rightRight(bookNode* root) {
	bookNode* rightNode = root->right;
	bookNode* secondNode = rightNode->right;
	root->left = NULL;
	root->right = NULL;

	rightNode->left = root;
	rightNode->right = secondNode;

	root->setHeight(0, 0);
	rightNode->setHeight(height(rightNode->left), height(rightNode->right));
	secondNode->setHeight(height(secondNode->left), height(secondNode->left));
	return rightNode;
}

//left right rotation
bookNode* leftRight(bookNode* root) {
	bookNode* leftNode = root->left;
	bookNode* rightNode = leftNode->right;
	root->left = NULL;
	root->right = NULL;

	rightNode->right = root;
	rightNode->left = leftNode;

	root->setHeight(0, 0);
	rightNode->setHeight(height(rightNode->left), height(rightNode->right));
	leftNode->setHeight(height(leftNode->left), height(leftNode->left));
	return rightNode;
}

//right left rotation
bookNode* rightLeft(bookNode* root) {
	bookNode* rightNode = root->right;
	bookNode* leftNode = rightNode->left;
	root->left = NULL;
	root->right = NULL;

	leftNode->right = rightNode;
	leftNode->left = root;

	root->setHeight(0, 0);
	rightNode->setHeight(height(rightNode->left), height(rightNode->right));
	leftNode->setHeight(height(leftNode->left), height(leftNode->left));
	return leftNode;
}

class customMap {
	int nodeCount = 0;
	vector <pair<int, string>> rankings;
public:
	int max = 0;
	bookNode* head;
	customMap() {
		head = NULL;
	}

	customMap(bookNode* head) {
		nodeCount++;
		this->head = head;
	}

	bookNode* Insert(bookNode* top, bookNode* addition) {
		//determines whether the tree is empty or if the new addition is greater than, less than or equal to the current bookNode
		if (top == NULL) {
			nodeCount++;
			addition->setHeight(0, 0);
			top = addition;
			return addition;
		}
		else if (top->root.compare(addition->root) < 0) {
			top->left = Insert(top->left, addition);
		}
		else if (top->root.compare(addition->root) > 0) {
			top->right = Insert(top->right, addition);
		}
		else if (top->root.compare(addition->root) == 0) {
			return top;
		}

		//sets the height of the new top bookNode
		if (top->left == NULL && top->right == NULL) {
			top->setHeight(0, 0);
		}
		else if (top->left != NULL && top->right == NULL) {
			top->setHeight(top->left->height, 1);
		}
		else if (top->left == NULL && top->right != NULL) {
			top->setHeight(1, top->right->height);
		}
		else if (top->left != NULL && top->right != NULL) {
			top->setHeight(top->left->height, top->right->height);
		}
		int balanced = balance(top);
		return top;
	}

	//searches for the book with the gi en name
	bool searchName(bookNode* top, string name) {
		bool found = false;
		if (top != NULL) {
			if (name.compare(top->root) == 0) {
				cout << "Book Name: " << top->root << endl;
				cout << "Author Name: " << top->additionalInfo.at(0) << endl;
				cout << "Year Published: " << top->additionalInfo.at(1) << endl;
				cout << endl;
				found = true;
			}
			else if (name.compare(top->root) < 0)
			{
				found = searchName(top->left, name);
			}
			else if (name.compare(top->root) > 0) {
				found = searchName(top->right, name);
			}
		}
		return found;
	}

	//searches for names inOrder count is used to count how many times an book by the author is found
	int searchAuthor(bookNode* top, string author, int count) {
		if (top != NULL) {
			if (top->left != NULL) {
				count = searchAuthor(top->left, author, count);
			}
			if (top->additionalInfo.at(0).find(author) != string::npos) {
				count++;
				cout << "Book Name: " << top->root << endl;
				cout << "Author Name: " << top->additionalInfo.at(0) << endl;
				cout << "Year Published: " << top->additionalInfo.at(1) << endl;
				cout << endl;
				top->additionalInfo.at(2) = to_string(stoi(top->additionalInfo.at(2)) + 5);
			}
			if (top->right != NULL) {
				count = searchAuthor(top->right, author, count);
			}
			return count;
		}
	}

	//searches through entire database to find any book published in that year
	int searchYear(bookNode* top, string year, int count) {
		if (top != NULL) {
			if (top->left != NULL) {
				count = searchYear(top->left, year, count);
			}
			if (top->additionalInfo.at(1).compare(year) == 0) {
				count++;
				cout << "Book Name: " << top->root << endl;
				cout << "Author Name: " << top->additionalInfo.at(0) << endl;
				cout << "Year Published: " << top->additionalInfo.at(1) << endl;
				cout << endl;
				top->additionalInfo.at(2) = to_string(stoi(top->additionalInfo.at(2)) + 1);
			}
			if (top->right != NULL) {
				count = searchYear(top->right, year, count);
			}
			return count;
		}
	}

	//Checks whether there is a book that contains the given keyword
	int searchKeyWord(bookNode* top, string word, int count) {
		if (top != NULL) {
			if (top->left != NULL) {
				count = searchKeyWord(top->left, word, count);
			}
			if (top->root.find(word) != string::npos) {
				count++;
				cout << "Book Name: " << top->root << endl;
				cout << "Author Name: " << top->additionalInfo.at(0) << endl;
				cout << "Year Published: " << top->additionalInfo.at(1) << endl;
				cout << endl;
				top->additionalInfo.at(2) = to_string(stoi(top->additionalInfo.at(2)) + 3);
			}
			if (top->right != NULL) {
				count = searchKeyWord(top->right, word, count);
			}
			return count;
		}
	}

	//inorder printing
	void printAll(bookNode* top) {
		if (top != NULL) {
			printAll(top->left);
			cout << "Book Name: " << top->root << endl;
			cout << "Author Name: " << top->additionalInfo.at(0) << endl;
			cout << "Year Published: " << top->additionalInfo.at(1) << endl;
			cout << endl;
			printAll(top->right);
		}
		else {
			return;
		}
	}

	//prints out the best books for the user to read
	int setRanks(bookNode* top, int count) {
		if (count == 10) {
			return count;
		}
		if (top != NULL) {
			if (stoi(top->additionalInfo.at(2)) == max) {
				cout << "#" << count + 1 << endl;
				cout << "Book Name: " << top->root << endl;
				cout << "Author Name: " << top->additionalInfo.at(0) << endl;
				cout << "Year Published: " << top->additionalInfo.at(1) << endl;
				cout << endl;
				count++;
			}
			count = setRanks(top->left, count);
			count = setRanks(top->right, count);
		}
		return count;
	}
};



int main() {
	ifstream inFile;
	vector <pair<int, string>> rankings;
	bookNode* head = NULL;
	customMap bookStorage = customMap();

	//opens the file
	inFile.open("books1.csv");
	if (!inFile.is_open()) {
		cout << "File didn't open somethings wrong" << endl;
		return 0;
	}

	string name, author, yearPublished, ISBN, line;
	//is used to get rid of the first line which contains headers for the columns
	getline(inFile, line);

	//loop will go through until there are no more rows to go through
	while (getline(inFile, line)) {
		stringstream row(line);
		getline(row, ISBN, ';');
		if (ISBN == "") {
			break;
		}
		getline(row, name, ';');
		getline(row, author, ';');
		getline(row, yearPublished, ';');
		bool valid = true;
		bookNode* temp = new bookNode(name, author, yearPublished);
		if (bookStorage.head == NULL) {
			bookStorage.head = temp;
			head = temp;
		}
		else {
			head = bookStorage.Insert(head, temp);
		}
	}

	//loop will run until user chooses to exit
	bool keepGoing = true;
	while (keepGoing) {
		cout << "What would you like to do?" << endl;
		cout << "1. Print all books" << endl;
		cout << "2. Search for a book" << endl;
		cout << "3. Search by key word" << endl;
		cout << "4. Search by year published" << endl;
		cout << "5. Search by author" << endl;
		cout << "6. Exit" << endl;

		int option;
		cin >> option;

		//runs print all function
		if (option == 1) {
			bookStorage.printAll(head);
		}
		//runs search name function
		else if (option == 2) {
			cout << "What's the books name?" << endl;
			string name;
			cin >> name;
			getline(cin, name);
			clock_t start = clock();
			bool found = false;
			found = bookStorage.searchName(head, name);
			clock_t end = clock();
			double elapsed = double(end - start) / (double)(CLOCKS_PER_SEC / 1000);
			cout << elapsed << " milliseconds passed" << endl;
			cout << endl;
			if (!found) {
				cout << "Sorry this book isn't in the database" << endl;
				cout << endl;
			}
		}
		//runs search key word function
		else if (option == 3) {
			cout << "What's the key word?" << endl;
			string word;
			cin >> word;
			int counter = 0;
			clock_t start = clock();
			counter = bookStorage.searchKeyWord(head, word, 0);
			clock_t end = clock();
			double elapsed = double(end - start) / (double)(CLOCKS_PER_SEC / 1000);
			cout << elapsed << " milliseconds passed" << endl;
			cout << endl;
			if (counter > 0) {
				bookStorage.max += 3;
			}
			else {
				cout << "No book names in the database contain that key word" << endl;
				cout << endl;
			}
		}
		//runs search year function
		else if (option == 4) {
			cout << "What year do you want?" << endl;
			string year;
			cin >> year;
			//getline(cin,year);
			clock_t start = clock();
			int counter = 0;
			counter = bookStorage.searchYear(head, year, 0);
			clock_t end = clock();
			double elapsed = double(end - start) / (double)(CLOCKS_PER_SEC / 1000);
			cout << elapsed << " milliseconds passed" << endl;
			cout << endl;
			if (counter > 0) {
				bookStorage.max += 1;
			}
			else {
				cout << "No books made in this year are in the database" << endl;
				cout << endl;
			}
		}
		//runs search author function
		else if (option == 5) {
			cout << "What author do you want?" << endl;
			string author;
			int counter = 0;
			cin >> author;
			getline(cin,author);
			clock_t start = clock();
			counter = bookStorage.searchAuthor(head, author, 0);
			clock_t end = clock();
			double elapsed = double(end - start) / (double)(CLOCKS_PER_SEC / 1000);
			cout << elapsed << " milliseconds passed" << endl;
			cout << endl;
			if (counter > 0) {
				bookStorage.max += 5;
			}
			else {
				cout << "No books by this author are in the database!" << endl;
				cout << endl;
			}
		}
		//prints out the ranked books list
		else if (option == 6) {
			keepGoing = false;
			cout << "Here's a list of book recommendations!" << endl;
			clock_t start = clock();
			bookStorage.setRanks(head, 0);
			cout << endl;
			clock_t end = clock();
			double elapsed = double(end - start) / (double)(CLOCKS_PER_SEC / 1000);
			cout << elapsed << " milliseconds passed" << endl;
		}
		//if incorrect input is given will tell user to use the right input
		else {
			cout << "Need a number between 1-6" << endl;
		}
		
	}

}