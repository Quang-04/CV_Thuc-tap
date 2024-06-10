#include <main.h>
#include "led_button.h"

int main(){
	HAL_Init();
	led_init();
	button_init();
	EXTI_init();
	while(1){
		control_led(led_green,Led_on);
		Custom_Delay1(1000);
		control_led(led_green,Led_off);
		Custom_Delay1(1000);
	}
}
