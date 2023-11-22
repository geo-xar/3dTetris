#pragma once

#include "IGrid.h"

class GridFactory
{
public:
    [[nodiscard]] static IGridPtrU createGrid();
};
