#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Functions.h"
/*
GetCode for the number codes in from of the file/Directory
ParseCode: parses the code that's given to the file/directory
Definitions the functions from functions.h and namespace for helper functions
Using overload operator to modify permission and time
*/
namespace //namespace for helper functions, mainly for the permission code
{
    // Mapping each number from 0-7 for constant mapping of the permission code.
    string GetCode(const char num)
    {
        map<char, string> num_codes =
        {
            {'0', "---"},
            {'1', "--x"},
            {'2', "-w-"},
            {'3', "-wx"},
            {'4', "r--"},
            {'5', "r-x"},
            {'6', "rw-"},
            {'7', "rwx"}

        };

        if(num == '8' || num == '9')   // Default values for any digit that isn't in the 0-7 range
        {
            //cout<<"Detecting numbers that are not in the 0-7 range";
            return num_codes['0'];
        }
        else if (isalpha(num))
        {
            return num_codes['0'];
        }
        else
        {
            return num_codes[num];
        }
    }

    // Parsing the code given to the file/directory
    void parseCode(const string& code, map<string, string>& access)
    {
        const vector<string> permission_groups = {"owner", "group", "public"};
        for (unsigned short i = 0; i < code.length(); i++)
        {
            access.at(permission_groups[i]) = GetCode(code[i]);
        }
    }
}

// mkdir for newly created files
// Takes the file_name as well as the flag of whether or not it's a directory.
void Functions::addFile(const string& file_name, const bool is_dir)
{
    time_t now = time(0);
    tm *curr_time = localtime(&now);
    const auto rand_size = (rand() % 1024) + 1;
    Functions new_file(file_name, *curr_time, "user", rand_size, is_dir);
    contents.insert(pair<string, Functions> (file_name, new_file));
}

// rm or rmdir
bool Functions::RemoveFile(const string& file_name)
{
    return contents.erase(file_name);
}

// Is the file in a directory
bool Functions::isDirectory()
{
    return isDir;
}

// 'chmod' Takes the three digit code along with the file name
//Takes these and modify the permission
void Functions::modPerm(const string& code, const string& name)
{
  for (const auto digit : code)
  {
    if (!isdigit(digit))
      {
        cout << "chmod: invalid mode: '" << code << "' " << endl;
        return;
      }
  }
  parseCode(code, contents[name].access);
}

// 'touch' used for files that already exist and need the time updated
void Functions::modTime(const string& file_name)
{
    time_t now = time(0);
    tm *curr_time = localtime(&now);
    contents[file_name].dateMod = *curr_time;
}


// Gets the contents of the current file directory
map<string, Functions>& Functions::getContents()
{
  return contents;
}

// Overloading the << operater for accessing type and time
// For the 'ls -l' command
ostream& operator<<(ostream& os, const Functions& structure)
{
    map<int, string> months=
    {
        {0, "Jan"},
        {1, "Feb"},
        {2, "Mar"},
        {3, "Apr"},
        {4, "May"},
        {5, "Jun"},
        {6, "Jul"},
        {7, "Aug"},
        {8, "Sep"},
        {9, "Oct"},
        {10,"Nov"},
        {11,"Dec"}
    };
    os << structure.access.at("dir");
    os << structure.access.at("owner");
    os << structure.access.at("group");
    os << structure.access.at("public");
    os << setw(2) << structure.contents.size()+1;
    os << setw(5) << structure.own;
    os << setw(5) << structure.own;
    os << setw(5) << structure.asize << " ";
    os << months[structure.dateMod.tm_mon] << " ";
    os << structure.dateMod.tm_mday << " ";
    os << setw(1) << structure.dateMod.tm_hour << ":" << structure.dateMod.tm_min;
    return os;
}
