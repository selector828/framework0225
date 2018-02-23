#pragma once

#pragma warning(disable: 4005)
//#pragma warning(disable: 4455)

#include <windows.h>

#include <string>

class Window final
{
private:
	HWND hwnd_;
	std::string title_ = "";
	unsigned int width_;
	unsigned int height_;

public:
	Window(std::string title = "DefaultWindowTitle", unsigned int width = 1280, unsigned int height = 720) : title_(title), width_(width), height_(height)
	{
		{// ウィンドウクラスの登録
			WNDCLASSEX  wc;
			memset(&wc, 0, sizeof(wc));
			wc.cbSize = sizeof(wc);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = GetModuleHandleA(nullptr);
			wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
			wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
			wc.lpszClassName = this->title_.c_str();
			wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
			RegisterClassExA(&wc);
		}

		{// ウィンドウの大きさの再調整、中央配置計算
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = static_cast<long>(this->width_);
			rc.bottom = static_cast<long>(this->height_);

			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);

			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;

			RECT rc_desk;
			GetWindowRect(GetDesktopWindow(), &rc_desk);

			int x = rc_desk.right / 2 - w / 2;
			int y = rc_desk.bottom / 2 - h / 2;

			{// ウィンドウの生成、表示
				this->hwnd_ = CreateWindowExA(0, this->title_.c_str(), this->title_.c_str(), WS_OVERLAPPEDWINDOW,
					x, y, w, h, 0, 0, GetModuleHandleA(nullptr), 0);

				ShowWindow(this->hwnd_, SW_SHOW);
			}
		}
	}

public:
	HWND const Hwnd(void)
	{
		return this->hwnd_;
	}
	unsigned int const Width(void)
	{
		return this->width_;
	}
	unsigned int const Height(void)
	{
		return this->height_;
	}

private:
	static LRESULT __stdcall WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (iMsg)
		{
		case WM_KEYDOWN:if (wParam == VK_ESCAPE) PostQuitMessage(0); break;
		case WM_DESTROY:PostQuitMessage(0); break;
		}
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

public:
	bool MessageLoop(void)
	{
		MSG msg = { 0 };
		memset(&msg, 0, sizeof(msg));

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			if (msg.message == WM_QUIT) return false;
		}
		return true;
	}
};