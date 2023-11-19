/*
Authors:
		Sayed Kamaledin Ghiasi-Shirazi
		Taraneh Ghandi
		Ali Taghizadeh
		Ali Rahimi-Baigi
*/

#pragma once
#include "Map.h"


template<class Key, class Value>
class PureParitySeeking23RedBlackBST : public Map<Key, Value> {
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
	PureParitySeeking23RedBlackBST() {
		nilSentinel = new Node();
		seqNo = 0;
		//root = nilSentinel->left;
	}

	virtual Value get(Key key) {
		Node*	root = nilSentinel->left;
		return get(root, key);
	}

	virtual void put(Key key, Value value) {
		Node* h;
		drawTreeAndSave("Insert" + to_string(key) + "-Start", key);
		h = put_top_down_pass(key, value);
		drawTreeAndSave("Insert" + to_string(key) + "-After-top-down-pass", key);
		put_bottom_up_pass(h); // fixup pass
		drawTreeAndSave("Insert" + to_string(key) + "-After-bottom-up-pass", key);
	}

	virtual void remove(Key key) {
		nilSentinel->key = key;
		Node* x; // Parent of deficient node
		drawTreeAndSave("Remove" + to_string(key) + "-Start", key);
		x = remove_top_down_pass(key);
		if (x) {
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Remove" + to_string(key) + "-After-top-down-pass", key, x->parent, deficiecyDirection);
			remove_bottom_up_pass(x);
			drawTreeAndSave("Remove" + to_string(key) + "-After-Bottom-up-pass", key);
		}
		else
			drawTreeAndSave("Remove" + to_string(key) + "-After-top-down-pass", key);
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

	virtual string drawTree(string operation, Key selectedKey, void* deficiencyParent, char deficiencyDirection)
	{
		string nodeName;
		string style;
		string color;
		Node* parent;
		Node* node;
		char direction;
		Node* root = nilSentinel->left;

		string result = "digraph structs { \n";
		if (root->key == selectedKey)
			style = ";style=dashed";
		if (nilSentinel == deficiencyParent && deficiencyDirection == 'L')
			style = ";shape=box;style=dashed";
		result += "\"" + operation + "\"" + "[color = blue; shape = none];\n";
		color = (root->color == RED ? "red" : "black");
		result += to_string(root->key) + " [color=" + color + style + "];\n";
		queue<pair<Node*, char> > q;
		q.push(pair<Node*, char>(nilSentinel, 'L'));

		while (q.empty() == false)
		{
			pair<Node*, char> p;
			p = q.front();
			q.pop();
			parent = p.first;
			direction = p.second;
			if (direction == 'L')
				node = parent->left;
			else
				node = parent->right;

			color = (node->color == RED ? "red" : "black");
			style = "";
			if (node != nilSentinel)
			{
				nodeName = to_string(node->key);
			}
			else
			{
				nodeName = "N_";
				nodeName += direction + to_string(parent->key);
				style = ";shape = point; label=\"\"";
			}

			if (parent != nilSentinel)
				result += to_string(parent->key) + " -> " + nodeName + " [color=" + color + style + "];\n";

			if (node != nilSentinel)
			{
				string nodeLeftName, nodeRightName;
				color = (node->left->color == RED ? "red" : "black");
				style = "";
				if (node->left != nilSentinel)
					nodeLeftName = to_string(node->left->key);
				else
				{
					nodeLeftName = "N_L" + to_string(node->key);
					style = ";shape = point; label=\"\"";
				}
				if (node->left!= nilSentinel && node->left->key == selectedKey)
					style = ";style=dashed";

				if (node == deficiencyParent && deficiencyDirection == 'L')
					style = ";shape=box;style=dashed";

				result += nodeLeftName + " [color=" + color + style + "]; \n";

				color = (node->right->color == RED ? "red" : "black");
				style = "";
				if (node->right != nilSentinel)
					nodeRightName = to_string(node->right->key);
				else
				{
					nodeRightName = "N_R" + to_string(node->key);
					style = ";shape = point; label=\"\"";
				}
				if (node->right!= nilSentinel && node->right->key == selectedKey)
					style = ";style=dashed";

				if (node == deficiencyParent && deficiencyDirection == 'R')
					style = ";shape=box;style=dashed";

				result += nodeRightName + " [color=" + color + style + "]; \n";

				result += "{rank = same; " + nodeLeftName + " ; " + nodeRightName + " };\n";

				//result += drawTree(node->left, node, 'L');
				//result += drawTree(node->right, node, 'R');
				q.push(pair<Node*, char>(node, 'L'));
				q.push(pair<Node*, char>(node, 'R'));
			}
		}

		result += "}";
		return result;
	}


	void drawTreeAndSave(string operation, Key selected, Node* deficiencyParent = NULL, char deficiencyDirection = '-')
	{
		if (draw_disabled)
			return;
		string out = drawTree(operation, selected, deficiencyParent, deficiencyDirection);
		string fileName = to_string(seqNo++) + operation;
		fstream fout;
		fout.open(fileName + ".dot", ios::out);
		fout << out;
		fout.close();
		string cmd;
		cmd = "dot -Gdpi=300 -Tpng \"" + fileName + ".dot\"" + " -o \"" + fileName + ".png\"";
		cout << cmd << endl;
		system(cmd.c_str());
		cmd = "del " + fileName + ".dot";
		system(cmd.c_str());
	}


private:
	Node*	nilSentinel;
	int		seqNo;

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
		Node *p, *y;
		char overfullDirection;
		while (true) {								// x->color is certainly BLACK
			this->incBottomUpVisit();
			p = x->parent;
			if (p->color == RED) {
				if (p == p->parent->left) {
					if (x == p->right) { 			// Rule (b.2)
						overfullDirection = x == x->parent->left ? 'L' : 'R';
						drawTreeAndSave("Rule (b.2)", x->key, x->parent, overfullDirection);
						x = p;
						p = rotateLeft(p);
					}
					overfullDirection = x == x->parent->left ? 'L' : 'R';
					drawTreeAndSave("Rule (b.1)", x->key, x->parent, overfullDirection);
					p = rotateRight(p->parent);		// Rule (b.1)
				}
				else {
					if (x == p->left) {				// Rule (b.2)
						overfullDirection = x == x->parent->left ? 'L' : 'R';
						drawTreeAndSave("Rule (b.2)", x->key, x->parent, overfullDirection);
						x = p;
						p = rotateRight(p);
					}
					overfullDirection = x == x->parent->left ? 'L' : 'R';
					drawTreeAndSave("Rule (b.1)", x->key, x->parent, overfullDirection);
					p = rotateLeft(p->parent);		// Rule (b.1)
				}
			}
			y = (x == p->left) ? p->right : p->left;
			if (y->color == RED) {
				overfullDirection = x == x->parent->left ? 'L' : 'R';
				drawTreeAndSave("Rule (a.2)", x->key, x->parent, overfullDirection);
				y->color = BLACK;					// Rule (a.2)
				x = p;
			}
			else if (p->color == BLACK)
				break;
		}
		if (x != nilSentinel->left)					// x->color is certainly BLACK
		{
				overfullDirection = x == x->parent->left ? 'L' : 'R';
				drawTreeAndSave("Rule (a.1)", x->key, x->parent, overfullDirection);
				x->color = RED;							// Rule (a.1)
		}
		else {
			overfullDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("No rule matches", x->key, x->parent, overfullDirection);
		}
	}

private:

	Node* remove_top_down_pass(Key key) {
		Node* deg2Node = NULL;
		Node*  h = nilSentinel->left;

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

	void remove_bottom_up_pass(Node* & x) {

		while (x != nilSentinel->left && x->color == BLACK) {
			x = applyParitySeekingRules(x);
		}
		char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
		if (x != nilSentinel->left)
			drawTreeAndSave("Parity-Seeking Rule (a)", x->key, x->parent, deficiecyDirection);
		x->color = BLACK; // Case I
	}

	Node* applyParitySeekingRules(Node* & x) {
		Node* p = x->parent;
		Node* y = x == x->parent->left ? x->parent->right : x->parent->left;
		char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
		if (y->color == RED) // Case III: Rotate to have a black sibling
			if (x == x->parent->left)
			{
				drawTreeAndSave("Parity-Seeking Rule (c)", p->key, x->parent, deficiecyDirection);
				rotateLeft(p);
			}
			else
			{
				drawTreeAndSave("Parity-Seeking Rule (c)", p->key, x->parent, deficiecyDirection);
				rotateRight(p);
			}
		else { 				//Case II: Make the sibling red
			drawTreeAndSave("Parity-Seeking Rule (b)", -1, x->parent, deficiecyDirection);
			y->color = RED;
			if (y->left->color == BLACK && y->right->color == BLACK)
				x = p;
			else {
				applyFixingRules(x, y, p); // Deficiency would certainly be fixed
				x = nilSentinel->left;	// set x equal to root to terminate the bottom up pass
			}
		}
		return x;
	}

	void applyFixingRules(Node* & x, Node* y, Node* p) {
		char deficiecyDirection = p == p->parent->left ? 'L' : 'R';
		if (x == x->parent->left) {
			if (y->left->color == RED) 	// Fixing rule (a)
			{
				drawTreeAndSave("Fixing Rule (a)", y->key, p->parent, deficiecyDirection);
				rotateRight(y);
			}
			drawTreeAndSave("Fixing Rule (b)", p->key, p->parent, deficiecyDirection);
			p = rotateLeft(p);				// Fixing rule (b)
		}
		else {
			if (y->right->color == RED)	// Fixing rule (a)
			{
				drawTreeAndSave("Fixing Rule (a)", y->key, p->parent, deficiecyDirection);
				rotateLeft(y);
			}
			drawTreeAndSave("Fixing Rule (b)", p->key, p->parent, deficiecyDirection);
			p = rotateRight(p);				// Fixing rule (b)
		}
		drawTreeAndSave("Fixing Rule (c)", p->key, p->parent, deficiecyDirection);
		p->left->color = BLACK;				// Fixing rule (c)
		p->right->color = BLACK;			// Fixing rule (c)
	}

	Node* performActualDeletion(Node* & h) {
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

