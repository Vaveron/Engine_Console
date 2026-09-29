#include "../headers/System.h"
#include <windows.h>
#include <iostream>

void systemE::SetConsoleSize(int width, int height, int fontSize, std::string name){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // ==========================================
    // 1. Получаем HWND
    // ==========================================
    HWND consoleWindow = GetConsoleWindow();
    // ==========================================
    // 2. Убираем ограничения размера окна
    // ==========================================
    if (consoleWindow){
        LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
        style &= ~WS_MAXIMIZEBOX;
        style &= ~WS_THICKFRAME;
        SetWindowLong(consoleWindow, GWL_STYLE, style);
        SetWindowPos(consoleWindow,nullptr,0, 0, 0, 0,SWP_NOMOVE |SWP_NOSIZE |SWP_NOZORDER |SWP_FRAMECHANGED);
    }
    // ==========================================
    // 3. Сначала максимально уменьшаем окно
    // ==========================================
    SMALL_RECT tinyWindow = {0,0,0,0};
    if (!SetConsoleWindowInfo(hConsole,TRUE,&tinyWindow)){
        std::cout<< "ERROR tiny window: "<< GetLastError()<< std::endl;
    }
    // ==========================================
    // 4. Меняем шрифт
    // ==========================================
    CONSOLE_FONT_INFOEX fontInfo{};
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (!GetCurrentConsoleFontEx(hConsole,FALSE,&fontInfo)){
        std::cout<< "ERROR get font: "<< GetLastError()<< std::endl;
    }
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = fontSize;
    if (!SetCurrentConsoleFontEx(hConsole,FALSE,&fontInfo)){
        std::cout<< "ERROR set font: "<< GetLastError()<< std::endl;
    }
    // ==========================================
    // 5. Устанавливаем buffer
    // ==========================================
    COORD bufferSize = {static_cast<SHORT>(width),static_cast<SHORT>(height)};
    if (!SetConsoleScreenBufferSize(hConsole,bufferSize)){
        std::cout<< "ERROR buffer: "<< GetLastError()<< std::endl;
    }
    // ==========================================
    // 6. Только теперь устанавливаем окно
    // ==========================================
    SMALL_RECT windowSize = {0,0,static_cast<SHORT>(width - 1),static_cast<SHORT>(height - 1)};
    if (!SetConsoleWindowInfo(hConsole,TRUE,&windowSize)){
        std::cout<< "ERROR window: "<< GetLastError()<< std::endl;
    }
    // ==========================================
    // 7. Проверяем, что реально получилось
    // ==========================================
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (GetConsoleScreenBufferInfo(hConsole,&info)){
        int realWidth =info.srWindow.Right -info.srWindow.Left + 1;
        int realHeight =info.srWindow.Bottom -info.srWindow.Top + 1;
        std::cout<< "Requested: "<< width << " x "<< height<< std::endl;
        std::cout<< "Actual: "<< realWidth << " x "<< realHeight<< std::endl;
    }
    SetConsoleTitleA(name.c_str());
}


void systemE::Cursor(){
    CONSOLE_CURSOR_INFO cursorInfo{};
    if(hConsole == INVALID_HANDLE_VALUE || hConsole == nullptr) return;
    if(!GetConsoleCursorInfo(hConsole, &cursorInfo)) return;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    SetConsoleCursorPosition(hConsole, coord);
}