#ifndef __CLK_H
#define __CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f4xx.h"

extern ErrStatus CLKHSEStatus;//标记外部晶振启动是否正常 0失败 1正常

ErrStatus rcu_init(void);

#ifdef __cplusplus
}
#endif

#endif