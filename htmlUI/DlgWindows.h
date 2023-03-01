#pragma once
#include "framework.h"

class CSkiaDraw;
class DlgWindows
{
public:
    explicit DlgWindows(HINSTANCE hInstance);
    ~DlgWindows();

    void RunMessageLoop();
    void setURI(const std::string& path);

private:
    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void OnPaint(HWND hwnd);
    void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
    void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
    void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

    HWND m_hWnd;
    HINSTANCE m_hInstance;
    std::shared_ptr<CSkiaDraw> m_pSkiaDraw;
    std::string m_uri;
    std::string m_uriBase;
};
