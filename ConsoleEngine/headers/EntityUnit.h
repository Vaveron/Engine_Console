#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include "../headers/RayCastReturn.h"
#include "../json.hpp"
using json = nlohmann::json;

class EntityUnit{
    friend class Engine;
    public:
        float positionX;
        float positionY;
        float sizeX = 1.0f;
        float sizeY = 1.0f;
        int width;
        int height;
        std::string StandartModeBehavoir = " ";
        float moveSpeed;
        std::vector<std::vector<wchar_t>> pixels;
        std::vector<std::vector<WORD>> colors;
        virtual void JsonImport(int resourceID);
        virtual void Update(){}; 
        virtual void Start(){};
        virtual RayCastReturn RayCastSelf(float PlayerPosX, float PlayerPosY, float rayDirX, float rayDirY);
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