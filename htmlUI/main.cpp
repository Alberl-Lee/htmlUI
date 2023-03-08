#include "DlgWindows.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    DlgWindows dlgWindows(hInstance);
    //dlgWindows.setURI("E:\\htmlUI\\_demo\\miniBrowser\\index.html");

#ifdef _DEBUG
    dlgWindows.setURI("..\\_demo\\miniBrowser\\index.html");
#else
    dlgWindows.setURI("index.html");
#endif

    dlgWindows.RunMessageLoop();
    return 0;
}
