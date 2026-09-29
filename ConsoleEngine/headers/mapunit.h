#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <functional>
#include "../json.hpp"
#include "../headers/RayCastReturn.h"
using json = nlohmann::json;

class MapUnit{
    friend class Engine;
    public:
        virtual ~MapUnit() = default;
        wchar_t Unit;
        std::string typeUnit = "";
        int width;
        int height;
        std::vector<std::vector<wchar_t>> pixels;
        std::vector<std::vector<WORD>> colors;
        virtual void watching(RayCastReturn* RayCast){}; 
        virtual void Startwatching(RayCastReturn* RayCast){};
        virtual void TextureImport(std::wstring Texture_str,std::wstring Texture_color_str,int x,int y);
        virtual void JsonImport(int resourceID);
        virtual RayCastReturn RayCastSelf(int mapX, int mapY, int side, float PlayerPosX, float PlayerPosY, int stepX, int stepY, float rayDirX, float rayDirY);
        virtual std::unique_ptr<MapUnit> Clone() const{
            return std::make_unique<MapUnit>(*this);
        }
        int CharToColor(wchar_t c);
        WORD Foreground[16] = {
            0,
            FOREGROUND_RED,
            FOREGROUND_GREEN,
            FOREGROUND_BLUE,
            FOREGROUND_RED | FOREGROUND_GREEN,
            FOREGROUND_RED | FOREGROUND_BLUE,
            FOREGROUND_GREEN | FOREGROUND_BLUE,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_INTENSITY,
            FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };
};


/*
0  → чёрный
1  → красный
2  → зелёный
3  → синий
4  → жёлтый
5  → пурпурный
6  → голубой
7  → серый
8  → тёмно-серый
9  → ярко-красный
10 → ярко-зелёный
11 → ярко-синий
12 → ярко-жёлтый
13 → ярко-пурпурный
14 → ярко-голубой
15 → белый
*/