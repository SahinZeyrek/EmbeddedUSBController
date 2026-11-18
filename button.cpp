#include "button.h"
#include <stdio.h>
#include "pico/cyw43_arch.h"
button::button(uint gpioPin, uint64_t desiredDebounceTime)
    : m_gpioPinID{gpioPin}, m_desiredDebounceTime{desiredDebounceTime}
{
    // change to functions outside the button class.
    m_onPress   =   std::bind(&button::onPress,   this);
    m_onRelease =   std::bind(&button::onRelease, this);
    m_onHold    =   std::bind(&button::onHold,    this);

}

button::~button()
{
}

void button::onPress()
{
    // call functor here
    b_didPress = true;
    b_isHoldingDown = true;
    printf("On Press \n");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // change after functor call
}


// add Held time
void button::onRelease()
{
    b_didPress = false;
    b_isHoldingDown = false;

    // call on release functor, pass total held time as well
    printf("On Release \n");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

}

bool button::IsHolding()  
{ 
    return b_isHoldingDown;
}

// add Holding time
void button::onHold()
{

    // call functor here, pass current hold time 

    printf("On Hold \n");

    //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);


}

uint64_t button::SaveLastRecordedTime()
{
    m_LastRecordedTime = time_us_64();
    return m_LastRecordedTime;
}

const bool button::HandleCallback(uint32_t event_mask)
{
    // handle each tick GPIO_IRQ_LEVEL_LOW
    bool isEdgeFall = event_mask  & GPIO_IRQ_EDGE_FALL;
    bool isLevelLow = event_mask  & GPIO_IRQ_LEVEL_LOW;
    bool isEdgeRise = event_mask  & GPIO_IRQ_EDGE_RISE;
    bool isLevelHigh = event_mask & GPIO_IRQ_LEVEL_HIGH;

   //if (isLevelLow && b_isHoldingDown && m_onHold)
   //{
   //    m_onHold();
   //}
    //if (isLevelLow && !isEdgeFall)
    //{
    //    // handle on hold stuff
    //    printf("On Hold \n");
    //    return true;
    //}

    // check debounce timer, if value is past desired time then send true.
    uint64_t currentTime = time_us_64();
    if (currentTime - m_LastRecordedTime < m_desiredDebounceTime)
        return false;

    SaveLastRecordedTime();
    if (isEdgeRise && m_onRelease)
    {
        m_onRelease();

        return true;
    }

    if (isEdgeFall && m_onPress)
    {
        m_onPress();
        return true;
    }

 

    return false;
}
