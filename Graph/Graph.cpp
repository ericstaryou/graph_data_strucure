#include <iostream>
#include <string>

using namespace std;

template <class T>
class StackNode{
public:
	T info;
	StackNode<T> * link;
};

template <class T>
class Stack{
	StackNode<T> * head;
public:
	Stack(){
		head = NULL;
	}

	~Stack(){
		StackNode<T> * current = head;

		while (current != NULL){
			current = current->link;
			delete head;
			head = current;
		}
	}

	void push(T element){ //insert at beginning
		StackNode<T> * newNode = new StackNode<T>;
		newNode->info = element;
		newNode->link = head;
		head = newNode;
	}

	T pop(){
		if (head != NULL){
			StackNode<T> * toBeDeleted = head;
			head = head->link;
			T temp = toBeDeleted->info;
			delete toBeDeleted;
			return temp;
		}
		else{
			cout << "the stacks is empty!" << endl;
			return NULL;
		}
	}

	T top(){
		if (head != NULL){
			return head->info;
		}
		else{
			cout << "the stacks is empty!" << endl;
			return NULL;
		}
	}

	bool isEmpty(){
		return (head == NULL);
	}

	void printStack(){
		StackNode<T> * current = head;
		while (current != NULL){
			cout << " " << current->info << " ";
			current = current->link;
		}
		cout << endl;
	}
};

template <class T>
class NodeType{
public:
	int columnIndex;
	T info;
	NodeType<T> * link;
};

template <class T>
class LinkedList{
public:
	NodeType<T>* head;
	NodeType<T>* tail;
	int size;

	LinkedList(){
		head = NULL;
		tail = NULL;
		size = 0;
	}

	~LinkedList(){
		clear();
	}

	int getSize(){
		return size;
	}

	bool search(int &index){
		NodeType<T>* p = head;

		while (p != NULL){
			if (p->columnIndex == index)
				return true;  //when there is this value, it will return true and straight away exit this function. (no need worry about the loop)
			else
				p = p->link;
		}

		return false;  //when it is out of the loop, it means there is no searched value.
	}

	void setValueAt(int &value, int colIndex){
		NodeType<T>* p = head;

		while (p != NULL){
			if (p->columnIndex == colIndex){
				p->info = value; break;
			}
			else
				p = p->link;
		}
	}

	int getValueAt(int colIndex){
		NodeType<T>* p = head;

		while (p != NULL){
			if (p->columnIndex == colIndex)
				return p->info;  
			else
				p = p->link;
		}

		return 0;  
	}

	void print(){
		NodeType<T> * current = head;
		while (current != NULL){
			cout << current->info << " ";
			current = current->link;
		}
		cout << endl;
	}

	void clear(){
		NodeType<T>* current = head;
		while (current != NULL){
			current = current->link;
			delete head;
			head = current;
		}
		if (head == NULL){
			tail = NULL; 
		}
		size = 0;
	}

	void insertWithOrder(int columnIndex, T value){
		NodeType<T>* newNode = new NodeType<T>;
		newNode->columnIndex = columnIndex;
		newNode->info = value;

		NodeType<T>* current = head;
		NodeType<T>* previous = nullptr;

		while (current != nullptr && newNode->columnIndex > current->columnIndex){
			previous = current;
			current = current->link;
		}

		if (previous != nullptr){
			previous->link = newNode;
			newNode->link = current;

			if (current == nullptr){
				tail = newNode;
			}

			size++;

		}
		else{
			NodeType<T>* temp = head;
			head = newNode;
			newNode->link = temp;
			size++;
		}

	}

	void specialDelete(int columnIndex){
		NodeType<T>* toDelete = head;
		NodeType<T>* prev = NULL;

		while (toDelete != NULL){
			if (toDelete->columnIndex == columnIndex){
				if (toDelete == head){ //if the node to be deleted is the first node
					head = head->link; //point the head to the second node

					if (toDelete == tail){ //if the node to be deleted is the last node
						tail = NULL;       //make the tail point at NULL!
					}

					delete toDelete; //delete the selected
					size--;
					return;
				}
				else{
					prev->link = toDelete->link; //make the previous node point to the node after the node that is going to be deleted

					if (toDelete == tail){ //if the node to be deleted is the last node
						tail = prev;       //make the tail point at the newest last node
					}

					delete toDelete;   //delete the node
					size--;
					return;
				}
			}
			else{							//if there is no matching node
				prev = toDelete;			//move both the pointers forward
				toDelete = toDelete->link;
			}
		}

		cout << "There is no such edge!" << endl;   //display message if there is no such node in the list
	}

};

class SM{
private:
	int rows, columns;

public:
	
	LinkedList<int> * matrix;

	SM(int rows, int columns){
		this->rows = rows;
		this->columns = columns;
		this->matrix = new LinkedList<int>[rows];
		for (int i = 0; i < rows; i++){
			this->matrix[i] = LinkedList<int>();
		}
	}

	SM(SM& other){
		this->rows = other.rows;
		this->columns = other.columns;
		this->matrix = new LinkedList<int>[rows];
		for (int i = 0; i < rows; i++){
			this->matrix[i] = other.matrix[i];
		}
	}

	~SM(){
		delete[] matrix;
	}
	
	void readElements(int row, int column, int value){
		matrix[row].insertWithOrder(column, value);
	}

	void removeElements(int row, int column){
		matrix[row].specialDelete(column);
	}

};

class AGraph{
private:
	SM * adList;
	int numOfVertices;

public:
	AGraph(int numOfVertices){
		this->numOfVertices = numOfVertices;
		this->adList = new SM(numOfVertices, numOfVertices);
	}

	~AGraph(){
		delete adList;
	}

	void addDirectedEdge(int u, int v){
		adList->readElements(u-1, v-1, 0);
	}

	void addUndirectedEdge(int u, int v){
		adList->readElements(u-1, v-1, 0);
		adList->readElements(v-1, u-1, 0);
	}

	void removeDirectedEdge(int u, int v){
		adList->removeElements(u-1, v-1);
	}

	void printTopologicalSort(){
		if (!isCyclic()){
			Stack<int> Stack;
			
			//mark all the vertices as not visited
			bool *visited = new bool[numOfVertices];
			for (int i = 0; i < numOfVertices; i++)
			{
				visited[i] = false;
			}

			//traverse each unvisited vertex and call the recursive helper function
			for (int i = 0; i < numOfVertices; i++)
			{
				if (visited[i] == false)
				{
					printTopologicalSort(i, visited, Stack); //both the 'visited' array and the stack data stucture are passed by reference
				}											 //into the recursive helper function.
			}

			//Print the content of the Stack
			while (Stack.isEmpty() == false)
			{
				cout << Stack.pop() + 1 << " ";
			}

			cout << endl;
		}
		else{
			cout << "This graph is a cyclic graph! Unable to perform topological sort." << endl;
		}
	}

	void printTopologicalSort(int v, bool visited[], Stack<int> &Stack){ //helper function
		//mark the current vertex as visited in the 'visited' array
		visited[v] = true;

		//traverse all the vertices that are adjacent to this current vertex 
		NodeType<int> * current = adList->matrix[v].head;
		while (current != NULL){

			//if the adjacent vertex is not visited, this helper function is recurred
			if (!visited[current->columnIndex])
				printTopologicalSort(current->columnIndex, visited, Stack);
			
			//move the current pointer
			current = current->link;
		}

		//Push the current vertex to the Stack that stores the sorted outcome
		Stack.push(v);
	}

	//return true if graph is a cycle, else false
	bool isCyclic(){

		//Mark all vertices as not visited
		//Mark all the vertices as not part of the recursion stack
		bool *visited = new bool[numOfVertices];
		bool *recStack = new bool[numOfVertices];

		for (int i = 0; i < numOfVertices; i++)
		{
			visited[i] = false;
			recStack[i] = false;
		}

		//traverse each vertices and call the recursive helper function
		for (int i = 0; i < numOfVertices; i++)
		{
			if (isCyclic(i, visited, recStack)){
				delete visited; //clean up heap memory
				delete recStack;
				return true;
			}
		}

		delete visited; //clean up heap memory
		delete recStack;
		return false;
		//hgjhghello
	}

	bool isCyclic(int v, bool visited[], bool *recStack){ //helper function
		if (visited[v] == false){

			//Mark the current vertex as visited and part of the recursion stack
			visited[v] = true;
			recStack[v] = true;

			//Access the current vertex's adjacent vertices
			NodeType<int> * current = adList->matrix[v].head;
			while (current != NULL){
				if (!visited[current->columnIndex] && isCyclic(current->columnIndex, visited, recStack)) //if the adjacent vertex is not visited and is a cyclic
					return true;
				else if (recStack[current->columnIndex]) //if the adjacent vertex is visited and it is currently in the recursion stack
					return true;

				current = current->link;
			}

		}
		recStack[v] = false; //remove the current vertex from recursion stack
		return false;
	}

};

int main(){
	AGraph g(8); //hello
	g.addDirectedEdge(1, 4);
	g.addDirectedEdge(1, 6);         
	g.addDirectedEdge(1, 2);
	g.addDirectedEdge(3, 8);
	g.addDirectedEdge(3, 2);
	g.addDirectedEdge(5, 1);
	g.addDirectedEdge(7, 1);
	g.addDirectedEdge(7, 8);
	g.addDirectedEdge(8, 6);
	cout << "Topological Sort of 1st Graph: " << endl;
	g.printTopologicalSort(); 

	AGraph g2(6);
	g2.addDirectedEdge(1, 2);
	g2.addDirectedEdge(1, 6);
	g2.addDirectedEdge(2, 3);
	g2.addDirectedEdge(3, 4);
	g2.addDirectedEdge(4, 5);
	g2.addDirectedEdge(5, 1);
	g2.addDirectedEdge(5, 6);
	cout << "Topological Sort of 2nd Graph: " << endl;
	g2.printTopologicalSort();

	system("pause");
	return 0;
}