/*
Authors:
		Ali Taghizadeh
		Ali Rahimi-Baigi
		Sayed Kamaledin Ghiasi-Shirazi		
		Taraneh Ghandi		
*/

#pragma once
#include "Map.h"


template<class Key, class Value>
class ClassicalRedBlackBST: public Map<Key, Value>{
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
			this->color = RED;
			left = right = nilSentinel;
		}
	};

public:
	ClassicalRedBlackBST() {
		nilSentinel = new Node();
		//root = nilSentinel->left;
	}

	virtual Value get(Key key) {
		Node*	root = nilSentinel->left;
		return get(root, key);
	}

	virtual void put(Key key, Value value) {
		Node* h;
		h = put_top_down_pass(key, value);
		put_bottom_up_pass(h); // fixup pass
		Node*	root = nilSentinel->left;
		root->color = BLACK;
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
		while (1){
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

	void put_bottom_up_pass(Node* z) {
		Node*  y;   // sibling of parent of z

		while (z->parent->color == RED) {
			this->incBottomUpVisit();
			if (z->parent == z->parent->parent->left){
				y = z->parent->parent->right;
				if (y->color == RED){
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->right){
						z = z->parent;
						rotateLeft(z);
					}
					rotateRight(z->parent->parent);
				}
			}
			else
			{
				y = z->parent->parent->left;
				if (y->color == RED) {
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						rotateRight(z);
					}
					rotateLeft(z->parent->parent);
				}
			}
		}
	}

private:

	Node* remove_top_down_pass(Key key) {
		Node* deg2Node = NULL;
		Node*  h = nilSentinel->left;

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
			if (x == x->parent->left)
				x = leftRemoveFixup(x);
			else 
				x = rightRemoveFixup(x);
		}
		x->color = BLACK;	// Rule (e)
	}

	Node* leftRemoveFixup(Node* x) {
		Node* p = x->parent;
		Node* w = p->right;
		if (w->color == RED) {		// Rule (a)
			rotateLeft(p);
			w = p->right;
		}
		if (w->left->color == BLACK && w->right->color == BLACK){ //Rule (b)
			w->color = RED;
			x = p;
		}
		else
		{
			if (w->right->color == BLACK) {  //Rule (c)
				rotateRight(w);
				w = p->right;
			}
			w->right->color = BLACK;  //Rule (d)
			rotateLeft(p);
			x = nilSentinel->left;
		}
		return x;
	}

	Node* rightRemoveFixup(Node* x) {
		Node* p = x->parent;
		Node* w = p->left;			
		if (w->color == RED) {		// Rule (a)
			rotateRight(p);
			w = p->left;
		}
		if (w->right->color == BLACK && w->left->color == BLACK) { //Rule (b)
			w->color = RED;
			x = p;
		}
		else {
			if (w->left->color == BLACK) {  //Rule (c)
				rotateLeft(w);
				w = p->left;
			}
			w->left->color = BLACK; //Rule (d)
			rotateRight(p);
			x = nilSentinel->left;
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

	inline void colorFlip(Node* h) {
		h->color = (COLOR)!(int)h->color;
		h->left->color = (COLOR)!(int)h->left->color;
		h->right->color = (COLOR)!(int)h->right->color;
	}

	inline Node* rotateLeft(Node* h) {
		this->incLeftRotations();
		Node* x = h->right;
		COLOR c = h->right->color;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;   
		else  h->parent->right = x;
		h->right = x->left;
		x->left->parent = h;
		x->left = h;
		h->parent = x;
		x->color = x->left->color;
		x->left->color = c;
		return x;
	}

	inline Node* rotateRight(Node* h) {
		this->incRightRotations();
		Node* x = h->left;
		COLOR c = h->left->color;
		x->parent = h->parent;
		if (h->parent->left == h) h->parent->left = x;   
		else  h->parent->right = x;
		h->left = x->right;
		x->right->parent = h;
		x->right = h;
		h->parent = x;
		x->color = x->right->color;
		x->right->color = c;
		return x;
	}
};

