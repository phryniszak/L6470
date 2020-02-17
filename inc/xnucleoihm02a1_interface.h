/**
 ******************************************************************************
 * @file    xnucleoihm02a1_interface.h
 * @brief   This file is used as header of the interface between the 
 *          X-NUCLEO-IHM02A1 and the NUCLEO-F0xx board.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XNUCLEO_INTERFACE_H
#define __XNUCLEO_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

    int hspi;
    
    /* GPIO settings for the nSTBY_nRST pins of the L6470 */
#define L6470_nSTBY_nRST_GPIO_RPI       24

    /* GPIO settings for the nBUSY_SYNC pin of the L6470 */
#define L6470_nBUSY_SYNC_GPIO_RPI       27

    /* GPIO settings for the nFLAG pin of the L6470 */
#define L6470_nFLAG_GPIO_RPI            22

    /* GPIO settings for the options about the nCS pins of the L6470 */
#define L6470_nCS_OP0_GPIO_RPI          8
#define L6470_nCS_OP1_GPIO_RPI          7

    typedef unsigned sL6470_GPIO;

    extern sL6470_GPIO L6470_nSTBY_nRST_GPIO;
    extern sL6470_GPIO L6470_nBUSY_SYNC_GPIO;
    extern sL6470_GPIO L6470_nFLAG_GPIO;
    extern sL6470_GPIO L6470_nCS_GPIO;
    extern sL6470_GPIO L6470_nCS_GPIO_Array[];

#ifdef __cplusplus
}
#endif

#endif /* __XNUCLEO_INTERFACE_H */
