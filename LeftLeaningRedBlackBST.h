/*
Authors:
		Ali Rahimi-Baigi
		Sayed Kamaledin Ghiasi-Shirazi
		Taraneh Ghandi
		Ali Taghizadeh		
*/

#include <iostream>

using namespace std;


template<class Key, class Value>
class LeftLeaningRedBlackBST : public Map<Key,Value> {
public:
    enum COLOR {
        RED, BLACK
    };
private:
    class Node {
    public:
        Key key;                  // key
        Value value;              // associated data
        Node *left, *right;         // left and right subtrees
        bool color;            // color of parent link

    public:
		// This constructor is used only for nil
		Node() {
			this->color = BLACK;
			left = right = this;
		}

        Node(Key key, Value value, Node* nil) {
            this->key = key;
            this->value = value;
            this->color = RED;
            left = right = nil;
        }
    };

	Node *nil;
    Node *root;            // root of the BST

public:
	LeftLeaningRedBlackBST()
	{
		nil = new Node();
		root = nil;
	}

public:
    inline Node *getRoot() {
        return root;
    }

public:
    inline bool contains(Key key) {
        return (get(key) != nil);
    }

public:
    inline Value get(Key key) {
        return get(root, key);
    }

	virtual int size() {
		return recSize(root);
	}

	int recSize(Node* h) {
		if (h == nil)
			return 0;
		else
			return 1 + recSize(h->left) + recSize(h->right);
	}


private:
    inline Value get(Node *x, Key key) {
		//if (x == nil)
		//	throw "Not found";
        if (key== x->key) return x->value;
        if (key < x->key) return get(x->left, key);
        else return get(x->right, key);
    }

public:
    inline Value min() {
        if (root == nil) return nil;
        else return min(root);
    }


private:
    inline Value min(Node *x) {
        if (x->left == nil) return x->value;
        else return min(x->left);
    }

public:
    inline Value max() {
        if (root == nil) return nil;
        else return max(root);
    }

private:
    inline Value max(Node *x) {
        if (x->right == nil) return x->value;
        else return max(x->right);
    }

public:
    inline void put(Key key, Value value) {
        root = put(root, key, value);
        root->color = BLACK;
    }

private:
    inline Node *put(Node *h, Key key, Value value) {
        if (h == nil)
            return new Node(key, value, nil);

        if (key == h->key)
            h->value = value;
        else if (key < h->key)
            h->left = put(h->left, key, value);
        else
            h->right = put(h->right, key, value);


        if (isRed(h->right))
            h = rotateLeft(h);

        if (isRed(h->left) && isRed(h->left->left))
            h = rotateRight(h);

        if (isRed(h->left) && isRed(h->right))
            colorFlip(h);

        return h;
    }

public:
    void remove(Key key) {
        root = remove(root, key);
        if (root != nil) {
            root->color = BLACK;
        }
    }

private:
    Node *remove(Node *h, Key key) {
        if (h != nil) {
            if (key < h->key) {
                if (!isRed(h->left) && h->left != nil && !isRed(h->left->left))
                    h = moveRedLeft(h);
                h->left = remove(h->left, key);
            } else {
                if (isRed(h->left))
                    h = rotateRight(h);
                if ((key == h->key) && (h->right == nil)) {
                    delete h;
                    return nil;
                }
                if (!isRed(h->right) && h->right != nil && !isRed(h->right->left))
                    h = moveRedRight(h);
                if (key == h->key) {
                    h->value = get(h->right, min(h->right));
                    h->key = min(h->right);
                    h->right = deleteMin(h->right);
                } else h->right = remove(h->right, key);
            }

            return fixUp(h);
        }
        return nil;
    }

public:
    void deleteMin() {
        root = deleteMin(root);
        if (root != nil)
            root->color = BLACK;
    }

private:
    Node *deleteMin(Node *h) {
        if (h->left == nil) {
            delete h;
            return nil;
        }
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);

        h->left = deleteMin(h->left);

        return fixUp(h);
    }

public:
    void deleteMax() {
        root = deleteMax(root);

        if (root != nil)
            root->color = BLACK;
    }

private:
    Node deleteMax(Node *h) {
        if (isRed(h->left))
            h = rotateRight(h);

        if (h->right == nil) {
            delete h;
            return nil;
        }

        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);

        h->right = deleteMax(h->right);

        return fixUp(h);
    }


private:
	inline bool isRed(Node *x) {
        return x->color == RED;
    }

    inline void colorFlip(Node *h) {
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

	inline Node *rotateLeft(Node *h) {  // Make a right-leaning 3-node lean to the left.
		this->incLeftRotations();
        Node *x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = x->left->color;
        x->left->color = RED;
        return x;
    }

    inline Node *rotateRight(Node *h) {  // Make a left-leaning 3-node lean to the right.
		this->incRightRotations();
        Node *x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = x->right->color;
        x->right->color = RED;
        return x;
    }

    Node *moveRedLeft(Node *h) {  
		// Assuming that h is red and both h.left and h.left.left
        // are black, make h.left or one of its children red.
        colorFlip(h);
        if (h->right != nil && isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            colorFlip(h);
        }
        return h;
    }

	inline  Node *moveRedRight(Node *h) {  
		// Assuming that h is red and both h.right and h.right.left
        // are black, make h.right or one of its children red.
        colorFlip(h);

        if (h->left != nil && isRed(h->left->left)) {
            h = rotateRight(h);
            colorFlip(h);
        }
        return h;
    }

	inline  Node *fixUp(Node *h) {
        if (isRed(h->right))
            h = rotateLeft(h);

        if (isRed(h->left) && isRed(h->left->left))
            h = rotateRight(h);

        if (isRed(h->left) && isRed(h->right))
            colorFlip(h);

        return h;
    }
};