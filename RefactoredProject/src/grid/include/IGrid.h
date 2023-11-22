#pragma once
#include <memory>

class IGrid
{
public:
    virtual ~IGrid() = default;
    virtual void draw() = 0;
};

using IGridPtrU = std::unique_ptr<IGrid>;
