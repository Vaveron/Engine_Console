#pragma once
#include "BindPixel.h"
#include <vector>
#include <windows.h>
class UIunit {
    public:
        int positionX =0;
        int positionY =0;
        int sizePanelX = 0;
        int sizePanelY = 0;
        //virtual void JsonImport(int resourceID);
        bool Active = true;
        std::vector<std::vector<wchar_t>> PanelPixels;
        std::vector<std::vector<WORD>> PanelPixelsColors;
        std::vector<BindPixel> BindPixels;
};  