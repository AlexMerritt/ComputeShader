#pragma once

#include <Windows.h>

class Window
{
public:
	bool Initialize(int width, int height);


	HWND GetHandle() { return m_hwnd; }
	int Width() { return m_width; }
	int Height() { return m_height; }

private:
	void DestroyTempWindow();

private:
	HWND m_hwnd;
	HINSTANCE m_hinstance;
	LPCWSTR m_applicationName;

	int m_width;
	int m_height;
	bool m_tempWindowActive;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);