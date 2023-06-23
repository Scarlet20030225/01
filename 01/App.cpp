#include "App.h"

Main* m_main = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�A�v���P�[�V�����̃G���g���[�֐�
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	m_main = new Main;

	if (m_main != NULL)
	{
		//�E�B���h�E�̍쐬
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
			(LPSTR)kAppName,		// �E�B���h�E�̃N���X��
			(LPSTR)kAppName,		// �E�B���h�E�̖��O
			WS_OVERLAPPEDWINDOW,	// �E�B���h�E�̃X�^�C��
			CW_USEDEFAULT,			// �E�B���h�E�̍��[�̈ʒu
			CW_USEDEFAULT,			// �E�B���h�E�̏�[�̈ʒu
			CW_USEDEFAULT,			// �E�B���h�E�̕�
			CW_USEDEFAULT,			// �E�B���h�E�̍���
			0,						// �e�E�B���h�E�̃n���h��
			0,						// ���j���[�n���h��
			hInstance,				// �C���X�^���X�n���h��
			0						// �E�B���h�E�쐬�f�[�^
		);

		if (!m_main->m_hWnd)
		{
			return E_FAIL;
		}

		ShowWindow(m_main->m_hWnd, SW_SHOW);
		UpdateWindow(m_main->m_hWnd);

		{
			// ���b�Z�[�W���[�v
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
		//�A�v���I��
		delete m_main;
	}
	return 0;
}

//�E�B���h�E�v���V�[�W��
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
		id = MessageBox(hWnd, TEXT("���Ȃ��͂��قł���?�i�΁j"), TEXT("����"), MB_YESNO | MB_ICONEXCLAMATION);

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