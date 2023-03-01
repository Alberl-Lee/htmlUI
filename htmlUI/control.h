#pragma once
#include "framework.h"

enum ENUM_DOM_TYPE
{
    EM_DOM_TYPE_TEXT,
    EM_DOM_TYPE_CONTROL,
};

enum ENUM_BUTTON_STATE
{
    EM_BUTTON_STATE_NORMAL,
    EM_BUTTON_STATE_HOVER,
    EM_BUTTON_STATE_FOCUS,
    EM_BUTTON_STATE_ACTIVE,
};

struct BUTTON_INFO
{
    ENUM_DOM_TYPE domType;
    ENUM_BUTTON_STATE state;
    SkColor colorNormal;
    SkColor colorHover;
    SkColor colorDown;
    SkColor colorUp;

    SkRect rect;
    SkScalar border_radius;
    std::string strText;
    std::string strOuterHtml;
};

struct CSS_INFO
{
    int width;
    int height;
    int border_radius;
    SkColor background_color;

    explicit CSS_INFO() {
        width = -1;
        height = -1;
        border_radius = -1;
        background_color = -1;
    }
};
