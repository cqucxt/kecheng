#include <iostream>
#include<cassert>
#define defaultSize 14
#define Assert(a,b) assert((a)&&(b))
using namespace std;

//**************定义队列
template<typename E>
class Queue{
private:
    void operator=(const Queue&) {}
    Queue(const Queue&) {}  //拷贝构造函数
public:
    Queue() {}
    virtual ~Queue() {}
    virtual void clear() = 0;
    virtual void enqueue(const E&) = 0;
    virtual E dequeue() = 0;
    virtual const E& frontValue() = 0;
    virtual int length() = 0;
};

template<typename E>
class AQueue: public Queue<E> {
private:
    int maxSize;
    int front;
    int rear;
    E *listArray;
public:
    AQueue(int size=defaultSize) {
        maxSize=size+1;
        rear=0;
        front=1;
        listArray=new E[maxSize];
    }
    ~AQueue() {
        delete [] listArray;
    }
    void clear() {
        rear=0;
        front=1;
    }
    void enqueue(const E& it) { //尾部入队列
        Assert(((rear+2)%maxSize)!=front,"Queue is full");
        rear=(rear+1)%maxSize;
        listArray[rear]=it;
    }
    E dequeue() {   //头部出队列
        Assert(length()!=0,"Queue is empty");
        E it=listArray[front];
        front=(front+1)%maxSize;
        return it;
    }
    const E& frontValue()  {
        Assert(length()!=0,"Queue is empty");
        return listArray[front];
    }
    const E& rearValue()  {
        Assert(length()!=0,"Queue is empty");
        return listArray[rear];
    }
    virtual int length() {
        return ((rear+maxSize)-front+1)%maxSize;
    }
};

//**************定义栈
template<typename E>
class Stack {
private:
    void operator=(const Stack&) {}
    Stack(const Stack&) {}
public:
    Stack() {}
    virtual ~Stack() {}
    virtual void clear() = 0;
    virtual void push(const E& it) = 0;
    virtual E pop() = 0;
    virtual const E& topValue() = 0;
    virtual int length() = 0;
};

template<typename E>
class QStack:public Stack<E>{
private:
    int maxSize;    //栈的容量
    //基于数组实现的队列
    AQueue<E> QA;
    AQueue<E> QB;
public:
    QStack(int size=defaultSize):QA(size),QB(size) {
        maxSize=size;
    }
    ~QStack() {}
    void clear() {
        QA.clear();
        QB.clear();
    }
    //两个队列互相转换除队尾的全部元素，队尾元素相当于栈顶，对栈顶进行操作
    void push(const E& it) {
        Assert(QA.length()<maxSize,"QStack is full");
        QA.enqueue(it);
    }
    E pop() {
        Assert(QA.length()>0,"QStack is empty");
		while(QA.length()>1) {
            QB.enqueue(QA.dequeue());
        }
        E it =QA.dequeue();
        while(QB.length()>=1) {
            QA.enqueue(QB.dequeue());
        }
            return it;
        
    }
    //哪个队列长，所有元素就在哪个队列里面
    const E& topValue() {
        return QA.rearValue();
    }
    virtual int length() {
        return QA.length();
    }
    void validate(int N) {   //验证出栈顺序是否有效
        AQueue<E> Qtmp(N);
        int val;
        //读取待测序列
        for(int i=1;i<=N;i++) {
            cin>>val;
            Qtmp.enqueue(val);
        }
        int odnr=1; //记录原顺序1，2，……，N
        while(odnr<=N) {
            //元素压入栈
            while(Qtmp.frontValue()>=odnr) {
                if(length()<maxSize) {  //保证不溢出栈
                    push(odnr);
                    odnr++;
                } else {
                    if(length()==maxSize) {
                        if(odnr==Qtmp.frontValue()) {
                            push(odnr);
                            odnr++;
                            break;
                        } else {    //若栈压入的最后一位（栈顶）没办法和队首元素相等，该顺序必然错误·
                            cout<<'F'<<endl;
                            return;
                        }
                    }
                }
            }
            //序列比较
            for(int i=1;i<=length();i++){
                if(topValue()==Qtmp.frontValue()){  //当栈首位等于队列首位，将两者都移出
                    pop();
                    Qtmp.dequeue();
                }
            }
        }
        //倘若能执行完整个循环（对1~N），则此序列正确
        cout<<'T'<<endl;
        return;
        }
};

int main()
{
    int N;  //入栈的最大整数
    int K;  //栈存储整数的数量上限
    int m;  //共有m行序列需要检测

    while(cin>>N>>K>>m){
        QStack<int> q(K);
        int i=1;
        while(i<=m) {
            q.validate(N);
            i++;
        }
    }
}

