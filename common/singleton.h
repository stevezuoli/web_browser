#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#define SINGLETON_H(CLASSNAME) \
public:\
static CLASSNAME* GetInstance();\
static void DeleteSingleton();\
private:\
static CLASSNAME* _Instance;

#define SINGLETON_CPP(CLASSNAME) \
CLASSNAME* CLASSNAME::_Instance = NULL;\
CLASSNAME* CLASSNAME::GetInstance()\
{\
if(_Instance == NULL)\
{\
_Instance = new CLASSNAME();\
}\
return _Instance;\
}\
void CLASSNAME::DeleteSingleton()\
{\
if(_Instance != NULL)\
{\
delete _Instance;\
_Instance=NULL;\
}\
}
#endif

