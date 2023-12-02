#pragma once

#include "IMainWindow.h"

class MainWindowFactory
{
public:
    [[nodiscard]] static IMainWindowPtrU createMainWindow(int argc, char** argv);
};
