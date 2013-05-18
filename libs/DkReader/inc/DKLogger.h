#ifndef __DKLOGGER__
#define __DKLOGGER__
#include "Mutex.h"
#include "ModuleCfg.h"
#include <string>
#include <vector>
#include "dkBaseType.h"
using std::vector;
using std::string;

class DKLogger: public MutexBase{
public:

    /// Get current thread id
    string getCurrentThreadID();

    /// Get current time for writing logs into file
    string getTime();

    /// Get timestamp which will be used in a file name
    string getTimeStamp();

    ///Write logs into files and output them on console 
    void log(string s, string moduleName);
    int ForceLog(string filename, LPCSTR format, ... );

    /// Set log file for specific module
    void setModuleLogFile( const string moduleName,const string file);

    /// Enable or Disable log output for specified module
    void setEnabled(string, bool);

    /// Get current working directory
    string getWorkingDir();

    string refineLogFile(string filepath, const string modulename);

    string autoCreatedModuleLogFile(string module);

    /// Get given module's log file handler
    string getModuleLogFile(string);

    /// Get specific module 
    ModuleCfg* getModule(string moduleName);

    static DKLogger* getInstance();

    bool isFileExists( string const absFilename);
    void retriveFileParent(string const abcFilepath, string *parentFolder);
    void retriveFilename(string const abcFilepath, string *filename);
    bool createFolder(string const folder);
    void addModuleCfg(string moduleName, bool enabled);
    void addModuleCfg(string moduleName, bool enabled, bool consoleOutput);
    void addModuleCfg(string moduleName, bool enabled, bool consoleOutput, string logFile);
    void saveModuleCfgsToIni();

    void printModules();
    static void InitLogger();

private:
    DKLogger();
    DKLogger(const DKLogger &);
    DKLogger operator = (const DKLogger &);
    ~DKLogger();

protected:
    static DKLogger *instance;
    vector<ModuleCfg*> modules;
private:
    bool anyModuleCfgChanged;
};

#endif

