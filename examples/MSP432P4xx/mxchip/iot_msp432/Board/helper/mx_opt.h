/**
 ******************************************************************************
 * @file    mico_opt.h
 * @author  William Xu
 * @version V1.0.0
 * @date    22-July-2015
 * @brief   This file provide MiCO default configurations
 ******************************************************************************
 *
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

#ifndef __MX_OPT_H
#define __MX_OPT_H



#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                            MiCO OS and APP VERSION
 ******************************************************************************/

/******************************************************************************
 *                             MiCO Debug Enabler
 ******************************************************************************/

#if !defined MX_DEBUG_MIN_LEVEL
#define MX_DEBUG_MIN_LEVEL                    MX_DEBUG_LEVEL_ALL
#endif

#if !defined MX_DEBUG_TYPES_ON
#define MX_DEBUG_TYPES_ON                     MX_DEBUG_ON
#endif


/******************************************************************************
 *                            Debug and Log
 ******************************************************************************/

#if !defined CONFIG_APP_DEBUG
#define CONFIG_APP_DEBUG                       MX_DEBUG_ON
#endif


#ifdef __cplusplus
} /*extern "C" */
#endif

#endif //__MX_OPT_H
