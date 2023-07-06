template <typename Key, typename E>
class BSTNode{
private:
	Key k;    // The node¡¯s key
	E it;        // The node¡¯s value
	BSTNode* lc;    // Pointer to left child
	BSTNode* rc;    // Pointer to right child
public:
// Two constructors --with and without initial value
	BSTNode() { lc= rc= NULL; }
	BSTNode(Key K, E e, BSTNode* l =NULL, BSTNode* r =NULL)
	{ k = K; it = e; lc= l; rc= r; } 
	~BSTNode() {} // Destructor
	E& element() { return it; }
	void setElement(const E& e) { it = e; }
	Key& key() { return k; }
	void setKey(const Key& K) { k = K; }
	// Functions to set and return the children
	inline BSTNode* left() const { return lc; }
	void setLeft(BinNode<E>* b) { lc= (BSTNode*) b; }
	inline BSTNode* right() const { return rc; }
	void setRight(BinNode<E>* b) { rc= (BSTNode*) b; }
	// Return true if it is a leaf, false otherwise
	bool isLeaf() { return (lc== NULL) && (rc== NULL); }
};

