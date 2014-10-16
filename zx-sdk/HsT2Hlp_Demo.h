#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <Wincon.h>
#include <stdio.h>
#include "../zx-sdk/CITICs_HsT2Hlp.h"
#include <nb30.h>

void ShowAnsData(HSHLPHANDLE HlpHandle);
void ShowErrMsg(HSHLPHANDLE HlpHandle, int iFunc);
HSHLPHANDLE Run();
void SetNecessaryParam(HSHLPHANDLE HlpHandle);
bool GetLocalIP(char* ip);
bool GetFirstMac (char * mac);
int run_zxsdk_init();
int run_zxsdk_test();
int run_zxsdk_fini();