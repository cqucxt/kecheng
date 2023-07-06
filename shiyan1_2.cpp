#include <iostream>
using namespace std;

//Double linked list link node with freelist support
template <typename E>
class Link
{
private:
    static Link<E> *freelist; //Reference to freelist head

public:
    E element1; //ϵ���ʹ���
    E element2;
    Link *next; //Pointer to next node in list
    Link *prev; //Pointer to previous node

    //Constructors
    Link(const E &e1, const E &e2, Link *prevp = NULL, Link *nextp = NULL)
    {
        element1 = e1;
        element2 = e2;
        prev = prevp;
        next = nextp;
    }
    Link(Link *prevp = NULL, Link *nextp = NULL)
    {
        prev = prevp;
        next = nextp;
        element1=NULL;
        element2=NULL;
    }

    // Destructor
    ~Link() {}

    void *operator new(size_t) //Overloaded new operator
    {
        if (freelist == NULL)
            return ::new Link;    //Create space
        Link<E> *temp = freelist; //Can take from freelist
        freelist = freelist->next;
        return temp; //Return the link
    }

    //Overloaded delete operator
    void operator delete(void *ptr)
    {
        ((Link<E> *)ptr)->next = freelist; //Put on freelist
        freelist = (Link<E> *)ptr;
    }
};

template <typename E>
Link<E> *Link<E>::freelist = NULL;  // ��freelist��Ϊ��

template <typename E>
class DoubleList
{
public:
    Link<E> *head; //Pointer to list header
    Link<E> *tail; //pointer to last element
    Link<E> *curr; //Access to current element

    void init() //Intialization helper method
    {
        head = new Link<E>;
        curr = head;
        tail = new Link<E>;
        head->next = tail;
        tail->prev = head;
        curr = head;
    }

public:
    DoubleList() { init(); } //Constructor
    ~DoubleList() {}         //Destructor

    //��պ�����������������ʾ���е�removeall����
    void removeall() //Return link nodes to free store
    {
        while (head->next != tail)
        {
            Link<E> *temp = head->next;
            head->next = head->next->next;
            head->next->prev = head;
            delete temp;
        }
    }

    void append(const E &xishu, const E &zhishu) //Append to list
    {
        tail->prev = tail->prev->next = new Link<E>(xishu, zhishu, tail->prev, tail);
    }

    //�õ���������Ч��ĸ����ĺ���
    int getLength() 
    {
        int xiangshu = 0;
        for (curr = head->next; curr != NULL; curr = curr->next)
        {
            if ((curr->element1) != 0)
                xiangshu++;
        }
        return xiangshu;
    }

    // ���������
    void operator=(DoubleList &d)
    {
        this->removeall();
        d.curr = d.head->next;
        while (d.curr != d.tail)
        {
            this->append(d.curr->element1, d.curr->element2);
            d.curr = d.curr->next;
        }
        d.curr = d.head->next;
    }

    // ��������أ�����main�������
    friend ostream &operator<<(ostream &os, DoubleList d)
    {
        int xiangshu=0;
        for (d.curr = d.head->next; d.curr != NULL; d.curr = d.curr->next)
        {
            //�����������ж�ϵ���Ƿ�Ϊ0������Ϊ�㼴Ϊ��Ч�������+1����
            if ((d.curr->element1) != 0)
                xiangshu++;
        }
        
        //�������
        os << xiangshu << endl;
        
        //��ʽ�����ϵ���ʹ���
        for (d.curr = d.head->next; d.curr->next != NULL; d.curr = d.curr->next)
        {
            os << d.curr->element1 << ' ' << d.curr->element2 << endl;
        }
        return os;
    }
};

void add(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &temp);
void multiple(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &multiResult);

int main()
{
    DoubleList<int> d1, d2, multipleResult, addResult;
    int firstXiangShu, secondXiangShu;
    cin >> firstXiangShu >> secondXiangShu;
    for (int i = 0; i < firstXiangShu; i++)
    {
        int xishu, zhishu;
        cin >> xishu >> zhishu;
        d1.append(xishu, zhishu);
    }
    for (int i = 0; i < secondXiangShu; i++)
    {
        int xishu, zhishu;
        cin >> xishu >> zhishu;
        d2.append(xishu, zhishu);
    }

    add(d1, d2, addResult);
    cout << addResult;
    multiple(d1, d2, multipleResult);
    cout << multipleResult;
    return 0;
}

//��Ӻ���
void add(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &temp)
{
    DoubleList<int> result;

    d1.curr = d1.head->next; //��currָ�붼ָ���һ��
    d2.curr = d2.head->next;
    while (1)
    {
        if (d1.curr->element2 > d2.curr->element2) //����d1�ĵ�ǰ���ָ������d2���Ͱ�d1�ĵ�ǰ�����result��
        {
            result.append(d1.curr->element1, d1.curr->element2);
            d1.curr = d1.curr->next; //currָ�������
        }
        else
        {
            if (d1.curr->element2 < d2.curr->element2) //����d1�ĵ�ǰ���ָ��С��d2���Ͱ�d2�ĵ�ǰ�����result��
            {
                result.append(d2.curr->element1, d2.curr->element2);
                d2.curr = d2.curr->next; //currָ�������
            }
            else
            {
                // ������������ȣ����Ҳ���λ������β�����Ϳ���ֱ�ӽ�ϵ����Ӳ�����ӵ�result�����С�
                if ((d1.curr->element2 == d2.curr->element2) && (d1.curr != d1.tail && d2.curr != d2.tail)) 
                {
                    result.append(d1.curr->element1 + d2.curr->element1, d2.curr->element2);
                    d1.curr = d1.curr->next;
                    d2.curr = d2.curr->next;
                }
                else
                {
                    //���d1�Ѿ����ڶ�β����ֱ�����d2����result����
                    if (d1.curr == d1.tail)
                    {
                        result.append(d2.curr->element1, d2.curr->element2);
                        d2.curr = d2.curr->next;
                    }
                    else
                    {
                        // ��d2���ڶ�βʱͬ��
                        if (d2.curr == d2.tail)
                        {
                            result.append(d1.curr->element1, d1.curr->element2);
                            d1.curr = d1.curr->next;
                        }
                    }
                }
            }
        }

        //�����߶����ڶ�βʱ��������ɣ������˳�whileѭ��
        if ((d1.curr == d1.tail) && (d2.curr == d2.tail))
            break;
    }

    d1.curr = d1.head;
    d2.curr = d2.head;
    temp = result;
}

//��˺���
//����������ʱ���Դ�������������˿���ֱ�ӽ����������������ˣ�������Ҫ�����ٴ�����
void multiple(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &multiResult)
{
    int xiangshu_d1 = d1.getLength();
    int xiangshu_d2 = d2.getLength();

    d1.curr = d1.head->next; //��currָ�붼ָ���һ��
    d2.curr = d2.head->next;

    //����forѭ��������ϵ����˺ʹ������
    for (int i = 0; i < xiangshu_d1; i++)
    {
        DoubleList<int> temp;
        int xishu_d1 = d1.curr->element1;
        int zhishu_d1 = d1.curr->element2;

        d2.curr = d2.head->next;

        for (int j = 0; j < xiangshu_d2; j++)
        {
            int xishu_d2 = d2.curr->element1;
            int zhishu_d2 = d2.curr->element2;

            temp.append(xishu_d1 * xishu_d2, zhishu_d1 + zhishu_d2);
            d2.curr = d2.curr->next;
        }
        
        //ʹ��add������������ͬ����ϲ�
        add(temp, multiResult, multiResult);
        d1.curr = d1.curr->next;
    }
}

