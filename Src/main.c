#include "main.h"
#include "hostx/cookbook.h"
#include "hostx/intercom.h"
#include "hostx/vm.h"

extern COOK_RECEIPT  RECEIPT_ComPort;
extern COOK_RECEIPT  RECEIPT_CRC32;

int main(void)
{
    SystemClock_Config();
    
    //VM_HostCommand( VM_PAUSE         , &sys_pause            , 150600);
    //VM_HostCommand( VM_RESTART       , &sys_restart          , 1);
    //VM_HostCommand( VM_TERMINATE     , &sys_terminate        , 1);
    
    INTERCOM_CreateChannel( 0, &RECEIPT_ComPort );
    //INTERCOM_CreateChannel( RECEIPT_Dummy    ,32 ,32 );
    //INTERCOM_CreateChannel( RECEIPT_Dummy    ,32 ,32 );
    
    loop:while(1)
    {
        VM_Update();
    }
}

void DMA1_Channel4_IRQHandler(void)
{ 
    IO_CHANNEL* T = INTERCOM_GetChannel(0);
    
    T->Handler_RX( T ); 
}

void DMA1_Channel5_IRQHandler(void)
{ 
    IO_CHANNEL* T = INTERCOM_GetChannel(0);
    
    T->Handler_TX( T ); 
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 24000000
 *            HCLK(Hz)                       = 24000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            HSE PREDIV1                    = 2
 *            PLLMUL                         = 6
 *            Flash Latency(WS)              = 0
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
    /* Enable HSE oscillator */
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1)
    {
    };
    
    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_2, LL_RCC_PLL_MUL_6);
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1)
    {
    };
    
    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    };
    
    /* Set APB1 & APB2 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    
    /* Set systick to 1ms */
    SysTick_Config(24000000 / 1000);
    
    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    SystemCoreClock = 24000000;
}
