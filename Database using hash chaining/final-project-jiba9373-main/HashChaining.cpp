//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashChaining.cpp
//-----------------------------------------------------------------------------

#include "HashChaining.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

HashChaining::HashChaining(int size) //constructor, creating a new dynamic array and assigning all the elements to point to null until filled
{
    hashTableSize= size;
    hashTable = new Course*[hashTableSize];
    for(int i=0;i<size;i++)
        hashTable[i] = nullptr;
}

HashChaining::~HashChaining() //deconstructor, traversing through the entire table and then deleting the elements through traversing through the linked list since it is chaining
{
    if (hashTableSize > 0)
    {
        for(int i = 0; i < hashTableSize; i++) //traversing through the hash table
        {
            if (hashTable[i]) //if the table has elements and is not null
            {
                Course *head = hashTable[i];
                Course *del;
                while (head->next != NULL) //traversing through the linked list of the hashtable[posiiton]
                {
                    del = head;
                    head = head->next;
                    delete del;
                    del = NULL;
                }
                delete head;
                head = NULL;
            }  
        }
        delete[] hashTable; //delete the entire hashtable
    }
}

int HashChaining::hash(int courseNumber) //finding the original position an element could be inseted in 
{
    return (courseNumber % hashTableSize);
}
/*
Bulk Instert
Inserting all the information from the file successfully into the hash table as well as populating the BST

Parameters:
search/collision to keeptrack of amount needed to find position for node 
myFile to open the file through the filename
year,department,coursenum,coursename,profid,proffirst,proflast,line to get all information from the file
tempInst and prev, pointers to input and call the professor course and the previous to count and insert into the table
*/

void HashChaining::bulkInsert(string filename)
{
    int search = 0;
    int collision = 0;
    ifstream myfile;
    myfile.open(filename);
    if (myfile.is_open()) //if the file is able to open
    {
        //year,department,course_num,course_name,prof_id,prof_fname,prof_lname
        string yearStr;
        string department;
        string courseNumStr;
        string courseName;
        string profId;
        string profFirst;
        string profLast;
        string line;
        int year = 0;
        int courseNum = 0;
        Professor *tempInst = nullptr;
        Course* prev = nullptr;
        
        getline(myfile, line); //getting the first line out of the way, since it doesn't have any important info
        while (getline(myfile, yearStr, ',') && getline(myfile, department, ',') && getline(myfile, courseNumStr, ',') && getline(myfile, courseName, ',') && getline(myfile, profId, ',') && getline(myfile, profFirst, ',') && getline(myfile, profLast))
        {
            string profName = profFirst + " " + profLast; //getting profname all together
            year = stoi(yearStr);
            courseNum = stoi(courseNumStr);
            Course* temp = new Course(year, department, courseNum, courseName, nullptr); //making new instance of the course with information
            if (profDb.searchProfessor(profId) != NULL) //if you cannot find the professor in the BST, then insert it
            {
                tempInst = profDb.searchProfessor(profId);
                tempInst->coursesTaught.push_back(temp);
            }
            if (profDb.searchProfessor(profId) == NULL) //if you find the professor in the BST, then just add the course into the vector
            {
                profDb.addProfessor(profId, profName);
                tempInst = profDb.searchProfessor(profId);
                tempInst->coursesTaught.push_back(temp);
            }
            temp->prof = profDb.searchProfessor(profId); //searching for professor so we can input into the hash table
            int index = hash(courseNum);
            if (hashTable[index] == NULL) //if the position is empty, we insert immediatebly
            {   
                hashTable[index] = temp;
            }
            else //if not empty, we need to check for collisions and traverse
            {
                collision++;
                search++;
                prev = hashTable[index];
                while (prev->next != NULL) //adding into LL since this is chaining
                {
                    search++;
                    prev = prev->next;
                }
                prev->next = temp;
                temp->next = NULL;
                temp->previous = prev;
            }
        }
    myfile.close(); //close the file
    }
    cout << "[CHAINING] Hash table populated" << endl; 
    cout << "----------------------------" << endl;
    cout << "Collisions using chaining: " << collision << endl;
    cout << "Search operations using chaining: " << search << endl << endl;
}

/*
Hash Chaining searching
searches for the course using the year, number and id

Parameters: courseyear, num, profid
Index, to find positiom
counter to find the amount of times we need to traverse through the hash table to find the course information
*/
void HashChaining::search(int courseYear, int courseNumber, string profId)
{
    cout << "[CHAINING] Search for a course" << endl;
    cout << "---------------------------" << endl;
    
    int index = hash(courseNumber);
    int counter = 0;
    Course* head = hashTable[index]; //making instanece of the info by searching for it through the table
    if(head->year == courseYear && head->prof->profId == profId && head->courseNum == courseNumber)
    {
        counter++;
        cout << "Search operations using chaining: " << counter << endl;
        displayCourseInfo(head);
        return;
    }
    counter++;
    while(head != NULL) //checking and finding where the position is of the information through traversing
    {
        if (head->year == courseYear && head->prof->profId == profId && head->courseNum == courseNumber)
        {
            cout << "Search operations using chaining: " << counter << endl;
            displayCourseInfo(head);
            return;
        }
        head = head->next;
        counter++;
    }
    cout << "Course not found." << endl;
    return;
}

void HashChaining::displayAllCourses() //outputting all courses through loops of loop from array and then looping through entire LL
{
    Course *temp = nullptr;
    for (int i = 0; i < hashTableSize; i++) //getting through the hasht table
    {
        if (hashTable[i]) //if the position node exists
        {
            temp = hashTable[i];
            displayCourseInfo(temp);
            while (temp->next) //going through chains in position
            {
                temp = temp->next;
                displayCourseInfo(temp);
            }
        }
    }
}

void HashChaining::displayCourseInfo(Course* c) //displaying the information of a specific node in the table
{

    cout << c->year << " " << c->courseName << " " << c->courseNum << " " << c->prof->profName << endl;
    return;
}