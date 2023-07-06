#include <iostream>
using namespace std;

//Double linked list link node with freelist support
template <typename E>
class Link
{
private:
    static Link<E> *freelist; //Reference to freelist head

public:
    E element1; //系数和次数
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
Link<E> *Link<E>::freelist = NULL;  // 将freelist置为空

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

    //清空函数，套用书上链表示例中的removeall函数
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

    //得到链表中有效项的个数的函数
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

    // 运算符重载
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

    // 运算符重载，方便main函数输出
    friend ostream &operator<<(ostream &os, DoubleList d)
    {
        int xiangshu=0;
        for (d.curr = d.head->next; d.curr != NULL; d.curr = d.curr->next)
        {
            //遍历过程中判断系数是否为0，若不为零即为有效项，项数做+1运算
            if ((d.curr->element1) != 0)
                xiangshu++;
        }
        
        //项数输出
        os << xiangshu << endl;
        
        //格式化输出系数和次数
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

//相加函数
void add(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &temp)
{
    DoubleList<int> result;

    d1.curr = d1.head->next; //把curr指针都指向第一项
    d2.curr = d2.head->next;
    while (1)
    {
        if (d1.curr->element2 > d2.curr->element2) //假如d1的当前项的指数大于d2，就把d1的当前项加入result中
        {
            result.append(d1.curr->element1, d1.curr->element2);
            d1.curr = d1.curr->next; //curr指针向后移
        }
        else
        {
            if (d1.curr->element2 < d2.curr->element2) //假如d1的当前项的指数小于d2，就把d2的当前项加入result中
            {
                result.append(d2.curr->element1, d2.curr->element2);
                d2.curr = d2.curr->next; //curr指针向后移
            }
            else
            {
                // 如果两项次数相等，并且不能位于链表尾部，就可以直接将系数相加并且添加到result链表中。
                if ((d1.curr->element2 == d2.curr->element2) && (d1.curr != d1.tail && d2.curr != d2.tail)) 
                {
                    result.append(d1.curr->element1 + d2.curr->element1, d2.curr->element2);
                    d1.curr = d1.curr->next;
                    d2.curr = d2.curr->next;
                }
                else
                {
                    //如果d1已经处于队尾，就直接添加d2进入result链表
                    if (d1.curr == d1.tail)
                    {
                        result.append(d2.curr->element1, d2.curr->element2);
                        d2.curr = d2.curr->next;
                    }
                    else
                    {
                        // 当d2处于队尾时同理
                        if (d2.curr == d2.tail)
                        {
                            result.append(d1.curr->element1, d1.curr->element2);
                            d1.curr = d1.curr->next;
                        }
                    }
                }
            }
        }

        //当两者都处于队尾时，遍历完成，即可退出while循环
        if ((d1.curr == d1.tail) && (d2.curr == d2.tail))
            break;
    }

    d1.curr = d1.head;
    d2.curr = d2.head;
    temp = result;
}

//相乘函数
//由于在输入时是以次数降序排序，因此可以直接进行两个链表遍历相乘，而不需要进行再次排序
void multiple(DoubleList<int> &d1, DoubleList<int> &d2, DoubleList<int> &multiResult)
{
    int xiangshu_d1 = d1.getLength();
    int xiangshu_d2 = d2.getLength();

    d1.curr = d1.head->next; //把curr指针都指向第一项
    d2.curr = d2.head->next;

    //两个for循环，进行系数相乘和次数相加
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
        
        //使用add函数将次数相同的项合并
        add(temp, multiResult, multiResult);
        d1.curr = d1.curr->next;
    }
}

