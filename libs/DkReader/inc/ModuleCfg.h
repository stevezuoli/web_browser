#ifndef __MODULECFG__
#define __MODULECFG__
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
using std::string;
using std::vector;
#ifdef WIN32
using std::FILE;
#endif

#define LoggerCfgIni  "LoggerCfg.ini"
#define Enabled "Enabled"
#define ConsoleOutput  "ConsoleOutput"
#define LogFile "LogFile"
#define LOGExtenstion ".log"

class ModuleCfg{
public:
    ModuleCfg(string _moduleName,string _filename, bool _consoleOutput=true,
        bool _enabled=true): moduleName(_moduleName), enabled(_enabled), consoleOutput(_consoleOutput), filename(_filename){
    };

    ~ModuleCfg();
    bool isEnabled(){ return enabled;}
    void setEnabled( bool _enabled) { 
        enabled = _enabled; 
        consoleOutput = enabled;//Currently, let's make consoleOutput same with enabled!
    }

    void setLogFile(string filename);
    void setConsoleOutput(bool outputToConsole){ consoleOutput = outputToConsole;};

    bool isSaveToFile();
    bool isConsoleOutput();
    void print();
    string getModuleName(){
        return moduleName;
    }
    string getLogFile() { return filename; }
private:
    string moduleName;
    bool enabled;
    bool consoleOutput;
    string filename;
};

#endif
