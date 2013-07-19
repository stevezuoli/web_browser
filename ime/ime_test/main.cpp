/*
 * =====================================================================================
 *       Filename:  main.cpp
 *    Description:  run-test for ime
 *
 *        Version:  1.0
 *        Created:  07/12/2013 10:15:31 AM
 * =====================================================================================
 */

#include <stdio.h>
#include <string>
#include "ime/IMEManager.h"
using namespace std;

void ProcessKeyAndPrintResult(char key)
{
    printf("ProcessKeyAndPrintResult %c\n", key);
    IIMEBase* activeIme = IMEManager::GetActiveIME();
    eProcessResult res = activeIme->ProcessKeys(key);
    printf("%d\n", res);
    printf("%s\n", activeIme->GetInputString().c_str());
    const int maxSelects = 20;
    int curIndex = 0;
    while (curIndex < maxSelects)
    {
        string pyWord = activeIme->GetStringByIndex(curIndex++).c_str();
        if (pyWord.empty())
        {
            break;
        }
        else
        {
            printf("%s\n",pyWord.c_str());
        }
    }

    activeIme->SelectIndex(1);
    printf("GetInputString, %s\n", activeIme->GetInputString().c_str());
}

int main()
{
    printf("=================================================================\n");
    printf("=========     run test of ime.Ignore this.     ==================\n");
    printf("=================================================================\n");

    ProcessKeyAndPrintResult('w');
    ProcessKeyAndPrintResult('o');
    ProcessKeyAndPrintResult('d');

    printf("=================================================================\n");
    printf("=========     run test of ime done.Ignore this.     ==================\n");
    printf("=================================================================\n");
    return 0;
}

