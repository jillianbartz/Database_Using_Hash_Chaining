//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashOpenAddressing.cpp
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"

using namespace std;

HashOpenAddressing::HashOpenAddressing(int size)//constructor, creating a new dynamic array and assigning all the elements to point to null until filled
{
    this->hashTableSize= size;
    hashTable = new Course*[hashTableSize];
    for(int i=0;i<size;i++)
        hashTable[i] = nullptr;
}

HashOpenAddressing::~HashOpenAddressing() //destructor, traverse through the entire dynamic array and deleting all elements inside it
{
    if(hashTableSize > 0)
    {
        for (int i = 0; i < hashTableSize; i++) //traversing
        {
            if (hashTable[i]) //if exists
            {
                Course *head = hashTable[i];
                delete head;
                head = NULL;
            }
        }
        delete[] hashTable; //delete the entire table
    }
}

int HashOpenAddressing::hash(int courseNumber) //finding the original position an element could be inseted in 
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
tempInst and temp, pointers to input and call the professor course and the previous to count and insert into the table
*/

void HashOpenAddressing::bulkInsert(string filename)
{
   int search = 0;
    int collision = 0;
    ifstream myfile;
    myfile.open(filename);
    if (myfile.is_open()) //checking if file is open
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
        Course* temp = nullptr;
        Professor *tempInst = nullptr;
        getline(myfile, line);
        while (getline(myfile, yearStr, ',') && getline(myfile, department, ',') && getline(myfile, courseNumStr, ',') && getline(myfile, courseName, ',') && getline(myfile, profId, ',') && getline(myfile, profFirst, ',') && getline(myfile, profLast))
        {
            string profName = profFirst + " " + profLast; //getting profname all together
            year = stoi(yearStr); 
            courseNum = stoi(courseNumStr);
            temp = new Course(year, department, courseNum, courseName, nullptr); //making new instance of the course so we can add into table
            if (profDb.searchProfessor(profId) != NULL) ///if you cannot find the professor in the BST, then insert it
            {
                tempInst = profDb.searchProfessor(profId);
                tempInst->coursesTaught.push_back(temp);
            }
            if (profDb.searchProfessor(profId) == NULL)//if you find the professor in the BST, then just add the course into the vector
            {
                profDb.addProfessor(profId, profName);
                tempInst = profDb.searchProfessor(profId);
                tempInst->coursesTaught.push_back(temp);
            }
            temp->prof = profDb.searchProfessor(profId);//searching for professor so we can input into the hash table
            int index = hash(courseNum);
            int i = 0;
            
            if (hashTable[index] == NULL) //if the position is empty, we insert immediatebly
            {   
                hashTable[index] = temp;
            }
            else //if not empty, we need to check for collisions and traverse
            {
                collision++;
                i++;
                while(hashTable[index]) //quadratic probing 
                {
                    search++;
                    index = (index + i * i) % hashTableSize;
                    i++;
                }
                hashTable[index] = temp;
            }
        }
    myfile.close(); //closing the file
    }
    cout << "[OPEN ADDRESSING] Hash table populated" << endl;
    cout << "----------------------------" << endl;
    cout << "Collisions using chaining: " << collision << endl;
    cout << "Search operations using chaining: " << search << endl << endl;
}

/*
Hash OA searching
searches for the course using the year, number and id

Parameters: courseyear, num, profid
Index, to find positiom
counter to find the amount of times we need to traverse through the hash table to find the course information
*/

void HashOpenAddressing::search(int courseYear, int courseNumber, string profId)
{
    cout << "[OPEN ADDRESSING] Search for a course" << endl;
    cout << "---------------------------" << endl;
    int counter = 0;
    int i = 0;
    int index = hash(courseNumber);
    i++;
    while(hashTable[index]) //when the info exists in the table
    {
        counter++;
        Course* head = hashTable[index]; 
        if (head->year == courseYear && head->prof->profId == profId && head->courseNum == courseNumber) //if all info is correct
            {
                cout << "Search operations using chaining: " << counter << endl;
                displayCourseInfo(head);
                return;
            }
        index = (index + i * i) % hashTableSize; //using the quad probing again to continue to search it
        i++;
        counter++;
    }
    cout << "Course not found." << endl;
    return;
}

void HashOpenAddressing::displayAllCourses() //displaying entire table through travrsing through array and calling the info
{
    for (int i = 0; i < hashTableSize; i++) //traversing through
    {
        if (hashTable[i]) //if containing info
        {
            Course *temp = hashTable[i];
            displayCourseInfo(temp);
        }
    }
}

void HashOpenAddressing::displayCourseInfo(Course* c) //displaying the information of a specific node in the table
{
	cout << c->year << " " << c->courseName << " " << c->courseNum << " " << c->prof->profName << endl;
    return;
}