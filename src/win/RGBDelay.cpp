#define NOMINMAX
#include "AEConfig.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "Param_Utils.h"
#include "AE_Macros.h"
#include "String_Utils.h"
#include "RGBDelay.h"
#include <algorithm>
#include <stdio.h>

// バージョン情報
#define RGBDELAY_MAJOR_VERSION 0
#define RGBDELAY_MINOR_VERSION 2
#define RGBDELAY_BUG_VERSION 0
#define RGBDELAY_STAGE_VERSION 0
#define RGBDELAY_BUILD_VERSION 0

// Windows用のエクスポート定義
#define DllExport __declspec(dllexport)

// Mac版と同じ関数をそのまま使用
static PF_Err GlobalSetup(
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* output)
{
    PF_Err err = PF_Err_NONE;
    out_data->my_version = PF_VERSION(2, 4, 0, 0, 0);
    out_data->out_flags = 0x3000200;
    out_data->out_flags2 = 0x8000007;
    return err;
}



static PF_Err ParamsSetup(
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* output)
{
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;                // ←追加
    AEFX_CLR_STRUCT(def);           // ←追加

    PF_ADD_SLIDER("Red Delay", 0, 30, 0, 30, 0, 1);
    PF_ADD_SLIDER("Green Delay", 0, 30, 0, 30, 1, 2);
    PF_ADD_SLIDER("Blue Delay", 0, 30, 0, 30, 2, 3);

    out_data->num_params = RGBDELAY_NUM_PARAMS;
    return err;
}

// レンダリング処理
static PF_Err Render(
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* outputP)
{
    PF_Err err = PF_Err_NONE;
    PF_ParamDef red_param = {0}, green_param = {0}, blue_param = {0};

    // パラメータ取得
    A_long red_delay = params[RGBDELAY_RED_DELAY]->u.sd.value;
    A_long green_delay = params[RGBDELAY_GREEN_DELAY]->u.sd.value;
    A_long blue_delay = params[RGBDELAY_BLUE_DELAY]->u.sd.value;

    // ディレイ時刻のガード
    A_long red_time = in_data->current_time - red_delay * in_data->time_step;
    A_long green_time = in_data->current_time - green_delay * in_data->time_step;
    A_long blue_time = in_data->current_time - blue_delay * in_data->time_step;

    if (red_time < 0) red_time = 0;
    if (green_time < 0) green_time = 0;
    if (blue_time < 0) blue_time = 0;

    // 各チャンネル用に過去フレームを取得
    AEFX_CLR_STRUCT(red_param);
    AEFX_CLR_STRUCT(green_param);
    AEFX_CLR_STRUCT(blue_param);

    // エラーチェックを追加
    if ((err = PF_CHECKOUT_PARAM(in_data, RGBDELAY_INPUT, red_time, in_data->time_step, in_data->time_scale, &red_param)) != PF_Err_NONE) {
        return err;
    }

    if ((err = PF_CHECKOUT_PARAM(in_data, RGBDELAY_INPUT, green_time, in_data->time_step, in_data->time_scale, &green_param)) != PF_Err_NONE) {
        PF_CHECKIN_PARAM(in_data, &red_param);
        return err;
    }

    if ((err = PF_CHECKOUT_PARAM(in_data, RGBDELAY_INPUT, blue_time, in_data->time_step, in_data->time_scale, &blue_param)) != PF_Err_NONE) {
        PF_CHECKIN_PARAM(in_data, &red_param);
        PF_CHECKIN_PARAM(in_data, &green_param);
        return err;
    }

    // 16bit判定
    if (PF_WORLD_IS_DEEP(outputP)) {
        // 16bit処理
        for (A_long y = 0; y < outputP->height; y++) {
            PF_Pixel16* out_pixel = (PF_Pixel16*)((char*)outputP->data + y * outputP->rowbytes);
            PF_Pixel16* r_pixel = (PF_Pixel16*)((char*)red_param.u.ld.data + y * red_param.u.ld.rowbytes);
            PF_Pixel16* g_pixel = (PF_Pixel16*)((char*)green_param.u.ld.data + y * green_param.u.ld.rowbytes);
            PF_Pixel16* b_pixel = (PF_Pixel16*)((char*)blue_param.u.ld.data + y * blue_param.u.ld.rowbytes);

            for (A_long x = 0; x < outputP->width; x++) {
                // アルファは最大値
                A_u_short alpha = std::max(std::max(r_pixel[x].alpha, g_pixel[x].alpha), b_pixel[x].alpha);
                out_pixel[x].alpha = alpha;
                // ストレートアルファ（RGB値はそのまま）
                out_pixel[x].red = r_pixel[x].red;
                out_pixel[x].green = g_pixel[x].green;
                out_pixel[x].blue = b_pixel[x].blue;
            }
        }
    }
    else {
        // 8bit処理
        for (A_long y = 0; y < outputP->height; y++) {
            PF_Pixel* out_pixel = (PF_Pixel*)((char*)outputP->data + y * outputP->rowbytes);
            PF_Pixel* r_pixel = (PF_Pixel*)((char*)red_param.u.ld.data + y * red_param.u.ld.rowbytes);
            PF_Pixel* g_pixel = (PF_Pixel*)((char*)green_param.u.ld.data + y * green_param.u.ld.rowbytes);
            PF_Pixel* b_pixel = (PF_Pixel*)((char*)blue_param.u.ld.data + y * blue_param.u.ld.rowbytes);

            for (A_long x = 0; x < outputP->width; x++) {
                // アルファは最大値
                A_u_char alpha = std::max(std::max(r_pixel[x].alpha, g_pixel[x].alpha), b_pixel[x].alpha);
                out_pixel[x].alpha = alpha;
                // ストレートアルファ（RGB値はそのまま）
                out_pixel[x].red = r_pixel[x].red;
                out_pixel[x].green = g_pixel[x].green;
                out_pixel[x].blue = b_pixel[x].blue;
            }
        }
    }

    // メモリ解放
    PF_CHECKIN_PARAM(in_data, &red_param);
    PF_CHECKIN_PARAM(in_data, &green_param);
    PF_CHECKIN_PARAM(in_data, &blue_param);

    return err;
}

extern "C" DllExport
PF_Err PluginDataEntryFunction2(
    PF_PluginDataPtr inPtr,
    PF_PluginDataCB2 inPluginDataCallBackPtr,
    SPBasicSuite* inSPBasicSuitePtr,
    const char* inHostName,
    const char* inHostVersion)
{
    PF_Err result = PF_Err_INVALID_CALLBACK;

    result = PF_REGISTER_EFFECT_EXT2(
        inPtr,
        inPluginDataCallBackPtr,
        "RGBDelay",         // Name
        "ADBE RGBDelay",    // Match Name
        "Hotkey lab.",        // Category ← ここをPiPLと一致させる
        AE_RESERVED_INFO,   // Reserved Info
        "EffectMain",       // Entry point
        "https://www.adobe.com" // Support URL
    );

    return result;
}

extern "C" DllExport
PF_Err EffectMain(
    PF_Cmd cmd,
    PF_InData* in_data,
    PF_OutData* out_data,
    PF_ParamDef* params[],
    PF_LayerDef* output,
    void* extra)
{
    PF_Err err = PF_Err_NONE;

    switch (cmd) {
    case PF_Cmd_ABOUT: {
        const char* info =
            "RGBDelay v0.2.0\n"
            "Copyright (C) 2025 Tsuyoshi Okumura/Hotkey ltd.\n"
            "All Rights Reserved.\n"
            "\n"
            "This software is provided \"as is\" without warranty of any kind.\n"
            "Use at your own risk.\n";
        PF_SPRINTF(out_data->return_msg, "%s", info);
        break;
    }
    case PF_Cmd_GLOBAL_SETUP:
        err = GlobalSetup(in_data, out_data, params, output);
        break;
    case PF_Cmd_PARAMS_SETUP:
        err = ParamsSetup(in_data, out_data, params, output);
        break;
    case PF_Cmd_RENDER:
        err = Render(in_data, out_data, params, output);
        break;
    }

    return err;
}
