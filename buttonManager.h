#include <memory>
#include <vector>
#include "pico/stdlib.h"

using microseconds = uint64_t;
class button;
class ButtonManager {
public:
    ButtonManager();
    ~ButtonManager();
    static ButtonManager& GetButtonManager();

    void        Init(button* buttonToInit);
    button*     AddButton(uint gpioPin, uint64_t desiredDebounceTime);
    void CheckAllButtonHoldState();
    void StartHandleCallback(uint gpioPin);

    ButtonManager(const ButtonManager& other)            = delete;
    ButtonManager& operator=(const ButtonManager& other) = delete;
    ButtonManager(ButtonManager&& other)                 = delete;
    ButtonManager& operator=(ButtonManager&& other)      = delete;

   private:
    static void HandleCallback(uint gpioPin, uint32_t event_mask);
    std::vector<std::unique_ptr<button>> m_Buttons;

};

