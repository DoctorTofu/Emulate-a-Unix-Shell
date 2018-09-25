//Andy Wang
//shell.cpp
//Emulate a Unix Shell Assignment
//Rather than using pointers, used <map> and vectors
#include <string>
#include <ctype.h>
#include <sstream>
#include <map>
#include <vector>
#include "Functions.h"
#include "Functions.cpp"

using namespace std;

string getCommand(); //allows user to enter the command
vector<string> splitString(const string& command); //splits user input i.e. "command: cd, mkdir, rm, rmdir, etc" with whatever follows after it.
void parseCom(const string& command, map<vector<string>, Functions>& structure, vector<string>& path); //where are the commands and the necessary actions take place
void printList(Functions& structure, bool list_long);   //Print the full contents of the files and/or directories
void printWorkingDirectory(const vector<string>& path); // Directory contents are stored in a vector
void dirChange(vector<string>& curr_dir, const string& new_dir, Functions& files); //Function used to parse the 'cd' function

int main ()
{
    time_t now = time(0);
    srand(now);
    tm *curr_time = localtime(&now);
    const auto rand_size = (rand() % 1024) + 1;

    Functions root("root", *curr_time, "user", rand_size, true);
    map<vector<string>, Functions> fileForm;
    fileForm.insert(pair<vector<string>, Functions> ({"root"}, root));
    vector<string> last_path = {"root"};
    string command;

    do
    {
      cout << "\nbash$ "; //initial display when using shell
      command = getCommand(); //user enters command use getCommand function to get the input
      parseCom(command, fileForm, last_path); //takes the input of the command calls parseCom
    }while (command != "quit" && command != "exit"); //leave loop when user enters "quit" or "exit"

    return 0;
}

// Gets the command
string getCommand() //user inputs the command and function gets it
{
    string command;
    getline(cin, command);
    return command;
}

// Converting the line taken in from cin and converting it to a string stream
// This stream is split by a space to get the command and the word after it
vector<string> splitString(const string& command)
{
    stringstream ss(command);
    string word;
    vector<string> commandStr;
    while (getline(ss, word, ' ')) //space
    {
      commandStr.push_back(word);
    }
    return commandStr;
}

//Command parsing, where interpretation and error handling occurs
//Basically where all the specific actions for each command
void parseCom(const string& command, map<vector<string>, Functions>& structure, vector<string>& dir)
{
    vector<string> commandAction = splitString(command);
    if(commandAction[0] == "cd") //cd command
    {
        if(commandAction.size() > 1)
        {
            if(commandAction[1] == ".." || commandAction[1] == "../" ||
                 structure[dir].getContents().count(commandAction[1]))
            {
                dirChange(dir, commandAction[1], structure[dir]);
            }
            else
            {
                cout << "cd: " << commandAction[1] << " No such file or directory." << endl;
            }
        }
    }
    else if(commandAction[0] == "mkdir") //mkdir command
    {
        if(commandAction.size() > 1)
        {
            if(structure[dir].getContents().count(commandAction[1]))
            {
                cout << "mkdir: cannot create directory '" << commandAction[1]
                     << "': File exists" << endl;
            }
        }
        structure[dir].addFile(commandAction[1], true);
    }
    else if(commandAction[0] == "rm" || commandAction[0] == "rmdir") // rm and rmdir command
    {
        if(commandAction.size() == 2)
        {
            map<string, Functions> files = structure[dir].getContents();
            if (files.count(commandAction[1]))
            {
                if (structure[dir].RemoveFile(commandAction[1]) == 0)
                {
                    cout << commandAction[0] << ": " << commandAction[1] <<
                    "No such file or directory" << endl;
                }
            }
            else
            {
                cout << "No such file or directory" << commandAction[1] << endl;
            }
        }
        else
        {
            cout << commandAction[0] << ": missing operand" << endl;
        }
    }
    else if(commandAction[0] == "ls") //ls command
    {
        if(commandAction.size() == 2)
        {
            if(commandAction[1] == "-l")
            {
                printList(structure[dir], true);
            }
            else
            {
                cout << "Invalid command; Invalid flag." << endl;
            }
        }
        else
        {
            printList(structure[dir], false);
        }
    }
    else if(commandAction[0] == "touch") //touch command
    {
        if(commandAction.size() > 1)
        {
            if(structure[dir].getContents().count(commandAction[1]))
            {
                structure[dir].modTime(commandAction[1]);
            }
            else
            {
                structure[dir].addFile(commandAction[1], false);
            }
        }
        else
        {
            cout << "Invalid operation; Please enter a file name." << endl; //error message
        }
    }


    else if(commandAction[0] == "chmod") //chmod command
    {
        if(commandAction.size() > 2)
        {
            if(structure[dir].getContents().count(commandAction[2]))
            {
                structure[dir].modPerm(commandAction[1], commandAction[2]);
            }
            else
            {
                cout << "chmod:" << " cannot access '" << commandAction[1]
                     << "' : No such file or directory" << endl;
            }
        }
    }

    else if(commandAction[0] == "pwd")  //pwd command
    {
        printWorkingDirectory(dir);
    }
    else if(commandAction[0] != "exit" && commandAction[0] != "quit") //other invalid commands
    {
        cout << "Command " << commandAction[0] << " not found.\n";
    }
}

// Listing the structure of the directory.
// The list_long flag is used to print the full contents of the files and/or directories
void printList(Functions& structure, bool list_long)
{
    for (auto& file : structure.getContents())
    {
        if (!list_long)
        {
            cout << file.first << " ";
        } else if (list_long) {
            if (file.second.isDirectory())
            {
                cout << file.second << " " << setw(3) << file.first << "/" << endl;
            }
            else
            {
                cout << file.second << " " << setw(3) << file.first << endl;
            }
        }
    }
}
// Directory contents are stored in a vector
void printWorkingDirectory(const vector<string>& path) //this is for pwd
{
    string path_name = "";
    for (const auto& dir : path)
    {
      path_name += "/" + dir;
    }
    cout << path_name << "/";
}

// Function used to parse the 'cd' command.
// Checking to see if it is .. or ../ before checking anything else.
// All directories are mapped to the original map and when the directory level
// changes, the vector changes as well
void dirChange(vector<string>& curr_dir, const string& new_dir, Functions& files)
{
    if(new_dir == ".." || new_dir == "../")
    {
        if(curr_dir.size()>1)
        {
            curr_dir.pop_back();
        }
    }
    else
    {
        if(files.getContents()[new_dir].isDirectory())
        {
            curr_dir.push_back(new_dir);
        }
        else
        {
            cout << "cd: " << new_dir << ": No such file or directory" << endl;
        }
    }
}
