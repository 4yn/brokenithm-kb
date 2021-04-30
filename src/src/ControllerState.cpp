#include "ControllerState.hpp"

ControllerState::ControllerState() : m_button_state(0),
                                     m_button_state_staging(0) {}

void ControllerState::start()
{
    m_button_state_staging = 0;
}

void ControllerState::add_button(int i)
{
    m_button_state_staging |= button_lookup_table(i);
}

void ControllerState::end()
{
    m_button_state.store(m_button_state_staging);
}
