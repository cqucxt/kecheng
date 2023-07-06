#include <iostream>
#include<cassert>
#define defaultSize 14
#define Assert(a,b) assert((a)&&(b))
using namespace std;
template <typename E> class Stack{
private:       //防止该类被意外复制
	void operator =(const Stack&) {}     // 赋值运算符重载
	Stack(const Stack&) {}     // 类复制构造函数
public:
	Stack() {} // Default constructor
	virtual ~Stack() {} // Base destructor
	// Reinitialize the stack. The user is responsible for
	// reclaiming(回收) the storage used by the stack elements.
	virtual void clear() = 0;
	// Push an element onto the top of the stack.
	// it: The element being pushed onto the stack.
	virtual void push(const E& it) = 0;// Remove the element at the top of the stack.
	// Return: The element at the top of the stack.
	virtual E pop() = 0;
	// Return: A copy of the top element.
	virtual const E& topValue() const = 0;
	// Return: The number of elements in the stack.
	virtual intlength() const ;
};

template <typename E> class Queue{
private:       //防止该类被意外复制
	void operator =(const Queue&) {}    // 赋值运算符重载
	Queue(const Queue&) {}     // 类复制构造
public:
	Queue() {} // Default
	virtual ~Queue() {} // Base destructor
	// Reinitialize the queue. The user is responsible for
	// reclaiming the storage used by the queue elements.
	virtual void clear() = 0;// Place an element at the rear of the queue.
	// it: The element being enqueued.
	virtual void enqueue(const E& it) = 0;// Remove and return element at the front of the queue.
	// Return: The element at the front of the queue.
	virtual E dequeue() = 0;// Return: A copy of the front element.
	virtual const E& frontValue() const = 0;// Return: The number of elements in the queue.
	virtual int length() const = 0;
};

template <typename E> class AQueue: public Queue<E> {
private:
	int maxSize;             // Maximum size of queue
	int front;// Index of front element
	int rear;     // Index of rear element
	E *listArray;    // Array holding queue elements
public:
	
	AQueue(int size =0) {      // Constructor
		maxSize= size+1;// Make list array one position larger for empty slot
		rear = 0; 
		front = 1;
		listArray= new E[maxSize];
	} 
	~AQueue() { delete [] listArray; } 
	void clear() { rear = 0; front = 1; }           // Reinitialize
	void enqueue(const E& it) { // Put "it" in queue
		Assert(((rear+2) % maxSize) != front, "Queue is full");
		rear = (rear+1) % maxSize;  // Circular increment
		listArray[rear] = it;
	}
	E dequeue() {  // Take element out
		Assert(length() != 0, "Queue is empty");
		E it = listArray[front];
		front = (front+1) % maxSize;   // Circular increment
		return it;
	} 
	const E& frontValue() const{// Get front value
		Assert(length() != 0, "Queue is empty");
		return listArray[front];
	}
	virtual int length() const// Return length
	{ return ((rear+maxSize) -front + 1) % maxSize; }
	
};



template <typename E> class QStack : public Stack<E>{
private:
    int maxSize; //栈的容量
	AQueue<E> QA;
	AQueue<E> QB; //基于数组实现的队列
public:
    QStack(int size = defaultSize): QA(size), QB(size) //初始化队列
    {
       maxSize = size;
    }
    ~QStack() { }
              //完成下列函数的代码
    void clear(){
		QA.clear();
		QB.clear();	
	}
    void push(const E& it) {
    	Assert(QA.length()<maxSize, "QStack is full");
		QA.enqueue(it);
	}
    E pop() {
    	Assert(QA.length() != 0, "QStack is empty");
		while(QA.length()>1) {
            QB.enqueue(QA.dequeue());
        }
		E it2=QA.dequeue();
		while(QB.length()>0) {
            QA.enqueue(QB.dequeue());
        }
		return it2;
	}
    const E& topValue() const {
		Assert(QA.length() != 0, "QStack is empty");
		E *it;
		E *it2;
		for(int i=1;i<QA.length();i++){
			it=QA.dequeue();
			QB.enqueue(it);	
		}
		it2=QB.frontValue();
		for(int i=1;i<QB.length();i++){
			it=QB.dequeue();
			QA.enqueue(it);	
		}
		return it2;
	}
    virtual int length() const {
		return QA.length();
	}
};

int main(){
	int N, K ,m;
	cin>>N>>K>>m;
	QStack<int> qs(K);
	AQueue<int> aq(K);
	for(int i = 0; i<m;i++){
		for(int j=0;j<N;j++){
			int ig;
			cin>>ig;
			aq.enqueue(ig);
		}
		int temp=1;
		while(temp<=N){
			if(qs.length()==0) qs.push(temp++);
			if(qs.topValue()==aq.frontValue()){
				qs.pop(); aq.dequeue(); 
			}else{
				if(qs.length()<K){
					qs.push(temp++);
				}else{
					break;
				}
			}
		}
		if(aq.length()==0) printf("T\n");
		else printf("F\n"); 
	}
}

