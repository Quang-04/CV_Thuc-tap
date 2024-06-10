#include "Led_button.h"
#include <main.h>
#define Set_bit(REG, VAL, POS) *REG=*REG|(VAL<<POS)
#define Clean_bit(REG, VAL, POS) *REG=*REG&~(VAL<<POS)
#define Read_bit(REG, POS) ((*REG>>POS)&1)

#define GPIOA_BASE 0x40020000
#define GPIOD_BASE 0x40020C00
#define EXTI_BASE 0x40013C00
#define NVIC_BASE 0xE000E100
#define TIMER_BASE 0x40010000
// Set led_init is output(12,13,14,15)
void led_init(){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t *GPIOD_Moder=(uint32_t*)(GPIOD_BASE+0x00);
	Clean_bit(GPIOD_Moder,0b11111111,24);
	Set_bit(GPIOD_Moder,0b01010101,24);
}
//Set button_init is input
void button_init(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t *GPIOA_Moder=(uint32_t*)(GPIOA_BASE+0x00);
	Clean_bit(GPIOA_Moder,0b11,0);
}

//control led
void control_led(Colour_led led,state state){
	uint32_t *GPIOD_ODR=(uint32_t*)(GPIOD_BASE+0x14);
	if(state==Led_on){
	Set_bit(GPIOD_ODR,1,led);
}
	else Clean_bit(GPIOD_ODR,1,led);
}
state read_button(){
	uint32_t *GPIOA_IDR=(uint32_t*)(GPIOA_BASE+0x10);
	return Read_bit(GPIOA_IDR,0);
}
void fuction_button(){
	if(read_button()==1){
		 control_led(Led_orange,Led_on);
	}
	else control_led(Led_orange,Led_off);
	uint32_t* EXTI_PR = (uint32_t*)(EXTI_BASE +0x14);
	Set_bit(EXTI_PR ,1,0);
}
void EXTI_init(){
	// CALL FALLING
	uint32_t* EXTI_FTSR=(uint32_t*)(EXTI_BASE + 0x0C);
	Set_bit(EXTI_FTSR,1,0);
	//CALL RISING
	uint32_t* EXTI_RTSR=(uint32_t*)(EXTI_BASE+ 0x08);
	Set_bit(EXTI_RTSR,1,0);
	//CALL MASK
	uint32_t* EXTI_IMR =(uint32_t*)(EXTI_BASE+0x00);
	Set_bit(EXTI_IMR,1,0);
	//NVIC accept
	uint32_t* NVIC_0=(uint32_t*)(NVIC_BASE);
	Set_bit(NVIC_0,1,6);
	// MOVE vector table
	memcpy(0x20000000,0x00,0x194);
	uint32_t* VTOR=0xE000ED08;
	*VTOR=0x20000000;
	uint32_t* px=0x20000058;
	*px =(int) fuction_button|1;
}
void Custom_Delay1(int time){
	__HAL_RCC_TIM1_CLK_ENABLE();
	uint16_t *TIMER_PSC=(uint16_t*)(TIMER_BASE+ 0x28);
	uint16_t *TIMER_ARR=(uint16_t*)(TIMER_BASE+ 0x2C);
	*TIMER_PSC=16000-1;
	uint16_t* TIMER_CR1=(uint16_t*)(TIMER_BASE+ 0x00);
	Set_bit(TIMER_CR1,1,0);
	Custom_Delay_wating();
}
void Custom_Delay_wating(){
	uint16_t *TIMER_SR=(uint16_t*)(TIMER_BASE+ 0x10);
	while(Read_bit(TIMER_SR,0)==0);
	Clean_bit(TIMER_SR,1,0);
}
