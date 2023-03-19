﻿#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

#include "prepar.h"

using namespace std;

HWND hEdit;
HWND hButton;//按钮一号“抽取一人”
vector<string> names;

HWND hButton2;//按钮二号
bool scrolling = false;//是否滚动
HWND hButton3;
HWND hButton4;

string UTF8ToGB(const char* str);

void loadNames() {
    ifstream file("名单.txt");
    string name;
    //int a = 10;
    while (getline(file, name)) {
    //while (a-- > 0) {
        getline(file, name);
        name = UTF8ToGB(name.c_str()).c_str();
        names.push_back(name);
    }
}

/*string getRandomName2() {
    srand(time(0));
    int index = rand() % names.size();
    return names[index];
}
*/

void openFile() {
    ShellExecute(NULL, "open", "名单.txt", NULL, NULL, SW_SHOWNORMAL);
}

/*void speakName(HWND hwnd) {
    char text[1024];
    GetWindowText(hEdit, text, 1024);

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    ISpVoice* pVoice = NULL;

    if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL,
        IID_ISpVoice, (void**)&pVoice))) {
        MessageBox(hwnd, "Error initializing text-to-speech engine.", "Error", MB_OK);
        return;
    }*/

void startScrolling(HWND hwnd) {
    scrolling = true;
    SetTimer(hwnd, 1, Movingspeed, NULL);
}

void stopScrolling(HWND hwnd) {
    scrolling = false;
    KillTimer(hwnd, 1);
}

string getRandomName() {
    srand(time(0));
    if (names.size() == 0){
        names.push_back("没有人");
    }
    int index = rand() % names.size();
    return string(names[index]);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND:
            if ((HWND)lp == hButton) {
                string name = getRandomName();
                SetWindowText(hEdit, name.c_str());
            }
            else if ((HWND)lp == hButton2) {
                if (scrolling) {
                    stopScrolling(hwnd);
                }
                else {
                    startScrolling(hwnd);
                }
            }
            break;
        case WM_CREATE:
            loadNames();
            hEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | ES_READONLY,
                                 10, 10, 200, 20,
                                 hwnd, NULL, NULL, NULL);
            hButton = CreateWindow("BUTTON", "抽取一人", WS_VISIBLE | WS_CHILD,
                                   10, 40, 100, 30,
                                   hwnd,(HMENU)1,NULL,NULL);
            hButton2 = CreateWindow("BUTTON", "开始/停止滚动", WS_VISIBLE | WS_CHILD,
                120, 40, 100, 30,
                hwnd, (HMENU)2, NULL, NULL);
            break;
        case WM_TIMER:
            if (wp == 1 && scrolling) {
                string name = getRandomName();
                SetWindowText(hEdit, name.c_str());
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,msg ,wp ,lp);
    }
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst ,LPSTR args,int ncmdshow){
    WNDCLASS wc = {0};
    wc.hbrBackground =(HBRUSH)COLOR_WINDOW ;
    wc.hCursor=LoadCursor(NULL,IDC_ARROW );
    wc.hInstance=hInst ;
    wc.lpszClassName="myWindowClass";
    wc.lpfnWndProc=WindowProcedure ;

if(!RegisterClass(&wc))
return -1;

CreateWindow("myWindowClass","一个窗口",WS_OVERLAPPEDWINDOW|WS_VISIBLE ,
             CW_USEDEFAULT,CW_USEDEFAULT ,500 ,500 ,
             NULL,NULL,NULL,NULL);

MSG msg ={0};
while(GetMessage(&msg,NULL,NULL,NULL ))
{
TranslateMessage(&msg );
DispatchMessage(&msg );
}
return 0;
}



string UTF8ToGB(const char* str) {
    string result;
    WCHAR* strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;
}