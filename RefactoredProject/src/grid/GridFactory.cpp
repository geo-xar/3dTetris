#include "GridFactory.h"
#include "Grid.h"

IGridPtrU GridFactory::createGrid()
{
    return std::make_unique<Grid>();
}
