#pragma once
#include "../headers/RayCastReturn.h"
#include "../headers/Logics.h"
#include "../headers/System.h"
#include "../headers/DoorUnit.h"
#include "../headers/wallUnit.h"

class graphics {
    friend class Engine;
    friend class UI;
    private:
        RayCastReturn RayCast(float angle);
        int FOV = 60;
        int SizeWindows[2];
        // ==========================================
        // Высота камеры
        //
        // Стена имеет высоту 1.25
        // Камера находится посередине стены
        // ==========================================
        float cameraHeight = 1.25f / 2.0f;
        void ConsoleExport();
        void ChangeFrame();
        std::vector<std::vector<wchar_t>> Frame;
        std::vector<std::vector<WORD>> ColorFrame; // Храним цвета (WORD - это 16-битное число)
        Logics* lo;
        systemE* sys;
};