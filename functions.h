#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;
//functions.h for certain functions that need different implementation
//for both functions.cpp and shell.cpp
// Only difference between a file and folder is isDir
/*
Basic properties:
Permissions -> Array of strings, size of 4, the first is the directory flag
Owner -> string
Group -> string
size -> int
Date -> Time and Date (use tm)
Access -> Read, Write, and Execute
Classes -> Owner, Group, Public
command Interprates
  When user enters command -> gets the entire string
  Read the first word to check for valid commands, if not return "invalid command statement"
  Check for -l and ls
  when entering a valid commands, holds the last part of it for use in the command functions
    Call the function that was inputed
      Pass in any flags
      Modify any files and directories that need to do so
*/
class Functions
{
    public:
    Functions() {}
    Functions(const string& name, const tm& date_modified, const string& owner, const int& size, const bool is_directory):
                    structname(name), dateMod(date_modified), own(owner), asize(size), isDir(is_directory)
                    {
                        if(is_directory)
                        {
                            access.insert(pair<string, string> ("dir", "d"));
                        }
                        else
                        {
                            access.insert(pair<string, string> ("dir", "-"));
                        }
                        access.insert(pair<string, string> ("owner", "rw-"));
                        access.insert(pair<string, string> ("group", "rw-"));
                        access.insert(pair<string, string> ("public", "r--"));
                    }
    ~Functions() {}
    void addFile(const string& file_name, const bool _is_dir); //mkdir
    bool RemoveFile(const string& file_name); //rm or rmdir
    bool isDirectory(); //check if something is in the directory
    void modPerm(const string& code, const string& name); //modify the permission
    void modTime(const string& file_name); //modify time stamp
    map<string, Functions>& getContents(); //get files and directories
    friend ostream& operator<<(ostream& os, const Functions& structure); //used for modifying permission and time

    protected:
    int asize;
    bool isDir;
    string structname;
    string own;
    map<string, string> access;
    map<string, Functions> contents;
    tm dateMod;

};
#endif //FUNCTIONS_H
