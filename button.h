#include "pico/stdlib.h"
#include "functional"
class button
{
public:
    explicit button(uint gpioPin, uint64_t desiredDebounceTime);
    ~button();

    void onPress();
    void onRelease();
    bool IsHolding();
    void onHold();

    uint64_t SaveLastRecordedTime();
    const bool HandleCallback(uint32_t event_mask);
    uint GetPinID(){return m_gpioPinID;};

    button& operator=(button&& )        = delete;
    button(button&& )                   = delete;
    button& operator=(const button&)    = delete;
    button(const button&)               = delete;

private:
    /* data */
    uint     m_gpioPinID{};
    uint64_t m_LastRecordedTime{};
    uint64_t m_desiredDebounceTime{};

    std::function<void()> m_onPress;
    std::function<void()> m_onRelease;
    std::function<void()> m_onHold;

    volatile bool b_didPress =      false;
    volatile bool b_isHoldingDown = false;

};


