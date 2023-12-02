#pragma once

#include "IMainWindow.h"
#include <optional>

class MainWindow final : public IMainWindow
{
public:
    MainWindow(int argc, char** argv);
    void draw() override;
private:
    std::tuple<int,int> getScreenResolution();
    void setWindowPositionAndSize();
    int _argc;
    char** _argv;
};

struct Color
{
    float red;
    float green;
    float blue;
    float alpha;
};
