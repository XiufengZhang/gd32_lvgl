// #include "stm32f4xx_rcc.h"
// #include "stm32f4xx_rtc.h"
// #include "stm32f4xx_pwr.h"
#include "clk.h"

ErrStatus CLKHSEStatus = ERROR;//标记外部晶振启动是否正常 ERROR失败 SUCCESS正常

/**
 * @brief  时钟 外设初始化
 * @param  None.
 * @retval None.
 */
ErrStatus rcu_init(void)//主时钟AHB 200MHz, APB2 100MHz, APB1 50MHz
{
    uint32_t clock = 0;

    clock = rcu_clock_freq_get(CK_SYS);
    // clock = rcu_clock_freq_get(CK_AHB);
    // clock = rcu_clock_freq_get(CK_APB1);
    // clock = rcu_clock_freq_get(CK_APB2);
    if (!clock)
        return ERROR;

    // TODO
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //RTC BKPSRAM 放置在RTC文件处理
    // PWR_BackupAccessCmd(ENABLE);
    // if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050)
    // //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相符
    // {
    //     RCC_BackupResetCmd(ENABLE);  //复位备份区域
    //     RCC_BackupResetCmd(DISABLE); //复位备份区域
    //     RCC_LSEConfig(RCC_LSE_ON);
    // }

    /* enable DMA1 clock*/
    rcu_periph_clock_enable(RCU_DMA1);// AHB1 peripherals

    // GD32F450I
    // rcu_periph_clock_enable(RCU_IPA); // AHB1 peripherals
    /* enable EXMC clock*/
    rcu_periph_clock_enable(RCU_EXMC);// AHB3 peripherals
    rcu_periph_clock_enable(RCU_TLI); // APB2 peripherals
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_GPIOH);
    // GD32F450I
    // rcu_periph_clock_enable(RCU_GPIOI);

    /*
    if (rcu_flag_get(RCC_FLAG_HSERDY))
        return SUCCESS;
    else
        return ERROR;
    */
    return SUCCESS;
}