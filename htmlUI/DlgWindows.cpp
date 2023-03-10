#include "DlgWindows.h"
#include "resource.h"
#include <vector>
//#include <shellscalingapi.h>
//#pragma comment(lib, "Shcore.lib")

#include "../common/UtilsSys.h"
#include "../thirdParty/WTL/atlmisc.h"
#include "control.h"
#include "SkiaDraw.h "

WCHAR szWindowClass[] = L"htmlUI"; // 主窗口类名

DlgWindows::DlgWindows(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr), m_pSkiaDraw(std::make_shared<CSkiaDraw>())
{
}

DlgWindows::~DlgWindows()
{
}

void DlgWindows::RunMessageLoop()
{
    //::SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

    MyRegisterClass(m_hInstance);
    if (!InitInstance(m_hInstance, SW_SHOWDEFAULT))
        return;

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}

void DlgWindows::setURI(const std::string &path)
{
    m_uri = path;
    m_uriBase = GetBasePath(path);
}

ATOM DlgWindows::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &DlgWindows::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HTMLUI));
    wcex.hIconSm = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;

    return ::RegisterClassExW(&wcex);
}

BOOL DlgWindows::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindow(szWindowClass, L"htmlUI title", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
        return FALSE;

    m_hWnd = hWnd;
    ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));

    m_pSkiaDraw->SetBaseURI(m_uriBase);
    m_pSkiaDraw->InitControls(m_uri);
    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);
    return TRUE;
}

LRESULT DlgWindows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    DlgWindows *self = reinterpret_cast<DlgWindows *>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!self)
        return ::DefWindowProc(hWnd, message, wParam, lParam);

    switch (message)
    {
        // HANDLE_MSG(hWnd, WM_DROPFILES, OnDropFiles);
        // HANDLE_MSG(hWnd, WM_LBUTTONDBLCLK, OnPaint);

        HANDLE_MSG(hWnd, WM_PAINT, self->OnPaint);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, self->OnLButtonDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, self->OnLButtonUp);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, self->OnMouseMove);

    case WM_ERASEBKGND:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void DlgWindows::OnPaint(HWND hwnd)
{
    CPaintDC dc(hwnd);
    CMemDCEx memDC(&dc);
    m_pSkiaDraw->DrawWithSkia(hwnd, memDC);
}

void DlgWindows::OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    m_pSkiaDraw->ChangeButtonState(SkPoint::Make(x, y), EM_BUTTON_STATE_HOVER);
    ::InvalidateRect(hwnd, nullptr, FALSE);
}

void DlgWindows::OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    m_pSkiaDraw->ChangeButtonState(SkPoint::Make(x, y), EM_BUTTON_STATE_ACTIVE);
    ::InvalidateRect(hwnd, nullptr, FALSE);
}

void DlgWindows::OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    auto btnActive = m_pSkiaDraw->ChangeButtonState(SkPoint::Make(x, y), EM_BUTTON_STATE_NORMAL);
    ::InvalidateRect(hwnd, nullptr, FALSE);

    if (btnActive)
    {
        m_pSkiaDraw->FireMouseMessage(btnActive);
    }
}
