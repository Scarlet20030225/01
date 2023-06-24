#include<windows.h>
#include<tchar.h>

class Win32Application
{
public:
	Win32Application() {}
	~Win32Application() {}



private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};