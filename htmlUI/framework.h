// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once
#define NOMINMAX

// Skia 
#include <include/core/SkBitmap.h>
#include <include/core/SkRect.h>
#include <include/core/SkImageEncoder.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkShader.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkTypes.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontStyle.h>
#include <include/core/SkFontTypes.h>
#include <include/core/SkSurface.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkPngChunkReader.h>

#ifdef _DEBUG
//下载的这个debug库有问题。改成MT后，依然编译错误：值“MT_StaticRelease”不匹配值“MT_StaticDebug”。 是编译者cmake设置有问题，暂时不管
#pragma comment(lib, "../thirdParty/skia/skia_x64_debug.lib")
#else
#pragma comment(lib, "../thirdParty/skia/skia_x64_release.lib")
#endif


// Skia 
#include <d3d12.h>
#pragma comment(lib, "D3D12.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Opengl32.lib")

#ifdef _DEBUG
#include <dxgi1_3.h>
#pragma comment(lib, "DXGI.lib")
#endif

// Windows 头文件
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#include <windows.h>
#include <windowsx.h>

// WTL
#include <atlbase.h>
#include "../thirdParty/WTL/atlapp.h"
extern CAppModule _Module;
#include "../thirdParty/WTL/atlwinx.h"
#include "../thirdParty/WTL/atlmisc.h"    // CRect CString
#include "../thirdParty/WTL/atlcrack.h"   // MSG_WM_PAINT
#include "../thirdParty/WTL/atlctrls.h"   // CButton
#include "../thirdParty/WTL/atlddx.h"     // DDX_
#include "../thirdParty/WTL/atldlgs.h"    // CColorDialog

#include "../../common/MemDC.h"