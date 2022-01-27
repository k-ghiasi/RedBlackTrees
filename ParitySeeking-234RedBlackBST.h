/*
Authros:
		Sayed Kamaledin Ghiasi-Shirazi
		Taraneh Ghandi
		Ali Taghizadeh
		Ali Rahimi-Baigi
*/

#pragma once
#include "Map.h"

template<class Key, class Value>
class ParitySeeking234RedBlackBST : public Map<Key, Value> {
private:
	enum COLOR {
		RED = 0, BLACK = 1
	};

	class Node {
	public:
		Key key;                // key
		Value value;            // associated data
		Node* left, *right;     // left and right subtrees
		Node* parent;
		enum COLOR color;				// color of parent link

	public:
		// This constructor is used only for nil
		Node() {
			this->color = BLACK;
			left = right = parent = this;
		}

		Node(Key key, Value value, Node* parent, Node* nil) {
			this->key = key;
			this->value = value;
			this->parent = parent;
			this->color = RED;
			left = right = nil;
		}
	};

public:
	ParitySeeking234RedBlackBST() {
		nil = new Node();
	}

	virtual Value get(Key key) {
		Node*	root = nil->left;
		return get(root, key);
	}

	virtual void put(Key key, Value value) {
		Node* h;
		h = put_top_down_pass(key, value);
		put_bottom_up_pass(h); // fixup pass
		Node*	root = nil->left;
		root->color = BLACK;
	}

	virtual void remove(Key key) {
		nil->key = key;
		Node* x; // Parent of deficient node
		x = remove_top_down_pass(key);
		if (x)
			remove_bottom_up_pass(x);
	}

	virtual int size() {
		return recSize(nil->left);
	}

	int recSize(Node* h) {
		if (h == nil)
			return 0;
		else
			return 1 + recSize(h->left) + recSize(h->right);
	}

private:
	Node*	nil;

private:
	inline Value get(Node* x, Key key) {
		//if (x == nil)
		//	throw "Not found";
		if (key == x->key) return x->value;
		if (key < x->key) return get(x->left, key);
		else return get(x->right, key);
	}

	Node* put_top_down_pass(Key key, Value value) {
		Node** p = &nil->left;	//root is nil->left
		Node* h = nil;
		while (1) {
			if (*p == nil) {
				*p = new Node(key, value, h, nil);
				break;
			}
			else {
				h = *p;
				if (key < h->key) {
					p = &h->left;
					continue;
				}
				else if (key > h->key) {
					p = &h->right;
					continue;
				}
				else { //if (key == h->key) 
					h->value = value;
					break;
				}
			}
		}
		return *p;
	}

	/*
	This method is taken from CLRS RB Trees
	*/
	void put_bottom_up_pass(Node* x) {
		Node* y;   // sibling of parent of x
		while (x->parent->color == RED) {
			if (x->parent == x->parent->parent->left) {
				y = x->parent->parent->right;
				if (y->color == RED) {
					colorFlip(x->parent->parent);
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						rotateLeft(x);
					}
					rotateRight(x->parent->parent);
				}
			}
			else
			{
				y = x->parent->parent->left;
				if (y->color == RED) {
					colorFlip(x->parent->parent);
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						rotateRight(x);
					}
					rotateLeft(x->parent->parent);
				}
			}
		}
	}

private:

	Node* remove_top_down_pass(Key key) {
		Node* deg2Node = NULL;
		Node* h = nil->left;

		while (1) {
			if (key > h->key) {
				if (deg2Node && isNil(h->right)) {
					deg2Node->key = h->key;
					deg2Node->value = h->value;
					h = performActualDeletion(h);
					break;
				}
				else {
					h = h->right;
					continue;
				}
			}
			else if (key < h->key) {
				h = h->left;
				continue;
			}
			else //if (key == h->key) 
			{
				if (isNil(h))	//key does not exist
					return NULL;
				if (isNil(h->left) || isNil(h->right)) {
					h = performActualDeletion(h);
					break;
				}
				else {
					deg2Node = h;
					h = h->left;
					continue;
				}
			}
		}
		return h;
	}

	void remove_bottom_up_pass(Node* x) {

		while (x != nil->left && x->color == BLACK)
			x = applyParitySeekingRules(x);
		x->color = BLACK; // Parity-seeking Rule (a)
	}

	Node* applyParitySeekingRules(Node* x) {
		Node* p = x->parent;
		Node* y = x == p->left? p->right:p->left;
		if (y->color == RED) 	
			// Parity-seeking Rule (c): Rotate to have a black sibling
			if (x == p->left)
				rotateLeft(p);
			else 
				rotateRight(p);
		else { 					
			// Parity-seeking Rule (b): Make the sibling red
			y->color = RED;
			x = p;
			x = fixUp(x, y);
		}
		return x;
	}

	Node* fixUp(Node* x, Node* z) {
		if (z->left->color == RED || z->right->color == RED) {			
			if (z == x->right) {
				if (z->right->color == BLACK) 	// Fixing rule (a)
					rotateRight(z);
				x = rotateLeft(x);				// Fixing rule (b)
			}
			else {
				if (z->left->color == BLACK)	// Fixing rule (a)
					rotateLeft(z);
				x = rotateRight(x);				// Fixing rule (b)
			}
			x->left->color = BLACK;				// Fixing rule (c)
			x->right->color = BLACK;			// Fixing rule (c)
			x = nil->left;						// terminate the bottom up pass
		}
		return x;
	}

	Node* performActualDeletion(Node* h) {
		bool fixed = false;
		Node **p = h == h->parent->left ? &h->parent->left : &h->parent->right;
		if (isNil(h->left)) {
			if (isNil(h->right)) {
				nil->parent = h->parent;
				fixed = h->color == RED;
				delete h;
				h = nil;
			}
			else {
				fixed = true;
				Node* tmp = h->right;
				tmp->parent = h->parent;
				delete h;
				h = tmp;
				h->color = BLACK;
			}
		}
		else { // h->right == nil
			fixed = true;
			Node* tmp = h->left;
			tmp->parent = h->parent;
			delete h;
			h = tmp;
			h->color = BLACK;
		}

		*p = h;
		if (fixed)
			h = NULL;
		return h;
	}

private:
	inline bool isRed(Node* x) {
		return x->color == RED;
	}

	inline bool isNil(Node* x) {
		return x == nil;
	}

	inline void colorFlip(Node* h) {
		h->color = (COLOR)!(int)h->color;
		h->left->color = (COLOR)!(int)h->left->color;
		h->right->color = (COLOR)!(int)h->right->color;
	}

	inline Node* rotateLeft(Node* h) {
		this->incLeftRotations();
		Node* x = h->right;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;
		else  h->parent->right = x;
		h->right = x->left;
		x->left->parent = h;
		x->left = h;
		h->parent = x;
		x->color = x->left->color;
		x->left->color = RED;
		return x;
	}

	inline Node* rotateRight(Node* h) {
		this->incRightRotations();
		Node* x = h->left;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;
		else  h->parent->right = x;
		h->left = x->right;
		x->right->parent = h;
		x->right = h;
		h->parent = x;
		x->color = x->right->color;
		x->right->color = RED;
		return x;
	}
};


