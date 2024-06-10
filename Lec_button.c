#ifndef CUSTOM_DRIVER_INC_LED_BUTTON_H_
#define CUSTOM_DRIVER_INC_LED_BUTTON_H_

typedef enum {
	led_green =12,
	Led_orange,
	led_red,
	Led_blue
}Colour_led;
typedef enum{
	Led_off,
	Led_on
}state;
void led_init();
void button_init();
void control_led(Colour_led led,state state);
state read_button();
void EXTI_init();

void Custom_Delay1(int time);

#endif /* CUSTOM_DRIVER_INC_LED_BUTTON_H_ */
