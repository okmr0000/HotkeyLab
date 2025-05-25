/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/*******************************************************************/

/*
    RGBDelay.h
*/

#pragma once

#ifndef RGBDELAY_H
#define RGBDELAY_H

// Windows header must come first
#include <Windows.h>

// After Effects SDK headers
#include "AEConfig.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "AE_EffectCBSuites.h"
#include "Param_Utils.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEGP_SuiteHandler.h"

#define PF_DEEP_COLOR_AWARE 1    // make sure we get 16bpc pixels

// Plugin version information
#define MAJOR_VERSION    2
#define MINOR_VERSION    4
#define BUG_VERSION      0
#define STAGE_VERSION    PF_Stage_DEVELOP
#define BUILD_VERSION    0

// Parameter definitions
enum {
    RGBDELAY_INPUT = 0,
    RGBDELAY_RED_DELAY,
    RGBDELAY_GREEN_DELAY,
    RGBDELAY_BLUE_DELAY,
    RGBDELAY_NUM_PARAMS
};

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport)
PF_Err
EffectMain(
    PF_Cmd            cmd,
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output,
    void            *extra);

#ifdef __cplusplus
}
#endif

#endif // RGBDELAY_H
