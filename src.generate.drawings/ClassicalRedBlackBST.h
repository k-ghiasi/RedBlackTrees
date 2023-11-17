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
		Node*	root = nilSentinel->left;
		root->color = BLACK;
		drawTreeAndSave("Insert" + to_string(key) + "-After-Bottom-up-pass", key);
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
				if (node->left != nilSentinel && node->left->key == selectedKey)
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
				if (node->right != nilSentinel && node->right->key == selectedKey)
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
		cmd = "dot.exe -Gdpi=300 -Tpng \"" + fileName + ".dot\"" + " -o \"" + fileName + ".png\"";
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
		if (key == x->key) return x->value;
		if (key < x->key) return get(x->left, key);
		else return get(x->right, key);
	}

	Node* put_top_down_pass(Key key, Value value) {
		Node** p = &nilSentinel->left;	//root is nilSentinel->left
		Node* h = nilSentinel;
		while (1){
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

	void put_bottom_up_pass(Node* & z) {
		Node*  y;   // sibling of parent of z
		Node* n;
		while (z->parent->color == RED) {
			char deficiecyDirection = z == z->parent->left ? 'L' : 'R';
			char rule = '-';
			if (z->parent == z->parent->parent->left) {
				y = z->parent->parent->right;
				if (y->color == RED) {
					drawTreeAndSave("Rule (a,b)", z->parent->parent->key, z->parent, deficiecyDirection);
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
					rule = 'a';
				}
				else {
					if (z == z->parent->right) {
						drawTreeAndSave("Rule (c)", z->key, z->parent, deficiecyDirection);
						z = z->parent;
						n = z;
						rotateLeft(n);
					}
					deficiecyDirection = z == z->parent->left ? 'L' : 'R';
					drawTreeAndSave("Rule (d)", z->parent->parent->key, z->parent, deficiecyDirection);
					n = z->parent->parent;
					rotateRight(n);
					rule = 'd';
				}
			}
			else
			{
				y = z->parent->parent->left;
				if (y->color == RED) {
					drawTreeAndSave("Rule (a,b)", z->parent->parent->key, z->parent, deficiecyDirection);
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
					rule = 'a';
				}
				else {
					if (z == z->parent->left) {
						drawTreeAndSave("Rule (c)", z->key, z->parent, deficiecyDirection);
						z = z->parent;
						n = z;
						rotateRight(n);
					}
					deficiecyDirection = z == z->parent->left ? 'L' : 'R';
					drawTreeAndSave("Rule (d)", z->parent->parent->key, z->parent, deficiecyDirection);
					n = z->parent->parent;
					rotateLeft(n);
					rule = 'd';
				}
			}
			if (rule == '-') {
				drawTreeAndSave("No rule matches", -1, z->parent, deficiecyDirection);
			}
		}
		char deficiecyDirection = z == z->parent->left ? 'L' : 'R';
		drawTreeAndSave("Parent is Black. Terminate buttom-up pass.", -1, z->parent, deficiecyDirection);
	}

private:

	Node* remove_top_down_pass(Key key) {
		bool fixed = false;
		Node* deg2Node = NULL;
		Node*  h = nilSentinel->left;

		while (1) {
			if (key > h->key) {
				if (isNil(h->right)) {
					deg2Node->key = h->key;
					deg2Node->value = h->value;
					h = performActualDeletion(h, fixed);
					if (fixed)
						h = NULL;
					break;
				}
				else {
					h = h->right;
					continue;
				}
			}
			else {
				if (key < h->key) {
					h = h->left;
					continue;
				}
				else if (key == h->key) {
					//if (isNil(h))
					//	throw "key not fount";
					if (isNil(h->left) || isNil(h->right)) {
						h = performActualDeletion(h, fixed);
						if (fixed)
							h = NULL;
						break;
					}
					else {
						deg2Node = h;
						h = h->left;
						continue;
					}
				}
			}
		}
		return h;
	}

	void remove_bottom_up_pass(Node* x) {

		while (x != nilSentinel->left && x->color == BLACK) {
			if (x == x->parent->left)
				x = leftRemoveFixup(x);
			else 
				x = rightRemoveFixup(x);
		}
		
		if (x->color == RED)
		{
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Rule (e)", x->key, x->parent, deficiecyDirection);
			x->color = BLACK;
		}
	}

	Node* leftRemoveFixup(Node* x) {
		Node* p = x->parent;
		Node* w = p->right;
		char rule = '-';
		if (w->color == RED) {				// Rule (a)
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Rule (a)", p->key, x->parent, deficiecyDirection);
			rotateLeft(p);
			w = p->right;
			rule = 'a';
		}
		if (w->left->color == BLACK && w->right->color == BLACK){ //Rule (b)
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Rule (b)", p->key, x->parent, deficiecyDirection);
			w->color = RED;
			x = p;
			rule = 'b';
		}
		else {
			Node* q = x->parent;
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			if (w->right->color == BLACK) {  //Rule (c)
				drawTreeAndSave("Rule (c)", w->key, q, deficiecyDirection);
				rotateRight(w);
				w = p->right;
			}
			drawTreeAndSave("Rule (d)", p->key, q, deficiecyDirection);
			w->right->color = BLACK;  //Rule (d)
			rotateLeft(p);
			x = nilSentinel->left;
			rule = 'd';
		}
		if (rule == '-'){
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("No rule matches", x->key, x->parent, deficiecyDirection);
		}
		return x;
	}

	Node* rightRemoveFixup(Node* x) {
		Node* p = x->parent;
		Node* w = p->left;			
		char rule = '-';
		if (w->color == RED) {		// Rule (a)
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Rule (a)", p->key, x->parent, deficiecyDirection);
			rotateRight(p);
			w = p->left;
			rule = 'a';
		}
		if (w->right->color == BLACK && w->left->color == BLACK) { //Rule (b)
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("Rule (b)", p->key, x->parent, deficiecyDirection);
			w->color = RED;
			x = p;
			rule = 'b';
		}
		else {
			Node* q = x->parent;
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			if (w->left->color == BLACK) {  //Rule (c)
				drawTreeAndSave("Rule (c)", w->key, q, deficiecyDirection);
				rotateLeft(w);
				w = p->left;
			}
			drawTreeAndSave("Rule (d)", p->key, q, deficiecyDirection);
			w->left->color = BLACK; //Rule (d)
			rotateRight(p);
			x = nilSentinel->left;
			rule = 'd';
		}
		if (rule == '-'){
			char deficiecyDirection = x == x->parent->left ? 'L' : 'R';
			drawTreeAndSave("No rule matches", x->key, x->parent, deficiecyDirection);
		}
		return x;
	}

	Node* performActualDeletion(Node* h, bool& fixed) {
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

