#include "CSSParser.h"
#include "../common/UtilsSys.h"
#include "control.h"

using namespace std;
using namespace htmlcxx;

CSSParser::CSSParser() :
    m_css(std::make_shared<CSS::Parser>()) {
}

void CSSParser::LoadCSS(const std::string& path)
{
    string css;
    ReadFile(path, css);
    m_css->parse(css);
}

CSS_INFO CSSParser::GetCSSInfoByClass(const std::string& strCSS)
{
    std::string strSelector = strCSS;
    CSS::Parser::Selector selector;
    if ('#' == strSelector.front())
    {
        selector.setId(strSelector);
    } 
    else
    {
        strSelector = strCSS;
        selector.setClass(strSelector);
    }

    std::vector<CSS::Parser::Selector> vctSel;
    vctSel.push_back(selector);

    CSS_INFO info;
    auto kv = m_css->getAttributes(vctSel);
    if (kv.find("width") != kv.end()) {
        info.width = atoi(kv["width"].c_str());
    }

    if (kv.find("height") != kv.end()) {
        info.height = atoi(kv["height"].c_str());
    }

    if (kv.find("border-radius") != kv.end()) {
        info.border_radius = atoi(kv["border-radius"].c_str());
    }

    if (kv.find("background-color") != kv.end()) {
        info.background_color = atoi(kv["background-color"].c_str());
    }

    return info;
}
