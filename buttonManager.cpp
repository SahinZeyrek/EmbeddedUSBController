#include "buttonManager.h"
#include "button.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include <stdio.h>

ButtonManager::ButtonManager()
{

}

ButtonManager::~ButtonManager()
{

}

ButtonManager& ButtonManager::GetButtonManager()
{
    static ButtonManager instance;
    return instance;
}

void ButtonManager::Init(button* buttonToInit)
{
    if(!buttonToInit)
    {
        printf("Failed to init button");
        return;
    } 

    uint id = buttonToInit->GetPinID();
    gpio_init(id);
    gpio_set_dir(id, false);
    gpio_pull_up(id);
    printf("IsHolding: %d", buttonToInit->IsHolding());
}

button* ButtonManager::AddButton(uint gpioPin, microseconds desiredDebounceTime)
{
    std::unique_ptr<button> buttonToAdd = std::make_unique<button>(gpioPin,desiredDebounceTime);
    button* toReturn = buttonToAdd.get();
    Init(toReturn);
    m_Buttons.emplace_back(std::move(buttonToAdd));
    return toReturn;
}

void ButtonManager::CheckAllButtonHoldState()
{
    for (const auto& button : m_Buttons)
    {
        if (button->IsHolding())
        {
            // send is holding signals
            button->onHold(); 
        } 
    }
    
}

void ButtonManager::StartHandleCallback(uint gpioPin)
{
    gpio_set_irq_enabled_with_callback(gpioPin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, HandleCallback);
}

void ButtonManager::HandleCallback(uint gpioPin, uint32_t event_mask)
{
    for (std::unique_ptr<button>& gpioButton : GetButtonManager().m_Buttons)
    {
        if (gpioButton->GetPinID() == gpioPin)
        {
             gpioButton->HandleCallback(event_mask);
            // handle button unique stuff.
        }
    }
    
}
