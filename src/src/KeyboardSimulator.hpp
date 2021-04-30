#include <memory>

struct KeyboardSimulator
{
    enum KeyboardSimulatorLayout
    {
        LAYOUT_YUANCON,
        LAYOUT_TASOLLER
    };

    struct Impl;
    std::unique_ptr<Impl> m_impl;

    KeyboardSimulator(KeyboardSimulatorLayout layout = LAYOUT_YUANCON);
    ~KeyboardSimulator();

    void send(uint64_t keys);
    void delay(int millis);
};
