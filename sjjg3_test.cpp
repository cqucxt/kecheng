template <typename Key, typename E>
class BSTNode
{
private:
    Key k;       /**< �ڵ��key */
    E it;        /**< �ڵ����� */
    BSTNode *lc; /**< �ڵ������ */
    BSTNode *rc; /**< �ڵ���Һ��� */

public:
    /** ȱʡ���캯�� */
    BSTNode() { lc = rc = NULL; }

    /** ����ʼֵ�Ĺ��캯��
    *
    *     @param K �ڵ��key
    *     @param e �ڵ�����
    *     @param l (��ȱ)�ڵ������
    *     @param r (��ȱ)�ڵ���Һ���
    */
    BSTNode(Key K, E e, BSTNode *l = NULL, BSTNode *r = NULL)
    {
        k = K;
        it = e;
        lc = l;
        rc = r;
    }

    /** ȱʡ�������� */
    ~BSTNode() = default; // Destructor

    /** ��-���� */
    E &element() { return it; }

    /** ��-���� */
    void setElement(const E &e) { it = e; }

    /** ��-��ֵ */
    Key key() { return k; }

    /** ��-��ֵ */
    void setKey(Key K) { k = K; }

    /** ��-���� */
    inline BSTNode *left() const { return lc; }

    /** ��-���� */
    void setLeft(BSTNode<Key, E> *b) { lc = (BSTNode *)b; }

    /** ��-�Һ��� */
    inline BSTNode *right() const { return rc; }

    /** ��-�Һ��� */
    void setRight(BSTNode<Key, E> *b) { rc = (BSTNode *)b; }

    /** �Ƿ�ΪҶ�� */
    bool isLeaf() { return (lc == nullptr) && (rc == nullptr); }
};

/**
 * @brief ����������
 */
template <typename Key, typename E>
class BST
{
private:
    BSTNode<Key, E> *root; /**< �����������ĸ��ڵ� */
    int nodecount;         /**< �����������Ľڵ��� */

    void clearhelp(BSTNode<Key, E> *root)
    {
        if (root == NULL)
            return;
        clearhelp(root->left());
        clearhelp(root->right());
        delete root;
    };

    BSTNode<Key, E> *inserthelp(BSTNode<Key, E> *root, const Key &k, const E &it)
    {
        if (root == NULL) // Empty tree: create node
            return new BSTNode<Key, E>(k, it, NULL, NULL);
        if (k < root->key())
            root->setLeft(inserthelp(root->left(), k, it));
        else
            root->setRight(inserthelp(root->right(), k, it));
        return root; // Return tree with node inserted
    }

    BSTNode<Key, E> *getmin(BSTNode<Key, E> *rt)
    {
        if (rt->left() == NULL)
            return rt;
        else
            return getmin(rt->left());
    }

    BSTNode<Key, E> *deletemin(BSTNode<Key, E> *rt)
    {
        if (rt->left() == NULL) // Found min
            return rt->right();
        else
        { // Continue left
            rt->setLeft(deletemin(rt->left()));
            return rt;
        }
    }

    BSTNode<Key, E> *removehelp(BSTNode<Key, E> *rt, const Key &k)
    {
        if (rt == NULL)
            return NULL; // k is not in tree
        else if (k < rt->key())
            rt->setLeft(removehelp(rt->left(), k));
        else if (k > rt->key())
            rt->setRight(removehelp(rt->right(), k));
        else
        { // Found: remove it
            BSTNode<Key, E> *temp = rt;
            if (rt->left() == NULL)
            {                     // Only a right child
                rt = rt->right(); //  so point to right
                delete temp;
            }
            else if (rt->right() == NULL)
            {                    // Only a left child
                rt = rt->left(); //  so point to left
                delete temp;
            }
            else
            { // Both children are non-empty
                BSTNode<Key, E> *temp = getmin(rt->right());
                rt->setElement(temp->element());
                rt->setKey(temp->key());
                rt->setRight(deletemin(rt->right()));
                delete temp;
            }
        }
        return rt;
    };

    E findhelp(BSTNode<Key, E> *root, const Key &k)
    {
        if (root == NULL)
            return NULL; // Empty tree
        if (k < root->key())
            return findhelp(root->left(), k); // Check left
        else if (k > root->key())
            return findhelp(root->right(), k); // Check right
        else
            return root->element(); // Found it
    };

    void printhelp(BSTNode<Key, E> *root, int level) const
    {
        if (root == NULL)
            return;                         // Empty tree
        printhelp(root->left(), level + 1); // Do left subtree
        //for (int i = 0; i < level; i++)     // Indent to level
        //    cout << "  ";
        cout << root->key() << "\n";         // Print node value
        printhelp(root->right(), level + 1); // Do right subtree
    };

public:
    /** �������� */
    BST()
    {
        root = NULL;
        nodecount = 0;
    }

    /** ȫ������ */
    ~BST() { clearhelp(root); }

    /** ȫ������ */
    void clear() // Reinitialize tree
    {
        clearhelp(root);
        root = nullptr;
        nodecount = 0;
    }

    void insert(const Key &k, const E &e)
    {
        root = inserthelp(root, k, e);
        nodecount++;
    }

    E remove(const Key &k)
    {
        E temp = findhelp(root, k); // First find it
        if (temp != nullptr)
        {
            root = removehelp(root, k);
            nodecount--;
        }
        return temp;
    }

    E find(const Key &k) const { return findhelp(root, k); }

    void select(int (*hit)(string, string), const string &k)
    {
        selecthelp(root, hit, k);
    }

    void select(int (*hit)(E, E, int), const E &e, const int a)
    {
        selecthelp(root, hit, e, a);
    }

    /** ��-�ڵ���    */
    int size() { return nodecount; }

    /** ��ӡ���һ����*/
    void print() const
    {
        if (root == nullptr)
            cout << "The BST is empty.\n";
        else
            printhelp(root, 0);
    }

    /** ƽ����*/
    void balance()
    {
        BSTNode<Key, E> *first = root, *second = 0;
        if (first == 0)
            return;
        while (first != 0)
        {
            if (first->left())
                rightRotateHelp(first, second);
            else
            {
                second = first;
                first = first->right();
            }
        }
        first = root;
        int n = 0;
        while (first != 0)
        {
            n++;
            first = first->right();
        }
        first = root;
        second = 0;
        int m = static_cast<int>(pow(2, (int)(floor(log(n) / log(2)))) - 1);
        for (int i = 0; i < n - m; i++)
        {
            leftRotateHelp(first, second);
            second = first;
            first = first->right();
        }
        while (m > 1)
        {
            m = m / 2;
            int i = m;
            first = root;
            second = 0;
            while (i != 0)
            {
                leftRotateHelp(first, second);
                second = first;
                first = first->right();
                i--;
            }
        }
    }
};

