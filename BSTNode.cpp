template <typename E> class BinNode{
public:
	virtual ~BinNode() {}   
	// Base destructor
	// Return the node¡¯s value
	virtual E& element() = 0;
	// Set the node¡¯s value
	virtual void setElement(const E&) = 0;
	// Return the node¡¯s left child
	virtual BinNode* left() const = 0;
	// Set the node¡¯s left child
	virtual void setLeft(BinNode*) = 0;
	// Return the node¡¯s right child
	virtual BinNode* right() const = 0;
	// Set the node¡¯s right child
	virtual void setRight(BinNode*) = 0;
	// Return true if the node is a leaf, false otherwise
	virtual boolisLeaf() = 0;
};
template <typename E>
void preorder(BinNode<E>* root) {
	if (root == NULL) return;   
	// Empty subtree, do nothing
	visit(root);                
	// Perform desired action
	preorder(root->left());
	preorder(root->right());
}
template <typenameE>
void preorder2(BinNode<E>* root) {
	visit(root); 
	// Perform whatever action is desired
	if (root->left() != NULL) preorder2(root->left());
	if (root->right() != NULL) preorder2(root->right());
}

