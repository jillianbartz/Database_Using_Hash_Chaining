//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: ProfBST.cpp
//-----------------------------------------------------------------------------

#include "ProfBST.h"

#include <iostream>

using namespace std;

ProfBST::ProfBST() //constructor
{
    root = NULL;
}

ProfBST::~ProfBST() //calls the destructor helper, destructor
{
    destroyNode(root);
    root = NULL;
}

void ProfBST::destroyNode(Professor *currNode) //destuctor helper, uses recursion to delete the BST from bottom up starting at left-most bottom
{
    if(currNode == nullptr) return;
    destroyNode(currNode->left);
    destroyNode(currNode->right);
    delete currNode;
    currNode = nullptr;
 }

 /*
 Create node helper:
 Helper function called to create a new node with profId and Name info, then adds correctly into BST

 Parameters:
 newNode creates new instance of Professor and adds it into the BST
 Returns the new node 
 */

Professor* ProfBST:: createNode(string profId, string profName)
{
    Professor* newNode = new Professor;
    newNode->profId = profId;
    newNode->profName = profName;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*
Add node helper
Helper function to traverse the BST and then once it finds the correct position, it will add into the tree calling the createNode function

Parameters:
get the info of currNode (the root) and the prof ID and name
Returns the currNode that is the position on the new node added
*/

Professor* ProfBST::addNodeHelper(Professor* currNode, string profId, string profName)
{
    if(currNode == NULL){
        return createNode(profId, profName);
    }
    else if(currNode->profId < profId){
        currNode->right = addNodeHelper(currNode->right, profId, profName);
    }
    else if(currNode->profId > profId){
        currNode->left = addNodeHelper(currNode->left, profId, profName);
    }
    return currNode;
}
/*
Search key helper
helper functio to find the node that contains the prof Id, calls it recursively starting with the root then traversing using BST properties

Parameters:
currNode(root)
returns the node when found
*/

Professor* ProfBST::searchKeyHelper(Professor* currNode, string profId)
{
    if(currNode == NULL)
        return NULL;

    if(currNode->profId == profId)
        return currNode;

    if(currNode->profId > profId)
        return searchKeyHelper(currNode->left, profId);

    return searchKeyHelper (currNode->right, profId);
}

void ProfBST::addProfessor(string profId, string profName) //calls the addnodehelper function and creates the root node as so 
{
    root = addNodeHelper(root, profId, profName);
}

Professor* ProfBST::searchProfessor(string profId) //calls the search professor function and returns the node found 
{
    return (searchKeyHelper(root, profId));
}

/*
Public Search professor
finding the professor through open addressing and chaining hash tables then outputting all info on the professor

Parameters:
temp, pointer to the searchprofessor function which will find the node of professor that we want 
then calls the display professor info function to output the function
*/

void ProfBST::publicSearchProfessor(string profId) 
{
    Professor* temp = searchProfessor(profId); 
    if (temp == NULL) //if we cannot find the professor
    { 
        cout << "Professor not found." << endl;
        return;
    }
    else //we can find the professor
    {
        cout << "[OPEN ADDRESSING] Search for a professor" << endl; //outputting both like this since information is equal for both hash tables
        cout << "-----------------------------------" << endl;
        cout << "Name: " << temp->profName << endl;
        displayProfessorInfo(temp); //calling to output the professor info

        cout << "[CHAINING] Search for a professor" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Name: " << temp->profName << endl;
        displayProfessorInfo(temp); 
    }
    return;
}
/*
Display professor info
Displays all the information from a specific professor from class and traverses through it's courses vector

Parameters:
P which is the node we want to display info from
returns:
informatiom in cout
*/

void ProfBST::displayProfessorInfo(Professor* p)
{
    if (p == NULL) //if the professor is not found
    {
        cout << "Professor not found." << endl;
        return;
    }
    else
    {
        for (long unsigned int k = 0; k < p->coursesTaught.size(); k++) //traversing through the classes in the vector
        {
            cout << p->coursesTaught[k]->courseNum << ": " << p->coursesTaught[k]->courseName << ", " << p->coursesTaught[k]->year << endl;
        }
        return;
    }
}


