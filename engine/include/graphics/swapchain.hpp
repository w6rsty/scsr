#pragma once

#include "image.hpp"
#include "window.hpp"

#include <vector>
#include <functional>

namespace scsr
{

class Swapchain
{
    PIN(Swapchain)
    SIG(Swapchain)
public:
    using WriteCommand = std::function<void(Ref<Image>)>;

    Swapchain(ImageProp prop, usize count);

    void PushWriteCommand(WriteCommand command);

    void AcquireAndWrite();
    void Present(Window& window);
private:
    std::vector<Ref<Image>> m_FrontBuffer;
    std::vector<Ref<Image>> m_BackBuffer;

    std::vector<WriteCommand> m_WriteCommands;
};

}