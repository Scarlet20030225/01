#include "App.h"

Main* m_main = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//アプリケーションのエントリー関数
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	m_main = new Main;

	if (m_main != NULL)
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
		wc.lpszClassName = (LPSTR)kAppName;
		wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wc);

		m_main->m_hWnd = CreateWindow
		(
			(LPSTR)kAppName,		// ウィンドウのクラス名
			(LPSTR)kAppName,		// ウィンドウの名前
			WS_OVERLAPPEDWINDOW,	// ウィンドウのスタイル
			CW_USEDEFAULT,			// ウィンドウの左端の位置
			CW_USEDEFAULT,			// ウィンドウの上端の位置
			CW_USEDEFAULT,			// ウィンドウの幅
			CW_USEDEFAULT,			// ウィンドウの高さ
			0,						// 親ウィンドウのハンドル
			0,						// メニューハンドル
			hInstance,				// インスタンスハンドル
			0						// ウィンドウ作成データ
		);

		if (!m_main->m_hWnd)
		{
			return E_FAIL;
		}

		ShowWindow(m_main->m_hWnd, SW_SHOW);
		UpdateWindow(m_main->m_hWnd);

		{
			// メッセージループ
			MSG msg = { 0 };
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		//アプリ終了
		delete m_main;
	}
	return 0;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_main->MsgProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int id;

	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);

			break;
		}

		break;

	case WM_CLOSE:
		id = MessageBox(hWnd, TEXT("あなたはあほですか?（笑）"), TEXT("質問"), MB_YESNO | MB_ICONEXCLAMATION);

		if (id == IDYES)
		{
			DestroyWindow(hWnd);
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}