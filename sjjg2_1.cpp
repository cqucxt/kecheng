#include <iostream>
#include<cassert>
#define defaultSize 14
#define Assert(a,b) assert((a)&&(b))
using namespace std;

//**************�������
template<typename E>
class Queue{
private:
    void operator=(const Queue&) {}
    Queue(const Queue&) {}  //�������캯��
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
    void enqueue(const E& it) { //β�������
        Assert(((rear+2)%maxSize)!=front,"Queue is full");
        rear=(rear+1)%maxSize;
        listArray[rear]=it;
    }
    E dequeue() {   //ͷ��������
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

//**************����ջ
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
    int maxSize;    //ջ������
    //��������ʵ�ֵĶ���
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
    //�������л���ת������β��ȫ��Ԫ�أ���βԪ���൱��ջ������ջ�����в���
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
    //�ĸ����г�������Ԫ�ؾ����ĸ���������
    const E& topValue() {
        return QA.rearValue();
    }
    virtual int length() {
        return QA.length();
    }
    void validate(int N) {   //��֤��ջ˳���Ƿ���Ч
        AQueue<E> Qtmp(N);
        int val;
        //��ȡ��������
        for(int i=1;i<=N;i++) {
            cin>>val;
            Qtmp.enqueue(val);
        }
        int odnr=1; //��¼ԭ˳��1��2��������N
        while(odnr<=N) {
            //Ԫ��ѹ��ջ
            while(Qtmp.frontValue()>=odnr) {
                if(length()<maxSize) {  //��֤�����ջ
                    push(odnr);
                    odnr++;
                } else {
                    if(length()==maxSize) {
                        if(odnr==Qtmp.frontValue()) {
                            push(odnr);
                            odnr++;
                            break;
                        } else {    //��ջѹ������һλ��ջ����û�취�Ͷ���Ԫ����ȣ���˳���Ȼ����
                            cout<<'F'<<endl;
                            return;
                        }
                    }
                }
            }
            //���бȽ�
            for(int i=1;i<=length();i++){
                if(topValue()==Qtmp.frontValue()){  //��ջ��λ���ڶ�����λ�������߶��Ƴ�
                    pop();
                    Qtmp.dequeue();
                }
            }
        }
        //������ִ��������ѭ������1~N�������������ȷ
        cout<<'T'<<endl;
        return;
        }
};

int main()
{
    int N;  //��ջ���������
    int K;  //ջ�洢��������������
    int m;  //����m��������Ҫ���

    while(cin>>N>>K>>m){
        QStack<int> q(K);
        int i=1;
        while(i<=m) {
            q.validate(N);
            i++;
        }
    }
}

