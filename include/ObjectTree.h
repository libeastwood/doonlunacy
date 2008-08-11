#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include "Definitions.h"
#include "ObjectClass.h"
#include "Log.h"

/// This class holds a tree with all objects (structures and units) in the game.
class ObjectTree
{
private:

	/// A structure for representing one node on the tree.
	class TreeNode
	{
	private:
		ObjectClass *ObjPointer;	//<	Pointer to the object
		Uint32 ObjectID;			//< ID of this object
	public:
		/*!
			Constructor for one node
			@param	ObjPointer	Pointer to a object
			@param	ObjectID	ID of this object
		*/
		TreeNode(ObjectClass *ObjPointer, Uint32 ObjectID) {
			this->ObjPointer = ObjPointer;
			this->ObjectID = ObjectID;
			leftChild = rightChild = parent = NULL;
		}
		
		/*!
			Get the object ID of this node.
			@return ObjectID of this node
		*/
		inline Uint32 getObjectID() { return ObjectID; };
		
		/*!
			Get a pointer to the object of this node
			@return Pointer to the object of this node
		*/		
		inline ObjectClass* GetObjPointer() { return ObjPointer; };
		
		TreeNode* leftChild;	//< left child node of this node
		TreeNode* rightChild;	//< right child node of this node
		TreeNode* parent;		//< parent node of this child
	};
	
public:
	/*!
		Default constructor
	*/
	ObjectTree();
	
	/*!
		Default destructor
	*/
	~ObjectTree();
	
	/*!
		This method adds one object to the tree. The ObjectID is choosen automatically.
		@param	ObjPointer	A pointer to the object.
		@return ObjectID of the added object.
	*/
	Uint32 AddObject(ObjectClass* ObjPointer);
	
	/*!
		This method searches the tree for the object with ObjectID.
		@param	ObjectID		ID of the object to search for
		@return Pointer to this object (NULL if not found)
	*/
	inline ObjectClass* getObject(Uint32 ObjectID)
	{
		if(ObjectID == (Uint32)NONE) {
			return NULL;
		}
		
		TreeNode* curNode = rootNode;
		while(curNode != NULL) {
			if(curNode->getObjectID() > ObjectID) {
				curNode = curNode->leftChild;
			} else if(curNode->getObjectID() < ObjectID) {
				curNode = curNode->rightChild;
			} else /* curNode->getObjectID() == ObjectID */ {
				return curNode->GetObjPointer();
			}
		}
//		fprintf(stderr,"WARNING: ObjectTree::getObject(): Cannot find object with ID %d!\n",ObjectID);
		return NULL;
	};
	
	/*!
		This method removes one object from the tree.
		@param	ObjectID		ID of the object to remove
		@return none
	*/
	void RemoveObject(Uint32 ObjectID);
	
	/*!
		This method prints the complete tree on stdout. It is only needed for debuging.
		While printing the tree is checked for node->Child->parent != node.
		@return none
	*/
	void PrintTree() {
		PrintSubTree(rootNode,0);
	}
	
private:
	/*!
		Helper method. Removes the complete subtree with node as root.
		@param	node	The root of the subtree.
		@return none
	*/
	void RemoveSubTree(TreeNode* node);
	
	/*!
		Helper method for saving a complete subtree.
		@param	stream	Stream to save to
		@param	node	root node of this subtree
		@return none
	*/
	void PrintSubTree(TreeNode* node,int indend) {
		if(node == NULL)
			return;
		
		// consistency check of the node
		if(node->getObjectID() != node->GetObjPointer()->getObjectID()) {
			LOG_WARNING("ObjectTree", "This node is not consistent (%d != %d)!",node->getObjectID(),node->GetObjPointer()->getObjectID());
		}
		
		// consistency check of the tree
		if(((node->leftChild != NULL) && (node->leftChild->parent != node))
			|| ((node->rightChild != NULL) && (node->rightChild->parent != node))) {
			LOG_WARNING("ObjectTree", "This tree is not consistent!");
		}
		
		PrintSubTree(node->leftChild,indend+2);
		for(int i=0; i < indend; i++) {
			printf(" ");
		}
		LOG_INFO("ObjectTree", "%d",node->getObjectID());
		PrintSubTree(node->rightChild,indend+2);		
	}
	
	TreeNode *rootNode;		//< root node of this tree
};

#endif // OBJECTTREE_H
