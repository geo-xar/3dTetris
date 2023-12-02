#include "MainWindowFactory.h"
#include "MainWindow.h"

IMainWindowPtrU MainWindowFactory::createMainWindow(int argc, char** argv)
{
    return std::make_unique<MainWindow>(argc, argv);
}
