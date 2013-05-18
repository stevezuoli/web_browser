
#ifndef __INIFILE__
#define __INIFILE__
#include <string>
#include <map>
#include <iostream>
using namespace std;
static const long ERRORNO=0xffffffff;
#define NoKey  1
#define NoValuename 2
typedef pair <string,string> Str_Pair;
typedef map <string,string> IniKey;
typedef map <string,IniKey> IniKeyMap;
class CIniFile
{
public:
    //.ini file path
    string path;

    //list of keys in ini
    IniKeyMap keys;

    //corresponding list of keynames
    //vector<string> names;

private:
    //Read one line from file
    istream & getline( istream & is, string & str );

    //Return the index for given valuename in the specified key, if not found, return ERRORNO
    //unsigned int FindValue(int keynum, const string &valuename);

    //Return index for given key, if not found, return ERRORNO
    //unsigned int FindKey(const string &keyname);

    //public variables
public:
    //Error info 
    string error;
public:
    CIniFile();

    virtual ~CIniFile();

    //Set ini file path
    void SetPath(const string &newpath);

    //reads ini file specified using CIniFile::SetPath()
    //true for success and false for any fialure
    bool ReadFile();

    //writes data stored in class to ini file
    void WriteFile(); 

    //writes data stored in class to ini file with file header, usually are specification/comments for this file
    void WriteFile(string); 

    //deletes all stored ini data
    void Reset();

    //returns number of keys currently in the ini
    unsigned int GetNumKeys();

    //returns number of values stored for specified key
    unsigned int GetNumValues(const string &keyname);

    //gets value of [keyname] valuename =
    //overloaded to return string, int, and double,
    //returns "", or 0 if key/value not found.  Sets error member to show problem
    string GetValue(const string &keyname, const string &valuename) throw (int);
    string GetStringValue(const string &keyname, const string &valuename, const string defaultValue);
    int GetIntValue(const string &keyname, const string &valuename, const int defaultValue);
    bool GetBoolValue(const string &keyname, const string &valuename, const bool defaultValue);

    //sets value of [keyname] valuename =.
    //specify the optional paramter as false (0) if you do not want it to create
    //the key if it doesn't exist. Returns true if data entered, false otherwise
    //overloaded to accept string, int, and double
    bool StoreStringValue(const string &key, const string &valuename, const string &value, bool create = 1);
    bool StoreIntValue(const string &key, const string &valuename, int value, bool create = 1);
    bool StoreBoolValue(const string &key, const string &valuename, bool value, bool create = 1);

    //deletes specified value
    //returns true if value existed and deleted, false otherwise
    bool DeleteValue(const string &keyname, const string &valuename);

    //deletes specified key and all values contained within
    //returns true if key existed and deleted, false otherwise
    bool DeleteKey(const string &keyname);
};

#endif
