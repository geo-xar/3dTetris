#pragma once
#include <memory>

class IMainWindow
{
public:
    virtual ~IMainWindow() = default;
    virtual void draw() = 0;
};

using IMainWindowPtrU = std::unique_ptr<IMainWindow>;
