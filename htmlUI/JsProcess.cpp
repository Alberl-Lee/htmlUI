#include "JsProcess.h"
#include "../thirdParty/quickjs/quickjs.h"
#include "../common/UtilsSys.h"
#include "framework.h"

JSRuntime *g_rt = nullptr;
JSContext *g_ctx = nullptr;

void bindCppFunc(JSContext *ctx);
void bindObject(JSContext *ctx);
void EvalJs(const std::string &jsCode);

CJsProcess::CJsProcess() {}

CJsProcess::~CJsProcess()
{
    if (g_ctx)
    {
        JS_FreeContext(g_ctx);
    }
    if (g_rt)
    {
        JS_FreeRuntime(g_rt);
    }
}

void CJsProcess::LoadJs(const std::string &path)
{
    std::string jsCode;
    ReadFile(path, jsCode);

    g_rt = JS_NewRuntime();
    g_ctx = JS_NewContext(g_rt);
    bindCppFunc(g_ctx);
    bindObject(g_ctx);
    EvalJs(jsCode);
}

void CJsProcess::FireMouseMessage(std::shared_ptr<BUTTON_INFO> btnActive)
{
    std::string jsCode = R"(
document.querySelector.addEventListener();
)";

    EvalJs(jsCode);
}

int cpp_Add(int a, int b)
{
    return a + b;
}

JSValue cppFuncBind(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    int a = 0;
    if (argc > 0)
    {
        JS_ToInt32(ctx, &a, argv[0]);
    }

    return JS_NewInt32(ctx, cpp_Add(a, 5));
}

void bindCppFunc(JSContext *ctx)
{
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global_obj, "testAdd", JS_NewCFunction(ctx, cppFuncBind, "testAdd", 1));
    JS_FreeValue(ctx, global_obj);
}

int cpp_addEventListener()
{
    ::MessageBoxA(NULL, "被点击了", "点击事件", NULL);
    return 0;
}

JSValue bind_addEventListener(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    return JS_NewInt32(ctx, cpp_addEventListener());
}

void bindObject(JSContext *ctx)
{
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue document = JS_NewObject(ctx);
    JSValue domObject = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, domObject, "addEventListener", JS_NewCFunction(ctx, bind_addEventListener, "addEventListener", 1));
    JS_SetPropertyStr(ctx, document, "querySelector", domObject);
    JS_SetPropertyStr(ctx, global_obj, "document", document);

    JS_FreeValue(ctx, global_obj);
}

void EvalJs(const std::string &jsCode)
{

    JSValue jsValue = JS_Eval(g_ctx, jsCode.c_str(), jsCode.length(), "<eval>", JS_EVAL_TYPE_GLOBAL);
    double dValue;
    if (JS_ToFloat64(g_ctx, &dValue, jsValue))
    {
        JS_FreeValue(g_ctx, jsValue);
        return;
    }

    JS_FreeValue(g_ctx, jsValue);
}
