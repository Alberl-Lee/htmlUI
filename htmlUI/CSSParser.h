#pragma once
#include <string>
#include <memory>
#include "../htmlcxx/css/parser_pp.h"

struct CSS_INFO;
class CSSParser
{
public:
    explicit CSSParser();

    void LoadCSS(const std::string& path);
    CSS_INFO GetCSSInfoByClass(const std::string& strCSS);
    
private:
    std::shared_ptr<htmlcxx::CSS::Parser> m_css;
};

