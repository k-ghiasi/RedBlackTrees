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
class PureParitySeeking234RedBlackBST : public Map<Key, Value> {
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
		// This constructor is used only for nilSentinel
		Node() {
			this->color = BLACK;
			left = right = parent = this;
		}

		Node(Key key, Value value, Node* parent, Node* nilSentinel) {
			this->key = key;
			this->value = value;
			this->parent = parent;
			this->color = BLACK;
			left = right = nilSentinel;
		}
	};

public:
	PureParitySeeking234RedBlackBST() {
		nilSentinel = new Node();
	}

	virtual Value get(Key key) {
		Node*	root = nilSentinel->left;
		return get(root, key);
	}

	virtual void put(Key key, Value value) {
		Node* h;
		h = put_top_down_pass(key, value);
		put_bottom_up_pass(h); // fixup pass
	}

	virtual void remove(Key key) {
		nilSentinel->key = key;
		Node* x; // Parent of deficient node
		x = remove_top_down_pass(key);
		if (x)
			remove_bottom_up_pass(x);
	}

	virtual int size() {
		return recSize(nilSentinel->left);
	}

	int recSize(Node* h) {
		if (h == nilSentinel)
			return 0;
		else
			return 1 + recSize(h->left) + recSize(h->right);
	}

private:
	Node*	nilSentinel;

private:
	inline Value get(Node* x, Key key) {
		//if (x == nilSentinel)
		//	throw "Not found";
		this->incTopDownVisit();
		if (key == x->key) return x->value;
		if (key < x->key) return get(x->left, key);
		else return get(x->right, key);
	}

	Node* put_top_down_pass(Key key, Value value) {
		Node** p = &nilSentinel->left;	//root is nilSentinel->left
		Node* h = nilSentinel;
		while (1) {
			this->incTopDownVisit();
			if (*p == nilSentinel) {
				*p = new Node(key, value, h, nilSentinel);
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

	void put_bottom_up_pass(Node* x) {
		Node *p, *g;
		while (x->parent->color == RED) {			// x->color is certainly BLACK
			this->incBottomUpVisit();
			p = x->parent;
			g = p->parent;
			if (p == g->left) {
				if (g->right->color == RED) {		// Rule (c)
					x->color = RED;
					g->left->color = BLACK;
					g->right->color = BLACK;
					x = g;
				}
				else {
					if (x == p->right) {			// Rule (b.2)
						x = p;
						rotateLeft(p);
					}
					rotateRight(g);					// Rule (b.1)
				}
			}
			else
			{
				if (g->left->color == RED) {		// Rule (c)
					x->color = RED;
					g->left->color = BLACK;
					g->right->color = BLACK;
					x = g;
				}
				else {
					if (x == p->left) {				// Rule (b.2)
						x = p;
						rotateRight(p);
					}
					rotateLeft(g);					// Rule (b.1)
				}
			}
		}
		if (x != nilSentinel->left)					// x->color is certainly BLACK
			x->color = RED;							// Rule (a)
	}

private:

	Node* remove_top_down_pass(Key key) {
		Node* deg2Node = NULL;
		Node* h = nilSentinel->left;

		while (1) {
			this->incTopDownVisit();
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

		while (x != nilSentinel->left && x->color == BLACK) {
			this->incBottomUpVisit();
			x = applyParitySeekingRules(x);
		}
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
			x = nilSentinel->left;						// terminate the bottom up pass
		}
		return x;
	}

	Node* performActualDeletion(Node* h) {
		bool fixed = false;
		Node **p = h == h->parent->left ? &h->parent->left : &h->parent->right;
		if (isNil(h->left)) {
			if (isNil(h->right)) {
				nilSentinel->parent = h->parent;
				fixed = h->color == RED;
				delete h;
				h = nilSentinel;
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
		else { // h->right == nilSentinel
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
		return x == nilSentinel;
	}

	inline Node* rotateLeft(Node* h) {
		this->incLeftRotations();
		Node* x = h->right;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;
		else h->parent->right = x;
		h->right = x->left;
		x->left->parent = h;
		x->left = h;
		h->parent = x;
		COLOR c = x->color;
		x->color = x->left->color;
		x->left->color = c;
		return x;
	}

	inline Node* rotateRight(Node* h) {
		this->incRightRotations();
		Node* x = h->left;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;
		else h->parent->right = x;
		h->left = x->right;
		x->right->parent = h;
		x->right = h;
		h->parent = x;
		COLOR c = x->color;
		x->color = x->right->color;
		x->right->color = c;
		return x;
	}
};


