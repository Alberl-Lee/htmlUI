#pragma once
#include <string>
#include "control.h"

class CJsProcess
{
public:
    CJsProcess();
    ~CJsProcess();

    void LoadJs(const std::string &path);
    void FireMouseMessage(std::shared_ptr<BUTTON_INFO> btnActive);

private:
};
