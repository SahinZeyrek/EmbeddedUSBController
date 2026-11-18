#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "button.h"
#include "buttonManager.h"


bool b_isON = false;
void TurnONLED(uint gpio, uint32_t event_mask)
{
    b_isON = !b_isON;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, b_isON);
};
void TurnOFFLED(uint gpio, uint32_t event_mask) 
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
};


constexpr uint gpio_buttonPIN = 2;

int main()
{
    stdio_init_all();
    sleep_ms(3 * 1000);

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    ButtonManager& manager=  ButtonManager::GetButtonManager();
    manager.AddButton          (gpio_buttonPIN, 50 * 1000);
    manager.StartHandleCallback(gpio_buttonPIN);
    // Example to turn on the Pico W LED
    //gpio_init(gpio_buttonPIN);
    //gpio_set_dir(gpio_buttonPIN, false);
    //gpio_pull_up(gpio_buttonPIN);
    //gpio_set_irq_enabled_with_callback(gpio_buttonPIN,GPIO_IRQ_EDGE_FALL,true,TurnONLED);    


    while (true) { 
    //  printf("Hello, world!\n");

        manager.CheckAllButtonHoldState();
        tight_loop_contents();
        
    }
}
