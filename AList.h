template <typename E> class List { // List ADT?
private://防止该类被意外复制?
	void operator =(const List&) {}      // 赋值运算符重载?
	List(const List&) {}           // 类复制构造?
public:
	List() {}          // Default constructor?
	virtual ~List() {} // Base destructor?
	// Clear contents from the list, to make it empty.?
	virtual void clear() = 0;
	// Insert an element at the current location.?
	// item: The element to be inserted?
	virtual void insert(const E& item) = 0; 
	virtual void append(const E& item) = 0;
	// Remove and return the current element.?
	// Return: the element that was removed.?
	virtual E remove() = 0;
	// Set the current position to the start of the list?
	virtual void moveToStart() = 0;// Set the current position to the end of the list?
	virtual void moveToEnd() = 0; 
	virtual void prev() = 0;
	// Move the current position one step right. No changeif already at end.
	virtual void next() = 0;
	// Return: The number of elements in the list.?
	virtual int length() const = 0;
	// Return: The position of the current element.?
	virtual int currPos() const = 0;
	// Set current position. pos: The position to make current.?
	virtual void moveToPos(int pos) = 0;
	// Return: The current element.?
	virtual const E& getValue() const = 0;
};

template <typename E> 
// Array-based list implementation?
class AList: public List<E> {
private:
	int maxSize;        // Maximum size of list?
	int listSize;          // Number of list items now?
	int curr;              // Position of current element?
	E* listArray;        // Array holding list elements
public:
	AList(int size=defaultSize) { // Constructor?
		maxSize = size;
		listSize = curr = 0;  //empty?
		listArray = new E[maxSize];
	}
	~AList() { delete [] listArray; } // Destructor
	void clear() { 
// Reinitialize the list?
		delete [] listArray;            
		// Remove the array?
		listSize = curr = 0;            
		// Reset the size?
		listArray = new E[maxSize];  // Recreate array?
		}
	void moveToStart() { curr = 0; }
	void moveToEnd() { curr = listSize; }
	void prev() { if (curr != 0) curr--; }
	void next() { if (curr < listSize) curr++; }
	int length() const  { return listSize; }
	// Return current position?
	int currPos() const { return curr; }
	// Set current list position to "pos"?
	void moveToPos(int pos) {
		Assert ((pos>=0)&&(pos<=listSize), "Pos out of range");
		curr = pos;  
	}
	const E& getValue() const 
	{	
		Assert((curr>=0)&&(curr<listSize),"No current element");
		return listArray[curr];
	}
	void insert(const E& it){
		Assert(listSize < maxSize, "List capacity exceeded");
		for(int i=listSize; i>curr; i--)  // Shift elements up?
			listArray[i] = listArray[i-1];  //   to make room?
		listArray[curr] = it;
		listSize++;        
	}
	void append(const E& it){ // Append "it"?
		Assert(listSize < maxSize, "List capacity exceeded");
		listArray[listSize++] = it;
	}
	E remove() {
		Assert((curr>=0) && (curr < listSize), "No element");
		E it = listArray[curr];           // Copy the element?
		for(int i=curr; i<listSize-1; i++)  // Shift them down?
			listArray[i] = listArray[i+1];
		listSize--;   // Decrement size?
		return it;
	}





