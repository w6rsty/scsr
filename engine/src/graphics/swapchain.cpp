#include "graphics/swapchain.hpp"

#include <Tracy.hpp>

namespace scsr
{

Swapchain::Swapchain(ImageProp prop, usize count)
{
    m_FrontBuffer.reserve(count);
    m_BackBuffer.reserve(count);

    for (usize i = 0; i < count; ++i)
    {
        m_BackBuffer.push_back(MakeRef<Image>(prop));
    }
}

void Swapchain::PushWriteCommand(WriteCommand command)
{
    m_WriteCommands.push_back(command);
}

void Swapchain::AcquireAndWrite()
{
    ZoneScoped;
    auto available = m_BackBuffer.back();
    
    for (auto& command : m_WriteCommands)
    {
        command(available);
    }
};

void Swapchain::Present(Window& window)
{
    ZoneScoped;
    auto available = m_BackBuffer.back();
    window.OnUpdate(m_BackBuffer.back());
}

}   