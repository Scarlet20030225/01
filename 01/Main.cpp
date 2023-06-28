#include"Dx12System.h"

using namespace MyDx12;

Dx12System* DxSys = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//アプリケーションのエントリー関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	DxSys = new Dx12System;

	if (DxSys != NULL)
	{
		//ウィンドウの作成
		WNDCLASSEX  wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize		 = sizeof(wc);
		wc.style		 = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc	 = WndProc;
		wc.hInstance	 = hInstance;
		wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		wc.lpszClassName = (LPSTR)AppName;
		wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&wc);

		DxSys->m_hWnd = CreateWindow
		(
			(LPSTR)AppName,
			(LPSTR)AppName,
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			WindowWidth,
			WindowHeight,
			0,
			0,
			hInstance,
			0
		);

		if (DxSys->m_hWnd)
		{
			return E_FAIL;
		}

		ShowWindow(DxSys->m_hWnd, SW_SHOW);
		UpdateWindow(DxSys->m_hWnd);

		if (SUCCEEDED(DxSys->InitDx3D()))
		{
			MSG msg = { 0 };
			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					DxSys->Render();
				}
			}
		}

		DxSys->DestroyDx3D();
		delete DxSys;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}