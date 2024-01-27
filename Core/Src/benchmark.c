/*
 * benchmark.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Rafael_2
 */

#include <stdlib.h>
#include <stdio.h>
#include <dsp/transform_functions.h>
#include <stm32f4xx_ll_cortex.h>

uint32_t getCurrentMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (LL_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

void fft_benchmark()
{

    puts("-=-=-=-=-=-= Real FFT benchmark - floating point =-=-=-=-=-=-");

    float data[2048];
    int n, l2n;

    puts("Generating sample data.");
    for (int i = 0; i < 2048; i++)
    {
        data[i] = ((float)rand()/RAND_MAX)*2-1;
    }

    for (l2n = 3; l2n < 12; l2n++)
    {
        n = 1 << l2n;
        arm_rfft_fast_instance_f32 fft_engine;

        arm_rfft_fast_init_f32(&fft_engine, n);

        printf("%4d-points real-valued fixed-point FFT: ", n);

        unsigned int start = getCurrentMicros();
        for (int i = 0; i < 50; i++)
        {
        	float fft[n];
        	arm_rfft_fast_f32(&fft_engine, data, fft, 0);
        	arm_rfft_fast_f32(&fft_engine, fft, data, 1);
        }
        unsigned int elapsed = getCurrentMicros() - start;
        printf("%12.5fus per transform\n", (float)elapsed / 100);
    }
}
