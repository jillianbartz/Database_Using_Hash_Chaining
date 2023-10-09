//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: main.cpp
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"
#include "HashChaining.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main (int argc, char* argv[])
{
    //checking to make sure we have all arguments
    if (argc < 3)
    {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size>" << endl;
        return -1;
    }

    //initializing all arguments
    string inFile = argv[1];
    string inSize = argv[2];
    int inSizeInt = stoi(inSize);

    //creating hashtables using hash objects as parameter
    HashChaining chaining(inSizeInt);
    HashOpenAddressing quad(inSizeInt);
    
    /*
    MENU BELOW:
    Ask and populate the hash table as needed

    parameters:
    Using objects for hash table created above
    Option choice in int/string
    getting info for option 2 or 3 for year, course number, professor id
    Temporary object for the BST to call in option 3

    Exit:
    exit using option 5, automatically calls the destructors
    */
    string choiceStr;
    START:
    cout << "=======Main Menu=======" << endl;
    cout << "1. Populate hash tables" << endl;
    cout << "2. Search for a course" << endl;
    cout << "3. Search for a professor " << endl;
    cout << "4. Display all courses " << endl;
    cout << "5. Exit" << endl;
    cout << endl;
    cout << "Enter an option: ";
    cin >> choiceStr;
    cout << endl;

    int choice = stoi(choiceStr);
    string yearStr;
    string courseNumStr;
    int courseNum;
    int year;
    string profId;
    ProfBST temp;

    switch(choice)
    {
        case 1:  //bulk inserting for both hash files, must call this before using any other option
        quad.bulkInsert(inFile);
        chaining.bulkInsert(inFile);
        goto START; // go back to start
        break;

        case 2: //searching the course using info asked, calling the search function for both hash classes/structs
        cout << "Enter the course year (e.g. 2021)" << endl;
        cin >> yearStr;
        year = stoi(yearStr);
        cout << "Enter a course number (e.g. 2270)" << endl;
        cin >> courseNumStr;
        courseNum = stoi(courseNumStr);
        cout << "Enter a Professor's ID (e.g. llytellf)" << endl;
        cin >> profId;
        quad.search(year, courseNum, profId);
        chaining.search(year, courseNum, profId);
        goto START; //go back to start
        break;

        case 3: //searching for professor by searching the BST
        temp = chaining.profDb;
        cout << "Enter a Professor's ID (e.g. nscollan0)" << endl;
        cin >> profId;
        temp.publicSearchProfessor(profId); //searching the BST using info found
        goto START; //go back to start
        break;

        case 4: //displaying all
        char dec;
        cout << "Which hash table would you like to display the courses for (O=Open Addressing, C=Chaining)?" << endl;
        cin >> dec;
        if (dec == 'O') //calling the open addressing display
        {
            quad.displayAllCourses(); 
        }
        else if (dec == 'C') //calling the chaining display
        {
            chaining.displayAllCourses();
        }
        goto START; //go back to start
        break;

        case 5: //automatically calls destructors and exits the file
        return 0;
        break;

        default: //in case user inputs something outside of 1-5 scope
        goto START;
    }

    

    return 0;
}