#pragma once

#include "IMainWindow.h"
#include "gmock/gmock.h"

class MainWindowMock : public IMainWindow
{
public:
    MOCK_METHOD0(draw, void(void));
};

using MainWindowMockPtrU = std::unique_ptr<MainWindowMock>;
