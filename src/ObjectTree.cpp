#include "ObjectTree.h"
#include "Log.h"
ObjectTree::ObjectTree() {
	rootNode = NULL;
}

ObjectTree::~ObjectTree() {
	RemoveSubTree(rootNode);
}

Uint32 ObjectTree::AddObject(ObjectClass* ObjPointer) {
	if(rootNode == NULL) {
		// This object is the new root
		if((rootNode = new TreeNode(ObjPointer,1<<30)) == NULL) {
			LOG_ERROR("ObjectTree", "::AddObject(): Cannot allocate memory for new node!");
			exit(EXIT_FAILURE);
		}
		return 1<<30;
	} else {
		Uint32 MinID = 0;
		Uint32 MaxID = 1<<30;
		
		TreeNode* prevNode = rootNode;
		TreeNode* curNode;
		while(1) {
			if(rand() % 2 == 0) {
				// go left
				curNode = prevNode->leftChild;
				MaxID = prevNode->getObjectID();
				if(curNode == NULL) {
					// Add node here
					TreeNode* newNode;
					Uint32 newID =  (Uint32) ((((Uint64) MinID) + ((Uint64) MaxID)) / 2);
					if((newID <= MinID) || (newID >= MaxID) || (newID == 0)) {
						// not allowed. Try again
						return AddObject(ObjPointer);
					}
					
					if((newNode = new TreeNode(ObjPointer, newID)) == NULL) {
						LOG_ERROR("ObjectTree", "::AddObject(): Cannot allocate memory for new node!");
						exit(EXIT_FAILURE);
					}
					
					prevNode->leftChild = newNode;
					newNode->parent = prevNode;
					return newID;
				} else {
					prevNode = curNode;
				}
			} else {
				// go right
				curNode = prevNode->rightChild;
				MinID = prevNode->getObjectID();
				if(curNode == NULL) {
					// Add node here
					TreeNode* newNode;
					Uint32 newID =  (Uint32) ((((Uint64) MinID) + ((Uint64) MaxID)) / 2);
					if((newID <= MinID) || (newID >= MaxID) || (newID == 0)) {
						// not allowed. Try again
						return AddObject(ObjPointer);
					}
					
					if((newNode = new TreeNode(ObjPointer, newID)) == NULL) {
						LOG_ERROR("ObjectTree", "::AddObject(): Cannot allocate memory for new node!");
						exit(EXIT_FAILURE);
					}
					
					prevNode->rightChild = newNode;
					newNode->parent = prevNode;
					return newID;
				} else {
					prevNode = curNode;
				}				
			}
		}
	}
}

void ObjectTree::RemoveObject(Uint32 ObjectID) {
//	debug
//	fprintf(stderr,"Removing object %d from map!\n",ObjectID);
//	PrintTree();

	if(ObjectID == (Uint32)NONE) {
		return;
	}
		
	TreeNode* curNode = rootNode;
	while(curNode != NULL) {
		if(curNode->getObjectID() > ObjectID) {
			curNode = curNode->leftChild;
		} else if(curNode->getObjectID() < ObjectID) {
			curNode = curNode->rightChild;
		} else /* curNode->getObjectID() == ObjectID */ {
			// we have found it
			if((curNode->leftChild == NULL) && (curNode->rightChild == NULL)) {
				// easiest case: just remove this child
				if(curNode == rootNode) {
					rootNode = NULL;
				} else if(curNode->parent->leftChild == curNode) {
					curNode->parent->leftChild = NULL;
				} else /* curNode->parent->rightChild == curNode */ {
					curNode->parent->rightChild = NULL;
				}
				delete curNode;
				return;
			} else if(curNode->leftChild == NULL) {
				// leftChild == NULL, rightChild != NULL
				if(curNode == rootNode) {
					rootNode = curNode->rightChild;
					curNode->rightChild->parent = NULL;
				} else if(curNode->parent->leftChild == curNode) {
					curNode->parent->leftChild = curNode->rightChild;
					curNode->rightChild->parent = curNode->parent;
				} else /* curNode->parent->rightChild == curNode */ {
					curNode->parent->rightChild = curNode->rightChild;
					curNode->rightChild->parent = curNode->parent;
				}
				delete curNode;
				return;
			} else if(curNode->rightChild == NULL) {
				// leftChild != NULL, rightChild == NULL
				if(curNode == rootNode) {
					rootNode = curNode->leftChild;
					curNode->leftChild->parent = NULL;
				} else if(curNode->parent->leftChild == curNode) {
					curNode->parent->leftChild = curNode->leftChild;
					curNode->leftChild->parent = curNode->parent;
				} else /* curNode->parent->rightChild == curNode */ {
					curNode->parent->rightChild = curNode->leftChild;
					curNode->leftChild->parent = curNode->parent;
				}
				delete curNode;
				return;
			} else {
				// leftChild != NULL, rightChild != NULL
				
				// find biggest ObjectID in the left subtree
				TreeNode* tmpNode = curNode->leftChild;
				while(tmpNode->rightChild != NULL) {
					tmpNode = tmpNode->rightChild;
				}
				
				// now remove it from there
				if(tmpNode->parent->leftChild == tmpNode) {
					tmpNode->parent->leftChild = tmpNode->leftChild;
					if(tmpNode->leftChild != NULL) {
						tmpNode->leftChild->parent = tmpNode->parent;
					}
				} else /* tmpNode->parent->rightChild == tmpNode */ {
					tmpNode->parent->rightChild = tmpNode->leftChild;
					if(tmpNode->leftChild != NULL) {
						tmpNode->leftChild->parent = tmpNode->parent;
					}
				}
				
				// insert it in the place of curNode
				tmpNode->leftChild = curNode->leftChild;
				if(tmpNode->leftChild != NULL) {
					tmpNode->leftChild->parent = tmpNode;
				}
				tmpNode->rightChild = curNode->rightChild;
				if(tmpNode->rightChild != NULL) {
					tmpNode->rightChild->parent = tmpNode;
				}
				tmpNode->parent = curNode->parent;
				if(curNode == rootNode) {
					rootNode = tmpNode;
				} else /* curNode->parent != NULL */ {
					if(curNode->parent->leftChild == curNode) {
						curNode->parent->leftChild = tmpNode;
					} else {
						curNode->parent->rightChild = tmpNode;
					}
				}
				
				delete curNode;
				return;
			}
		}
	}
}

void ObjectTree::RemoveSubTree(TreeNode* node) {
	if(node == NULL)
		return;
		
	RemoveSubTree(node->leftChild);
	RemoveSubTree(node->rightChild);
	if(node == rootNode) {
		rootNode = NULL;
	}
	delete node;
}
