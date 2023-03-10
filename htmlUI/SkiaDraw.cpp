#include "SkiaDraw.h"
#include <include/utils/SkTextUtils.h>
#include "../common/UtilsSys.h"
#include "jsProcess.h"
#include "CSSParser.h"

using namespace htmlcxx;
using namespace std;

void createBitmap(BITMAPINFO *&pbmp, int width, int height, int bitCount)
{
    if (pbmp)
        return;

    const size_t bmpSize = sizeof(BITMAPINFOHEADER) + width * height * sizeof(uint32_t);
    pbmp = (BITMAPINFO *)new BYTE[bmpSize]();
    pbmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmp->bmiHeader.biWidth = width;
    pbmp->bmiHeader.biHeight = -height;
    pbmp->bmiHeader.biPlanes = 1;
    pbmp->bmiHeader.biBitCount = bitCount;
    pbmp->bmiHeader.biCompression = BI_RGB;

    // biHeight为正时，bmpInfo->bmiColors像素是从图片的左下角开始。
    // biHeight为负时，bmpInfo->bmiColors像素是从图片的左上角开始。
    // 这是BMP的格式中指定。而skia绘制需要从上到下。
}

// 文字居中显示
void DrawTextCenter(const string &str, SkRect &skRect, SkPaint &textpaint, SkFont &font, SkCanvas *canvas)
{
    SkRect bounds;
    font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
    // float yCenter = skRect.centerY() + y / 2;
    float yCenter = skRect.centerY() - bounds.fTop / 2;
    SkTextUtils::DrawString(canvas, str.c_str(), skRect.centerX(), yCenter, font, textpaint, SkTextUtils::kCenter_Align);

    // string = "qwer";
    // textpaint.setColor(SkColor(0xff0000ff));
    ////canvas->drawString(string, 15, 20, font, textpaint);
    // SkTextUtils::DrawString(canvas, string.c_str(), skRect.centerX(), 10, font, textpaint, SkTextUtils::kRight_Align);
}

std::shared_ptr<BUTTON_INFO> InitBtn(ENUM_DOM_TYPE domType, SkRect rect, const std::string &strText, const std::string &strOuterHtml)
{
    // BUTTON_INFO* pBtn = new BUTTON_INFO;
    // std::shared_ptr<BUTTON_INFO> pBtn(new BUTTON_INFO());
    auto pBtn = std::make_shared<BUTTON_INFO>();
    pBtn->domType = domType;
    pBtn->state = EM_BUTTON_STATE_NORMAL;
    pBtn->rect = rect;
    pBtn->colorNormal = SkColorSetRGB(78, 201, 176);
    pBtn->colorHover = SkColorSetRGB(85, 177, 85);
    pBtn->colorDown = SkColorSetRGB(208, 177, 50);
    pBtn->colorUp = SkColorSetRGB(78, 201, 176);
    pBtn->border_radius = -1;
    pBtn->strText = strText;
    pBtn->strOuterHtml = strOuterHtml;

    return pBtn;
}

CSkiaDraw::CSkiaDraw() : m_css(std::make_shared<CSSParser>()), m_jsProcess(std::make_shared<CJsProcess>())
{
}

void CSkiaDraw::SetBaseURI(const std::string &path)
{
    m_uriBase = path;
}

void CSkiaDraw::InitControls(const std::string &path)
{
    HTML::ParserDom parser;
    string html;
    ReadFile(path, html);
    m_tree = parser.parseTree(html);

    SkPoint marginDefault(SkPoint::Make(10, 10));
    SkRect rectDefault = SkRect::MakeXYWH(marginDefault.fX, marginDefault.fY, 80, 35);
    SkPoint pointNow(SkPoint::Make(marginDefault.fX, marginDefault.fY));
    for (auto node : m_tree)
    {
        if ("link" == node.tagName())
        {
            node.parseAttributes();
            pair<bool, string> p = node.attribute("href");
            if (p.first)
            {
                m_css->LoadCSS(m_uriBase + p.second);
            }
        }
        else if ("script" == node.tagName())
        {
            node.parseAttributes();
            pair<bool, string> p = node.attribute("src");
            if (p.first)
            {
                m_jsProcess->LoadJs(m_uriBase + p.second);
            }
        }

        if ("" == node.content(html))
        {
            continue;
        }

        std::shared_ptr<BUTTON_INFO> pBtn = nullptr;
        SkRect rectNow = rectDefault;

        bool isNewLine = false;
        if ("button" == node.tagName())
        {
            node.parseAttributes();
            SkScalar border_radius = -1;
            pair<bool, string> p = node.attribute("class");
            if (p.first)
            {
                CSS_INFO cssInfo = m_css->GetCSSInfoByClass(p.second);
                border_radius = cssInfo.border_radius;
                rectNow.setXYWH(rectNow.fLeft, rectNow.fTop, (-1 == cssInfo.width) ? rectNow.width() : cssInfo.width, (-1 == cssInfo.height) ? rectNow.height() : cssInfo.height);
            }

            rectNow.offset(marginDefault.fX + pointNow.fX, pointNow.fY);
            pBtn = InitBtn(EM_DOM_TYPE_CONTROL, rectNow, node.content(html), node.text());
            pBtn->border_radius = border_radius;
        }
        else if ("h2" == node.tagName())
        {
            rectNow.setWH(100, 30);
            rectNow.offset(pointNow.fX, pointNow.fY);
            pBtn = InitBtn(EM_DOM_TYPE_TEXT, rectNow, node.content(html), node.text());
            pointNow.fY += rectNow.height() + marginDefault.fY;
            pointNow.fX = marginDefault.fX;
            isNewLine = true;
        }

        if (pBtn)
        {
            m_ctlAll.push_back(pBtn);
            pointNow.fX += marginDefault.fX + (isNewLine ? 0 : rectNow.width());
        }
    }
}

void CSkiaDraw::DrawWithSkia(HWND hwnd, CMemDCEx &memDC)
{
    CRect rect;
    ::GetClientRect(hwnd, rect);
    int bmpWidth = rect.Width();
    int bmpHeight = rect.Height();
    BITMAPINFO *bmpInfo = nullptr;
    createBitmap(bmpInfo, bmpWidth, bmpHeight, 32);
    void *pixels = bmpInfo->bmiColors; // 图片像素位置指针

    SkImageInfo info = SkImageInfo::Make(bmpWidth, bmpHeight, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
    sk_sp<SkSurface> surface = SkSurface::MakeRasterDirect(info, pixels, bmpWidth * sizeof(uint32_t));
    SkCanvas *canvas = surface->getCanvas();
    DrawControls(canvas);

    StretchDIBits(memDC->m_hDC, 0, 0, bmpWidth, bmpHeight, 0, 0, bmpWidth, bmpHeight, pixels, bmpInfo, DIB_RGB_COLORS, SRCCOPY); // 整张图绘制到DC
    delete[] bmpInfo;
}

std::shared_ptr<BUTTON_INFO> CSkiaDraw::ChangeButtonState(SkPoint pt, ENUM_BUTTON_STATE stateInRect)
{
    std::shared_ptr<BUTTON_INFO> btnActive = nullptr;
    for (auto btn : m_ctlAll)
    {
        if (btn->rect.contains(pt.fX, pt.fY))
        {
            btn->state = stateInRect;
            btnActive = btn;
        }
        else
        {
            btn->state = EM_BUTTON_STATE_NORMAL;
        }
    }

    return btnActive;
}

void CSkiaDraw::FireMouseMessage(std::shared_ptr<BUTTON_INFO> btnActive)
{
    m_jsProcess->FireMouseMessage(btnActive);
}

void CSkiaDraw::DrawControls(SkCanvas *canvas)
{
    canvas->clear(SK_ColorWHITE); // 白色背景
    // canvas->clear(0x00000000);  // 透明背景

    SkRect rect;
    SkColor colorBtn;
    SkPaint paint;
    SkPaint textpaint;
    textpaint.reset();
    textpaint.setColor(SkColor(0xffff0000));
    // textpaint.setAntiAlias(true);
    //  SkFont font(SkTypeface::MakeFromName("Consolas", SkFontStyle::Bold()), 16);
    //  SkFont font(SkTypeface::MakeFromName("Arial", SkFontStyle::Bold()), 16);
    SkFont font; // 默认字体
    font.setSize(16);

    for (auto btn : m_ctlAll)
    {
        switch (btn->state)
        {
        case EM_BUTTON_STATE_HOVER:
            colorBtn = btn->colorHover;
            break;
        case EM_BUTTON_STATE_ACTIVE:
            colorBtn = btn->colorDown;
            break;
        default:
            colorBtn = btn->colorNormal;
            break;
        }

        rect = btn->rect;
        paint.setColor(colorBtn);

        if (EM_DOM_TYPE_TEXT == btn->domType)
        {
            SkPaint paint;
            paint.setARGB(0xff, 0x48, 0xc7, 0x74);
            SkFont font(SkTypeface::MakeFromName("", SkFontStyle::Bold()), 30); // 默认字体
            // paint.setAntiAlias(true);
            canvas->drawString(btn->strText.c_str(), rect.fLeft, rect.fBottom, font, paint);
            // DrawTextCenter(str, rect, textpaint, font, canvas);
        }
        else
        {
            if (-1 != btn->border_radius)
            {
                paint.setAntiAlias(true);
                canvas->drawCircle(rect.centerX(), rect.centerY(), btn->border_radius, paint);
            }
            else
            {
                canvas->drawRect(rect, paint);
            }

            textpaint.setColor(SkColor(0xff000000));
            DrawTextCenter(btn->strText.c_str(), rect, textpaint, font, canvas);
        }
    }
}
