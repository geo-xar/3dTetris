#pragma once

#include "IGrid.h"
#include "gmock/gmock.h"

class GridMock : public IGrid
{
public:
    MOCK_METHOD0(draw, void(void));
};

using GridMockPtrU = std::unique_ptr<GridMock>;
