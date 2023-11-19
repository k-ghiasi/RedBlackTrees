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
		// This constructor is used only for nilSentinel
		Node() {
			this->color = BLACK;
			left = right = this;
		}

        Node(Key key, Value value, Node* nilSentinel) {
            this->key = key;
            this->value = value;
            this->color = RED;
            left = right = nilSentinel;
        }
    };

	Node *nilSentinel;
    Node *root;            // root of the BST
	int  seqNo;

public:
	LeftLeaningRedBlackBST()
	{
		nilSentinel = new Node();
		root = nilSentinel;
		seqNo = 0;
	}

public:
    inline Node *getRoot() {
        return root;
    }

public:
    inline bool contains(Key key) {
        return (get(key) != nilSentinel);
    }

public:
    inline Value get(Key key) {
        return get(root, key);
    }

	virtual int size() {
		return recSize(root);
	}

	int recSize(Node* h) {
		if (h == nilSentinel)
			return 0;
		else
			return 1 + recSize(h->left) + recSize(h->right);
	}

private:
    inline Value get(Node *x, Key key) {
		//if (x == nilSentinel)
		//	throw "Not found";
        if (key== x->key) return x->value;
        if (key < x->key) return get(x->left, key);
        else return get(x->right, key);
    }

public:
    inline Value min() {
        if (root == nilSentinel) return nilSentinel;
        else return min(root);
    }


private:
    inline Value min(Node *x) {
        if (x->left == nilSentinel) return x->value;
        else return min(x->left);
    }

public:
    inline Value max() {
        if (root == nilSentinel) return nilSentinel;
        else return max(root);
    }

private:
    inline Value max(Node *x) {
        if (x->right == nilSentinel) return x->value;
        else return max(x->right);
    }

public:
    inline void put(Key key, Value value) {
		drawTreeAndSave("Insert" + to_string(key) + "-Before", key);
        root = put(root, key, value);
        root->color = BLACK;
		drawTreeAndSave("Insert" + to_string(key) + "-After", key);
    }

	virtual string drawTree(string operation, Key selectedKey, void* deficiencyParent = NULL, char deficiencyDirection = '-')
	{
		string nodeName;
		string style;
		string color;
		Node* parent;
		Node* node;
		char direction;

		string result = "digraph structs { \n";
		if (root->key == selectedKey)
			style = ";style=dashed";
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
				if (parent == nilSentinel)
					node = root;
				else
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
				if (node->left->key == selectedKey)
					style = ";style=dashed";
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
				if (node->right->key == selectedKey)
					style = ";style=dashed";
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


	void drawTreeAndSave(string operation, Key selected)
	{
		if (draw_disabled)
			return;
		string out = drawTree(operation, selected);
		string fileName = to_string(seqNo++) + operation;
		fstream fout;
		fout.open(fileName + ".dot", ios::out);
		fout << out;
		fout.close();
		string cmd;
		cmd = "dot -Gdpi=300 -Tpng " + fileName + ".dot" + " -o " + fileName + ".png";
		cout << cmd << endl;
		system(cmd.c_str());
		cmd = "del " + fileName + ".dot";
		system(cmd.c_str());
	}

private:
    inline Node *put(Node* &h, Key key, Value value) {
		if (h == nilSentinel)
		{
			Node* g = new Node(key, value, nilSentinel);
			h = g;
			drawTreeAndSave("After-Insert" + to_string(key)+ "-Before-FixUp", key);
			return g;
		}

        if (key == h->key)
            h->value = value;
        else if (key < h->key)
            h->left = put(h->left, key, value);
        else
            h->right = put(h->right, key, value);


		bool fixed = false;
		if (isRed(h->right))
		{
			drawTreeAndSave("LeftRotate", h->key);
			h = rotateLeft(h);
			fixed = true;
		}

		if (isRed(h->left) && isRed(h->left->left))
		{
			drawTreeAndSave("RightRotate", h->key);
			h = rotateRight(h);
			fixed = true;
		}

		if (isRed(h->left) && isRed(h->right))
		{
			drawTreeAndSave("ColorFlip", h->key);
			colorFlip(h);
			fixed = true;
		}

		if (!fixed)
			drawTreeAndSave("Implicit-Fixup", h->key);

        return h;
    }

public:
    void remove(Key key) {
		drawTreeAndSave("Remove" + to_string(key) + "-Before", key);
        root = remove(root, key);
        if (root != nilSentinel) {
            root->color = BLACK;
        }
		drawTreeAndSave("Remove" + to_string(key) + "-After", key);
    }

private:
    Node *remove(Node* &h, Key key) {
        if (h != nilSentinel) {
            if (key < h->key) {
				if (!isRed(h->left) && h->left != nilSentinel && !isRed(h->left->left))
				{
					drawTreeAndSave("MoveRedLeft", h->key);
					h = moveRedLeft(h);
				}
				drawTreeAndSave("DownLeft", h->key);
                h->left = remove(h->left, key);
            } else {
				if (isRed(h->left))
				{
					drawTreeAndSave("RotateRight", h->key);
					h = rotateRight(h);
				}
                if ((key == h->key) && (h->right == nilSentinel)) {
					drawTreeAndSave("Delete" + to_string(key), key);
					if (root == h)
						root = nilSentinel;
                    delete h;
					h = nilSentinel;
                    return nilSentinel;
                }
				if (!isRed(h->right) && h->right != nilSentinel && !isRed(h->right->left))
				{
					drawTreeAndSave("MoveRedRight", h->key);
					h = moveRedRight(h);
				}
                if (key == h->key) {
					drawTreeAndSave("KeyFound", h->key);
					drawTreeAndSave("DeleteMin" + to_string(h->right->key), h->right->key);
					Key k = get(h->right, min(h->right));
					Value v = min(h->right);
                    h->right = deleteMin(h->right);
					h->value = k;
					h->key = v;
				}
				else
				{
					drawTreeAndSave("DownRight", h->key);
					h->right = remove(h->right, key);
				}
            }

			drawTreeAndSave("FixUp", h->key);
            return fixUp(h);
        }
        return nilSentinel;
    }

public:
    void deleteMin() {
        root = deleteMin(root);
        if (root != nilSentinel)
            root->color = BLACK;
    }

private:
    Node *deleteMin(Node* &h) {
		Key key = h->key;
        if (h->left == nilSentinel) {
			if (root == h)
				root = nilSentinel;
            delete h;
			h = nilSentinel;
            return nilSentinel;
        }
		if (!isRed(h->left) && !isRed(h->left->left))
		{
			drawTreeAndSave("MoveRedLeft", h->key);
			h = moveRedLeft(h);
		}

		drawTreeAndSave("DeleteMin" + to_string(h->left->key), h->left->key);
        h->left = deleteMin(h->left);

		drawTreeAndSave("FixUp", h->key);
        Node* g =fixUp(h);
		return g;
    }

public:
    void deleteMax() {
        root = deleteMax(root);

        if (root != nilSentinel)
            root->color = BLACK;
    }

private:
    Node deleteMax(Node *h) {
		Key key = h->key;
		drawTreeAndSave("DeleteMax" + to_string(key), h->key);
        if (isRed(h->left))
            h = rotateRight(h);

        if (h->right == nilSentinel) {
            delete h;
            return nilSentinel;
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

    inline void colorFlip(Node* &h) {
		Key key = h->key;
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

	inline Node *rotateLeft(Node* &h) {  // Make a right-leaning 3-node lean to the left.
		Key key = h->key;
		this->incLeftRotations();
        Node *x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = x->left->color;
        x->left->color = RED;
		if (root == h)
			root = x;
		h = x;
        return x;
    }

    inline Node *rotateRight(Node* &h) {  // Make a left-leaning 3-node lean to the right.
		Key key = h->key;
		this->incRightRotations();
        Node *x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = x->right->color;
        x->right->color = RED;	
		if (root == h)
			root = x;
		h = x;
        return x;
    }

    Node *moveRedLeft(Node* &h) {  // Assuming that h is red and both h.left and h.left.left
        // are black, make h.left or one of its children red.
		drawTreeAndSave("ColorFlip", h->key);
        colorFlip(h);

        if (h->right != nilSentinel && isRed(h->right->left)) {
			drawTreeAndSave("RotateRight", h->right->key);
            h->right = rotateRight(h->right);
			
			drawTreeAndSave("RotateLeft", h->key);
            h = rotateLeft(h);
			
			drawTreeAndSave("ColorFlip", h->key);
            colorFlip(h);
			
        }
        return h;
    }

	inline  Node *moveRedRight(Node* &h) {  // Assuming that h is red and both h.right and h.right.left
        // are black, make h.right or one of its children red.
		drawTreeAndSave("ColorFlip", h->key);
        colorFlip(h);

        if (h->left != nilSentinel && isRed(h->left->left)) {
			drawTreeAndSave("RotateRight", h->key);
            h = rotateRight(h);
			
			drawTreeAndSave("ColorFlip", h->key);
            colorFlip(h);
        }
        return h;
    }

	inline  Node *fixUp(Node* &h) {

		if (isRed(h->right))
		{
			drawTreeAndSave("RotateLeft", h->key);
			h = rotateLeft(h);
		}

		if (isRed(h->left) && isRed(h->left->left))
		{
			drawTreeAndSave("RotateRight", h->key);
			h = rotateRight(h);
		}

		if (isRed(h->left) && isRed(h->right))
		{
			drawTreeAndSave("ColorFlip", h->key);
			colorFlip(h);
		}

        return h;
    }
};