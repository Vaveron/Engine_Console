#pragma once
#include <windows.h>
#include <iostream>
#include <string>
class systemE{
    friend class Engine;
    friend class graphics;
    public:
        systemE(){
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    private:
        HANDLE hConsole = nullptr;
        COORD coord = { 0, 0 };
        void SetConsoleSize(int width, int height, int fontSize, std::string name);
        void Cursor();
};