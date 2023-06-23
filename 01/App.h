#include<windows.h>

#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"Gdi32.lib") 
#pragma comment(lib,"Ole32.lib") 

#define kAppName "NoTitle"	// ウィンドウタイトル

class Main
{
public:
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	HWND m_hWnd;

private:

};

class App
{
public:


private:

};