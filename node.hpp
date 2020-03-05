#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <assert.h>
using namespace std;

#define max(a, b) ((a) > (b) ? (a) : (b))

// Data structure to store a Binary Tree node
class Node {
public:
	Node(int data)
	{
		this->data = data;
		this->left = this->right = nullptr;
	}

	friend class printTree;
	friend class splayTree;
	Node *left, *right;
private:
	int data;
};

class splayTree {
public:
	splayTree() : root(nullptr) {}

	Node* getRoot() const {
		return root;
	}

	enum class Imbalance {
		None = 0,
		LLImbalance,
		LLLImbalance,
		LRImbalance,
		RRImbalance,
		RRRImbalance,
		RLImbalance,
		Unknown
	};

	Imbalance detectSplayImbalance(Node* curNode, int data, bool checkRight) {
		Imbalance imbalance = Imbalance::None;
		if (curNode->data == data) {
			return imbalance;
		}

		if (checkRight) {
			if (curNode->right && curNode->right->data == data) {
				cout << "Detected RRImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction right" << endl;
				imbalance = Imbalance::RRImbalance;
			}
			if (imbalance == Imbalance::None && curNode->right && curNode->right->right && curNode->right->right->data == data) {
				cout << "Detected RRRImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction right" << endl;
				imbalance = Imbalance::RRRImbalance;
			}
			if (imbalance == Imbalance::None && curNode->right && curNode->right->left && curNode->right->left->data == data) {
				cout << "Detected RLImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction right" << endl;
				imbalance = Imbalance::RLImbalance;
			}
			if (imbalance == Imbalance::None) {
				cout << "Cannot detect imbalance at node : " << curNode->data << " when inserting value : " << data << " and hint direction right" << endl;
			}
		} else {
			if (curNode->left && curNode->left->data == data) {
				cout << "Detected LLImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction left" << endl;
				imbalance = Imbalance::LLImbalance;
			}
			if (imbalance == Imbalance::None && curNode->left && curNode->left->left && curNode->left->left->data == data) {
				cout << "Detected LLLImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction left" << endl;
				imbalance = Imbalance::LLLImbalance;
			}
			if (imbalance == Imbalance::None && curNode->left && curNode->left->right && curNode->left->right->data == data) {
				cout << "Detected LRImbalance imbalance at node : " << curNode->data << " with inserting value : " << data << " and hint direction left" << endl;
				imbalance = Imbalance::LRImbalance;
			}
			if (imbalance == Imbalance::None) {
				cout << "Cannot detect imbalance at node : " << curNode->data << " when inserting value : " << data << " and hint direction left" << endl;
			}
		}
		return imbalance;
	}

	

	void insert(int data) {
		if (root == nullptr) {
			root = new Node(data);
			return;
		}
		if (data == root->data) {
			return;
		}

		bool nextTookRight = false;
		insertImpl(&root, data, nextTookRight);

		if (root->data != data) {
			auto imbalance = detectSplayImbalance(root, data, nextTookRight);
			handleImbalance(&root, imbalance);
		}
	}

	void insertImpl(Node** parent, int data, bool& tookRight) {
		assert(parent);
		Node* curNode = *parent;
		assert(curNode);

		if (data == curNode->data) {
			return;
		}

		if (data > curNode->data) {
			tookRight = true;
			if (curNode->right) {
				bool nextTookRight = false;
				insertImpl(&curNode->right, data, nextTookRight);
				if (curNode->data != data) {
					auto imbalance = detectSplayImbalance(*parent, data, true);
					handleImbalance(parent, imbalance);
				}
			} else {
				curNode->right = new Node(data);
				return;
			}
		} else {
			tookRight = false;;
			if (curNode->left) {
				bool nextTookRight = false;
				insertImpl(&curNode->left, data, nextTookRight);
				if (curNode->data != data) {
					auto imbalance = detectSplayImbalance(*parent, data, false);
					handleImbalance(parent, imbalance);
				}
			} else {
				curNode->left = new Node(data);
				return;
			}
		}
	}

	void remove(int data) {
		if (root == nullptr) {
			return;
		}
		removeImpl(&root, data);
	}

	void removeImpl(Node** parent, int data) {
		if (not (*parent)) {
			return;
		}
		assert(*parent);

		Node* curNode = *parent;

		if (data < curNode->data) {
			removeImpl(&curNode->left, data);
		} else if (data > curNode->data) {
			removeImpl(&curNode->right, data);
		} else {
			cout << "Found victim node : " << curNode->data << " at address : " << hex << curNode << dec << endl;
			deleteNode(parent, &curNode->data);
		}
		return;
	}

	void deleteNode(Node** parent, int* replaceValue) {
		assert(parent);
		Node* curNode = *parent;
		assert(curNode);

		if (not curNode->right && not curNode->left) {
			cout << "Found secondary victim node : " << curNode->data << " at address : " << hex << curNode << dec << endl;
			if (replaceValue) {
				*replaceValue = curNode->data;
			}
			free(curNode);
			*parent = nullptr;
			return;
		}
		if (curNode->right) {
			if (curNode->right->left) {
				Node** newParent = &curNode->right;
				pruneSmallestLeftNode(newParent, replaceValue);
			} else {
				cout << "Replacing old value : " << *replaceValue << " with new value : " << curNode->right->data << endl;
				*replaceValue = curNode->right->data;
				deleteNode(&curNode->right, &curNode->right->data);
			}
		} else {
			if (curNode->left->right) {
				pruneLargestRightNode(&curNode->left, replaceValue);
			} else {
				cout << "Replacing old value : " << *replaceValue << " with new value : " << curNode->left->data << endl;
				*replaceValue = curNode->left->data;
				deleteNode(&curNode->left, &curNode->left->data);
			}
		}
		return;
	}

	void pruneSmallestLeftNode(Node** parent, int* replacePayload) {
		assert(parent);
		Node* curNode = *parent;
		assert(curNode);

		if (curNode->left) {
			pruneSmallestLeftNode(&curNode->left, replacePayload);
		} else {
			cout << "Replacing old value : " << *replacePayload << " with new value : " << curNode->data << endl;
			*replacePayload = curNode->data;
			if (curNode->right) {
				cout << "Need replacement value for : " << curNode->data << endl;
				deleteNode(&curNode, &curNode->data);
			} else {
				free(curNode);
				*parent = nullptr;
			}
		}
		return;
	}

	void pruneLargestRightNode(Node** parent, int* replacePayload) {
		assert(parent);
		Node* curNode = *parent;
		assert(curNode);

		if (curNode->right) {
			pruneLargestRightNode(&curNode->right, replacePayload);
		} else {
			cout << "Replacing old value : " << *replacePayload << " with new value : " << curNode->data << endl;
			*replacePayload = curNode->data;
			if (curNode->left) {
				cout << "Need replacement value for : " << curNode->data << endl;
				deleteNode(&curNode, &curNode->data);
			} else {
				free(curNode);
				*parent = nullptr;
			}
		}
	}

	bool verifyBST(Node* root) {
		bool treeValid = true;

		if (root->right) {
			if (root->right->data <= root->data) {
				cout << "Error: Right node of : " << root->data << " is not greater : " << root->right->data << endl;
				treeValid = false;
			} else {
				treeValid = verifyBST(root->right);
			}
		}
		if (root->left) {
			if (root->left->data >= root->data) {
				cout << "ERROR: Left node of : " << root->data << " is not smaller : " << root->left->data << endl;
				treeValid = false;
			} else {
				treeValid = verifyBST(root->left);
			}
		}
		return treeValid;
	}

	bool handleImbalance(Node** anchor, Imbalance imbalance) {
		bool handled = true;
		switch (imbalance) {
			case Imbalance::LLLImbalance:
				handleLLImbalance(anchor);
				handleLLImbalance(anchor);
				break;
			case Imbalance::LLImbalance:
				handleLLImbalance(anchor);
				break;
			case Imbalance::LRImbalance:
				handleLRImbalance(anchor);
				break;
			case Imbalance::RRImbalance:
				handleRRImbalance(anchor);
				break;
			case Imbalance::RRRImbalance:
				handleRRImbalance(anchor);
				handleRRImbalance(anchor);
				break;
			case Imbalance::RLImbalance:
				handleRLImbalance(anchor);
				break;
			default:
				cout << "This tree is screwed. I don't understand the type of imbalance encountered." << endl;
				handled = false;
				break;
		}
		return handled;
	}

	// We get the address of grand parent of the inserted node on which to operate on.
	void handleLLImbalance(Node** gP) {
		assert(*gP);
		assert((*gP)->left);

		/***************************************************************************

                        *-100                        *-50
                       / \                          / \
                      /   \                        /   \
                     *-50  *-200  ===>            *-10  *-100
                    / \                                / \
                   /   \                              /   \
                  *-10  *-60                         *-60  *-200


		***************************************************************************/
		Node* lRight = (*gP)->left->right;
		Node* lNode = (*gP)->left;

		(*gP)->left = lRight;
		lNode->right = (*gP);
		*gP = lNode;
	}

	// We get the address of grand parent of the inserted node on which to operate on.
	void handleRRImbalance(Node** gP) {
		assert(*gP);
		assert((*gP)->right);

		/***************************************************************************

                        *-50                        *-100
                       / \                         / \
                      /   \                       /   \
                     *-10  *-100   ====>         *-50  *-110
                          / \                   / \
                         /   \                 /   \
                        *-60  *-110           *-10  *-60


		***************************************************************************/
		Node* rLeft = (*gP)->right->left;
		Node* rNode = (*gP)->right;

		(*gP)->right = rLeft;
		rNode->left = (*gP);
		*gP = rNode;
	}

	// We get the address of grand parent of the inserted node on which to operate on.
	void handleLRImbalance(Node** gP) {
		assert(*gP);
		assert((*gP)->left->right);

		/***************************************************************************
                                                       *-90
                                                      / \
                                                     /   \
                        *-100                       /     \
                       / \                         /       \
                      /   \                       /         \
                     *-50  *-200   ====>         *-50        *-200
                    / \     \                   / \         / \
                   /   \     \                 /   \       /   \
                  *-10  *-90  *-300           *-10  *-60  *-95  *-300
                       / \
                      /   \
                     *-60  *-95

		***************************************************************************/
		Node* l = (*gP)->left;
		Node* r = (*gP)->right;
		Node* lrr = (*gP)->left->right->right;
		Node* lrl = (*gP)->left->right->left;
		Node* newR = (*gP)->left->right;

		newR->right = *gP;
		newR->left = l;
		l->right = lrl;
		newR->right->left = lrr;
		*gP = newR;
	}

	// We get the address of grand parent of the inserted node on which to operate on.
	void handleRLImbalance(Node** gP) {
		assert(*gP);
		assert((*gP)->right->left);

		/***************************************************************************
                                                       *-75
                                                      / \
                                                     /   \
                        *-50                        /     \
                       / \                         /       \
                      /   \                       /         \
                     *-10  *-100   ====>         *-50        *-100
                          / \                   / \         / \
                         /   \                 /   \       /   \
                        *-75  *-200           *-10  *-60  *-80  *-200
                       / \
                      /   \
                     *-60  *-80

		***************************************************************************/

		Node* r = (*gP)->right;
		Node* l = (*gP)->left;
		Node* rll = (*gP)->right->left->left;
		Node* rlr = (*gP)->right->left->right;
		Node* newR = (*gP)->right->left;

		newR->right = r;
		newR->left = (*gP);
		newR->left->right = rll;
		r->left = rlr;
		(*gP) = newR;
	}

	void printImbalance(string& prtStr, Imbalance imbalance) {
		if (imbalance == Imbalance::None) {
			return;
		}

		cout << prtStr;
		switch (imbalance) {
		case Imbalance::LLImbalance:
			cout << "LLImbalance" << endl;
			break;
		case Imbalance::LLLImbalance:
			cout << "LLLImbalance" << endl;
			break;
		case Imbalance::LRImbalance:
			cout << "LRImbalance" << endl;
			break;
		case Imbalance::RRImbalance:
			cout << "RRImbalance" << endl;
			break;
		case Imbalance::RRRImbalance:
			cout << "RRRImbalance" << endl;
			break;
		case Imbalance::RLImbalance:
			cout << "RLImbalance" << endl;
			break;
		default:
			cout << "RLImbalance" << endl;
			break;
		}
	}

private:
	Node* root;
};
