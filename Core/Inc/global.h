/*
 * global.h
 *
 *  Created on: Dec 8, 2024
 *      Author: joaqu
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;

extern uint8_t buf;

extern float kp;
extern float ki;
extern float kd;
extern float sp;
extern float Ts;
extern uint8_t giro;
extern volatile uint32_t frequency;
#endif /* INC_GLOBAL_H_ */
