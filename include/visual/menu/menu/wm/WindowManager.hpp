/*
  Created on 06.07.18.
*/

#pragma once

#include <menu/wm/WindowContainer.hpp>

namespace zerokernel
{

class WindowManager : public Container
{
public:
    ~WindowManager() override = default;

    WindowManager();

    void loadFromXml(const tinyxml2::XMLElement *data) override;

    // Functions

    void init();

    void loadWindowFromXml(const tinyxml2::XMLElement *data);

    WMWindow *findWindow(size_t uid);
    WMWindow *getActiveWindow();
    void focusOnWindow(size_t uid);

    // Sub-elements

    WindowContainer *container{ nullptr };
    size_t active_window{ 0 };
};
} // namespace zerokernel