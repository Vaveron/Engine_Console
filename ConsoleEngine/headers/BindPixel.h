#pragma once
#include <functional>
#include <iostream>
struct BindPixel
{
    int positionSelfX = 0;
    int positionSelfY = 0;
    int sizeX = 0;
    int sizeY = 0;
    int lines = 1;
    int lineCount = 5;
    std::function<std::wstring()> function;
};
