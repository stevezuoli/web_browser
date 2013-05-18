#include "ModuleCfg.h"
#include <iostream>
#include <stdio.h>

using namespace std;

ModuleCfg::~ModuleCfg(){
}

bool ModuleCfg::isSaveToFile(){
    if(!this->enabled) return false; //When "Enabled" was set in .ini file, it only enabled console output defaultly.
    return !this->filename.empty();
}
bool ModuleCfg::isConsoleOutput(){
    if(!this->enabled) return false;
    return this->consoleOutput;
}

void ModuleCfg::print(){
    std::cout<<"=============================================="<<endl;
    std::cout<<"Module name="+this->getModuleName()<<", Enabled="<<this->isEnabled()<<", consoleOutput="<<consoleOutput<<", File="<<filename<< std::endl;
}

void ModuleCfg::setLogFile(string filename){
    this->filename = filename;
}

