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
    ~BST() { clearhelp(root); root=NULL; nodecount=0;}

    void insert(const Key &k, const E &e)
    {
        root = inserthelp(root, k, e);
        nodecount++;
    }

    E remove(const Key &k)
    {
        E temp = findhelp(root, k); // First find it
        if (temp != NULL)
        {
            root = removehelp(root, k);
            nodecount--;
        }
        return temp;
    }

	E removeAny() {
		if(root!=NULL){
			E temp=root->element();
			root=removehelp(root,root->key());
			nodecount--;
			return temp;
		}
		else return NULL;
	}

    E find(const Key &k) const { return findhelp(root, k); }

    void select(int (*hit)(string, string), const string &k)
    {
        selecthelp(root, hit, k);
    }

    int size() { return nodecount; }

    void print() const
    {
        if (root == NULL)
            cout << "The BST is empty.\n";
        else
            printhelp(root, 0);
    }
};

