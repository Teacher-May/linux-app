#include "gpio.h"

#define BEEP_GPIO	(40)


int main(int argc, char **argv)
{
    int count = 3;
	gpio_open(BEEP_GPIO);
	gpio_set_direction(BEEP_GPIO, "out");

	while(count--){
		gpio_set_value(BEEP_GPIO, 1);
		printf("gpio value=%d\n", gpio_get_value(BEEP_GPIO));
		sleep(1);

		gpio_set_value(BEEP_GPIO, 0);
		printf("gpio value=%d\n", gpio_get_value(BEEP_GPIO));
		sleep(1);
	}

	return 0;
}

