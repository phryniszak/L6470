/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utils.h
 * Author: pawel
 *
 * Created on 27 August 2018, 19:31
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

    void PrintRegisterValues(uint8_t ExpBrd, uint8_t L6470_Id);
    void PrintAllRegisterValues(void);
    int msleep(long msec);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */

