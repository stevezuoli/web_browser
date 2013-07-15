#include "ime/IMEManager.h"
#include "pthread.h"

#define IME_NEW(CLASSNAME,SHROTNAME)        \
    Imes[IME_NAME_##CLASSNAME] = new CLASSNAME(); \
    ImesShortNames[IME_NAME_##CLASSNAME] = SHROTNAME;\


#define NO_IME -1

IIMEBase* IMEManager::Imes[IME_COUNT];
int IMEManager::iIndex = NO_IME;
bool IMEManager::fIsInit= false;
const char* IMEManager::ImesShortNames[IME_COUNT]={0};
extern signed char iIMIndex;
pthread_t pThreadIme;

void* IMEManager::_init_ime_data(void*)
{
    IME_LIST(IME_NEW);
    iIndex = NO_IME;
    for(int i=0;i<IME_COUNT;i++)
    {
        if(Imes[i])
        {
            Imes[i]->Init();
        }
    }

    fIsInit = true;
    return NULL;
}

void IMEManager::Init()
{
    static bool fFirst = true;
    if (fFirst)
    {
        fFirst = false;
        // pthread_create(&pThreadIme, NULL, _init_ime_data, NULL);
        pthread_create(&pThreadIme, NULL, _init_ime_data, NULL);
    }
    else
    {
        if(fIsInit == false)
        {
            pthread_join(pThreadIme, NULL);
        }
    }

};
//IIMEBase* IMEManager::GetNext()
//{
    //while(!fIsInit)
    //{
        //sleep(1);
    //}
    //if(iIndex != -1)
    //{
        //Imes[iIndex]->Reset();
    //}
    //iIndex++;
    //if(iIndex == 0 && SystemSettingInfo::GetInstance()->m_IME.GetOptionIME() / 2 % 2 == 0)
    //{
        //iIndex ++;
    //}
    //if(iIndex == 1 && SystemSettingInfo::GetInstance()->m_IME.GetOptionIME() / 4 % 2 == 0)
    //{
        //iIndex ++;
    //}
    //if(iIndex == 2 && SystemSettingInfo::GetInstance()->m_IME.GetOptionIME() / 8 % 2 == 0)
    //{
        //iIndex ++;
    //}
    //if(iIndex == 3 && SystemSettingInfo::GetInstance()->m_IME.GetOptionIME() / 16 % 2 == 0)
    //{
        //iIndex ++;
    //}
    //if(iIndex == 4 && SystemSettingInfo::GetInstance()->m_IME.GetOptionIME() / 32 % 2 == 0)
    //{
        //iIndex ++;
    //}
    //if(iIndex >=IME_COUNT)
    //{
        //iIndex = NO_IME;
        //return NULL;
    //}
    //else
    //{
        //Imes[iIndex]->Reset();
        //return Imes[iIndex];
    //}

//}
int IMEManager::GetImeCount()
{
    return IME_COUNT;
}
int IMEManager::GetCurImeIndex()
{
    // TODO:
    return iIndex;
}
const char* IMEManager::GetCurImeShortName()
{
    if(iIndex>=0 && iIndex< IME_COUNT)
    {
        return ImesShortNames[iIndex];
    }
    else
    {
        return NULL;
    }
}

void IMEManager::Clear()
{
    iIndex = NO_IME;
}

IIMEBase* IMEManager::GetActiveIME()
{
	if(!fIsInit) 
	{
		Init();
	}

	while(!fIsInit)
	{
		sleep(1);
	}

    //int ime = SystemSettingInfo::GetInstance()->m_IME.GetOptionIME();

    int index = 0;
    //while ( ime >= 1 && (ime & 2) == 0)
    //{
        //ime /= 2;
        //index++;
    //}

	return Imes[index];
}


