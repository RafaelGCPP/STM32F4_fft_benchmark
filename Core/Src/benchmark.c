/*
 * benchmark.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Rafael_2
 */

#include <stdlib.h>
#include <stdio.h>
#include <dsp/transform_functions.h>
#include <dsp/support_functions.h>
#include <stm32f4xx_ll_cortex.h>

uint32_t getCurrentMicros() {
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

void time_benchmark() {
	printf("-=-=-=-=-=-= Tick counter benchmark =-=-=-=-=-=-\n\r");

	unsigned int start = getCurrentMicros();
	HAL_Delay(500);
	unsigned int elapsed = getCurrentMicros() - start - 960;
	printf("This should read 500000: %d\n\r", elapsed);
	start = getCurrentMicros();
	HAL_Delay(1);
	elapsed = getCurrentMicros() - start- 960;
	printf("This should read 1000: %d\n\r", elapsed);
	start = getCurrentMicros();
	HAL_Delay(100);
	elapsed = getCurrentMicros() - start- 960;
	printf("This should read 100000: %d\n\r", elapsed);

}

void fft_benchmark() {

	puts("-=-=-=-=-=-= Real FFT benchmark - floating point =-=-=-=-=-=-\r");

	float data[2048];
	int n, l2n;

	puts("Generating sample data.\r\n");
	for (int i = 0; i < 2048; i++) {
		data[i] = ((float) rand() / RAND_MAX) * 2 - 1;
	}

	for (l2n = 5; l2n < 12; l2n++) {
		n = 1 << l2n;
		arm_rfft_fast_instance_f32 fft_engine;

		arm_rfft_fast_init_f32(&fft_engine, n);

		printf("%4d-points real-valued floating-point FFT: ", n);

		unsigned int start = getCurrentMicros();
		for (int i = 0; i < 500; i++) {
			float fft[n];
			arm_rfft_fast_f32(&fft_engine, data, fft, 0);
			arm_rfft_fast_f32(&fft_engine, fft, data, 1);
		}
		unsigned int elapsed = getCurrentMicros() - start- 960 ;
		printf("%12.5fus per transform\n\r", (float) elapsed / 1000);
	}
}


void fft_benchmark_fixed_point() {

	puts("-=-=-=-=-=-= Real FFT benchmark - fixed point =-=-=-=-=-=-\r");

	long int data[2048];
	float fdata[2048];
	int n, l2n;

	puts("Generating sample data.\r\n");
	for (int i = 0; i < 2048; i++) {
		fdata[i] = ((float) rand() / RAND_MAX) * 2 - 1;
	}
	arm_float_to_q31(fdata, data, 2048);

	for (l2n = 5; l2n < 12; l2n++) {
		n = 1 << l2n;
        arm_rfft_instance_q31 fft_instance, ifft_instance;

        arm_rfft_init_q31(&fft_instance, n, 0, 1);
        arm_rfft_init_q31(&ifft_instance, n, 0, 1);

		printf("%4d-points real-valued fixed-point FFT: ", n);

		unsigned int start = getCurrentMicros();
		for (int i = 0; i < 500; i++) {
            long int fft[2 * n];
            long int data_out[n];
            arm_rfft_q31(&fft_instance, data, fft);
            arm_rfft_q31(&ifft_instance, fft, data_out);
		}
		unsigned int elapsed = getCurrentMicros() - start- 960 ;
		printf("%12.5fus per transform\n\r", (float) elapsed / 1000);
	}
}
