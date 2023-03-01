#pragma once
#include "framework.h"
#include "control.h"
#include "../htmlcxx/html/ParserDom.h"

class CJsProcess;
class CSSParser;
class CSkiaDraw
{
public:
    explicit CSkiaDraw();

    void SetBaseURI(const std::string &path);
    void InitControls(const std::string &path);
    void DrawWithSkia(HWND hwnd, CMemDCEx &memDC);
    void FireMouseMessage(std::shared_ptr<BUTTON_INFO> btnActive);
    std::shared_ptr<BUTTON_INFO> ChangeButtonState(SkPoint pt, ENUM_BUTTON_STATE stateInRect);

private:
    void DrawControls(SkCanvas *canvas);

    std::string m_uriBase;
    std::vector<std::shared_ptr<BUTTON_INFO>> m_ctlAll;
    tree<htmlcxx::HTML::Node> m_tree;
    std::shared_ptr<CSSParser> m_css;
    std::shared_ptr<CJsProcess> m_jsProcess;
};
