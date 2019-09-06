#include <iostream>
#include <string>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

//Global Variables
int counter = 0;

template<class T>
class Stack {
private:
	struct node {
		T data;
		node* next;
	};
	node* top;
	node* curr;
public:
	Stack() {
		top = NULL;
		curr = NULL;
	}
	char TOP();
	bool isEmpty();
	void pop();
	void push(T);
	void print();
	bool balance(string);
	string inToPost(string);
	int prec(char);
	int evaluatePost(string);
	int applyOp(int, int, char);
};

template<class T>
char Stack<T>::TOP() {
	return top->data;
}

template<class T>
bool Stack<T>::isEmpty() {
	if (top == NULL)
		return true;
	else
		return false;
}

template<class T>
void Stack<T>::push(T info) {
	node* n = new node; //pointer to create the new node
	n->data = info; //store newElement in the node
	n->next = top; //insert newNode before stackTop
	top = n;
}

template<class T>
void Stack<T>::pop() {
	node* temp;
	if (top != NULL) {
		temp = top;
		top = top->next;
		//delete temp;
	}
}

template<class T>
void Stack<T>::print() {
	curr = top;
	while (curr != NULL) {
		cout << curr->data;
		curr = curr->next;
	}
}

template<class T>
bool Stack<T>::balance(string line) {
	Stack<char> s;
	node* x;
	counter++;

	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '(' || line[i] == '[' || line[i] == '{') {
			s.push(line[i]);
			//s.print();
		}
		switch (line[i]) {
		case ')':
			x = s.top;
			s.pop();
			if (x->data == '{' || x->data == '[') {
				//cout << "Error at: " << counter << endl;
				return false;
			}
			break;
		case '}':
			x = s.top;
			s.pop();
			if (x->data == '(' || x->data == '[') {
				//cout << "Error at: " << counter << endl;
				return false;
			}
			break;

		case ']':
			x = s.top;
			s.pop();
			if (x->data == '(' || x->data == '{') {
				//cout << "Error at: " << counter << endl;
				return false;
			}
			break;
		}
	}
	if (!s.isEmpty())
		return false;

	// Check Empty Stack 
	return (s.isEmpty());
}

template<class T>
string Stack<T>::inToPost(string line) {
	Stack<char> s;
	string newline;

	for (int i = 0; i < line.length(); i++) {
		if (isalnum(line[i])) {
			newline = newline + line[i];
		}
		else if (line[i] == '(') // || line[i] == '[' || line[i] == '{'
			s.push(line[i]);
		else if (line[i] == ')') { // || line[i] == ']' || line[i] == '}'
			while (!s.isEmpty() && s.TOP() != '(') { // || s.TOP() != '[' || s.TOP() != '{'
				char c = s.TOP();
				s.pop();
				newline = newline + c;
			}
			if (s.TOP() == '(') {
				char c = s.TOP();
				s.pop();

			}
		}
		else if (line[i] == '[') // || line[i] == '[' || line[i] == '{'
			s.push(line[i]);
		else if (line[i] == ']') { // || line[i] == ']' || line[i] == '}'
			while (!s.isEmpty() && s.TOP() != '[') { // || s.TOP() != '[' || s.TOP() != '{'
				char c = s.TOP();
				s.pop();
				newline = newline + c;
			}
			if (s.TOP() == '[') {
				char c = s.TOP();
				s.pop();

			}
		}
		else if (line[i] == '{') // || line[i] == '[' || line[i] == '{'
			s.push(line[i]);
		else if (line[i] == '}') { // || line[i] == ']' || line[i] == '}'
			while (!s.isEmpty() && s.TOP() != '{') { // || s.TOP() != '[' || s.TOP() != '{'
				char c = s.TOP();
				s.pop();
				newline = newline + c;
			}
			if (s.TOP() == '{') {
				char c = s.TOP();
				s.pop();

			}
		}
		else {
			while (!s.isEmpty() && prec(line[i]) <= prec(s.TOP())) {
				char c = s.TOP();
				s.pop();
				newline = newline + c;
			}
			if ((line[0] == '-' || line[0] == '+') && i == 0)
				newline = newline + '0';
			else if (line[i - 1] == '(' || line[i - 1] == '[' || line[i - 1] == '{')
				newline = newline + '0';
			s.push(line[i]);
		}
	}
	while (!s.isEmpty()) {
		char c = s.TOP();
		s.pop();
		newline = newline + c;
	}
	return newline;
}


template<class T>
int Stack<T>::prec(char c) {
	if (c == '+' || c == '-')
		return 1;
	else
		return -1;
}

template <class T>
int Stack<T>::evaluatePost(string line) {
	Stack<char> s;

	for (int i = 0; i < line.length(); i++) {
		if (isalnum(line[i])) {
			if (isalpha(line[i]))
				s.push((line[i] - '0')*(line[i] - '0'));
			else
				s.push(line[i] - '0');
		}
		else if (line[i] == '+' || line[i] == '-') {
			int val2 = s.TOP();
			s.pop();

			int val1 = s.TOP();
			s.pop();;

			s.push(applyOp(val1, val2, line[i]));
		}
	}
	return s.TOP();
}

template<class T>
int Stack<T>::applyOp(int a, int b, char op) {
	switch (op) {
	case '+': return a + b;
	case '-': return a - b;
	}
}

int main(int argc, char* argv[])
{
	ArgumentManager am(argc, argv);
	std::string inputFile = am.get("input");
	std::string outputFile = am.get("output");

	//input/output files
	//string inputFile = "input1.txt";
	//string outputFile = "output1.txt";

	ifstream inputs(inputFile);
	ofstream out(outputFile);

	int errorCount = 0, count = 0;
	int* arr = new int[1000];
	int* err = new int[1000];
	string line, newline;
	Stack<string> nl;
	Stack<char> s;
	Stack<int> intype;

	while (getline(inputs, line)) {
		if (line == "")
			continue;
		else {
			if (s.balance(line)) {
				newline = nl.inToPost(line);
				int num = intype.evaluatePost(newline);
				arr[count] = num;
				count++;
			}
			else {
				err[errorCount] = counter;
				errorCount++;
			}
		}
	}


	int no = 0;
	if (errorCount > 0) {
		for (int i = 0; i < errorCount; i++)
			out << "Error at: " << err[i] << endl;
	}
	else {
		for (int j = 1; j < count; j++)
			if (arr[j - 1] != arr[j]) {
				out << "No" << endl;
				no = 1;
				break;
			}
			else
				no = 2;
		if (no == 2 || count == 1)
			out << "Yes" << endl;;

	}
	/*
	line = "a+{(b-[A+1])+3+B}";
	cout << "original: +a+(b-(A+1)+3+B)" << endl;
	newline = nl.inToPost(line);
	cout << "postfix: " << newline << endl;
	int num = intype.evaluatePost(newline);
	cout << num << endl;
	*/
	system("pause");
	return 0;
}
