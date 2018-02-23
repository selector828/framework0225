#pragma once

#include <windows.h>

class Graphics
{
public:
	Graphics(HWND hwnd, unsigned int width = 1280, unsigned int height = 720) : width_(width), height_(height)
	{
		{ //åxçêñhé~
			hwnd = hwnd;
		}
	}

public:
	virtual void Render(void) = 0;

protected:
	unsigned int width_ = 0;
	unsigned int height_ = 0;
};