template <typename Key, typename E>
class BSTNode
{
private:
    Key k;       /**< 节点的key */
    E it;        /**< 节点数据 */
    BSTNode *lc; /**< 节点的左孩子 */
    BSTNode *rc; /**< 节点的右孩子 */

public:
    /** 缺省构造函数 */
    BSTNode() { lc = rc = NULL; }

    /** 带初始值的构造函数
    *
    *     @param K 节点的key
    *     @param e 节点数据
    *     @param l (可缺)节点的左孩子
    *     @param r (可缺)节点的右孩子
    */
    BSTNode(Key K, E e, BSTNode *l = NULL, BSTNode *r = NULL)
    {
        k = K;
        it = e;
        lc = l;
        rc = r;
    }

    /** 缺省析构函数 */
    ~BSTNode() = default; // Destructor

    /** 查-数据 */
    E &element() { return it; }

    /** 改-数据 */
    void setElement(const E &e) { it = e; }

    /** 查-键值 */
    Key key() { return k; }

    /** 改-键值 */
    void setKey(Key K) { k = K; }

    /** 查-左孩子 */
    inline BSTNode *left() const { return lc; }

    /** 改-左孩子 */
    void setLeft(BSTNode<Key, E> *b) { lc = (BSTNode *)b; }

    /** 查-右孩子 */
    inline BSTNode *right() const { return rc; }

    /** 改-右孩子 */
    void setRight(BSTNode<Key, E> *b) { rc = (BSTNode *)b; }

    /** 是否为叶子 */
    bool isLeaf() { return (lc == nullptr) && (rc == nullptr); }
};

/**
 * @brief 二叉搜索树
 */
template <typename Key, typename E>
class BST
{
private:
    BSTNode<Key, E> *root; /**< 二叉搜索树的根节点 */
    int nodecount;         /**< 二叉搜索树的节点数 */

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
    /** 空树构造 */
    BST()
    {
        root = NULL;
        nodecount = 0;
    }

    /** 全树析构 */
    ~BST() { clearhelp(root); }

    /** 全树重置 */
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

    /** 查-节点数    */
    int size() { return nodecount; }

    /** 打印输出一棵树*/
    void print() const
    {
        if (root == nullptr)
            cout << "The BST is empty.\n";
        else
            printhelp(root, 0);
    }

    /** 平衡树*/
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

