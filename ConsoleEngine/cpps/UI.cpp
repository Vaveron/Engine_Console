#include "../headers/UI.h"
#include <windows.h>
#include <vector>

void UI::AddUItoFrame(){
    for(auto& panel : Panels){
        if(panel->Active == true){
            for(auto& bindfunc : panel->BindPixels){
                std::wstring return_ = bindfunc.function();
                int number = 0;
                for(wchar_t symbol : return_){
                    std::vector<std::vector<wchar_t>> CharBIG =RenderChar(symbol,bindfunc.sizeX / bindfunc.lineCount,bindfunc.sizeY / bindfunc.lines);
                    for(int y = 0; y < bindfunc.sizeY / bindfunc.lines; y++){
                        for(int x = 0; x < bindfunc.sizeX / bindfunc.lineCount; x++){
                            panel->PanelPixels[bindfunc.positionSelfY + y][bindfunc.positionSelfX + x + (bindfunc.sizeX / bindfunc.lineCount) * number]= CharBIG[y][x];
                        }
                    }
                    number++;
                }
            }
            for(int y =0; y<panel->sizePanelY; y++){
                for(int x =0; x<panel->sizePanelX; x++){

                    gr->Frame[y+panel->positionY][x+panel->positionX] = panel->PanelPixels[y][x];
                    gr->ColorFrame[y+panel->positionY][x+panel->positionX] = panel->PanelPixelsColors[y][x];
                }
            }
        }
    }
}

std::vector<std::vector<wchar_t>> UI::RenderChar( wchar_t character, int width,int height){
    // ==========================================
    // Создаём DC
    // ==========================================
    HDC hdc = GetDC(nullptr);
    HDC memDC = CreateCompatibleDC(hdc);
    // ==========================================
    // Bitmap
    // ==========================================
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes =1;
    bmi.bmiHeader.biBitCount =32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pixels = nullptr;
    HBITMAP bitmap = CreateDIBSection(memDC,&bmi,DIB_RGB_COLORS,&pixels,nullptr,0);
    SelectObject(memDC,bitmap);
    // ==========================================
    // Чёрный фон
    // ==========================================
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    HBRUSH blackBrush =CreateSolidBrush(RGB(0,0,0));
    FillRect(memDC,&rect,blackBrush);
    DeleteObject(blackBrush);

    // ==========================================
    // Белый символ
    // ==========================================
    SetTextColor(memDC,RGB(255,255,255));
    SetBkColor(memDC,RGB(0,0,0));
    SetBkMode(memDC,OPAQUE);
    // ==========================================
    // Шрифт
    // ==========================================
    HFONT font = CreateFontW(height,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH | FF_DONTCARE,L"Consolas");
    SelectObject(memDC,font);
    // ==========================================
    // Размер символа
    // ==========================================
    SIZE textSize{};
    GetTextExtentPoint32W(memDC,&character,1,&textSize);
    // ==========================================
    // Центрируем символ
    // ==========================================
    int x =(width - textSize.cx) / 2;
    int y =(height - textSize.cy) / 2;
    // ==========================================
    // Рисуем символ
    // ==========================================
    TextOutW(memDC,x,y,&character,1);
    // ==========================================
    // Создаём результат
    // ==========================================
    std::vector<std::vector<wchar_t>> result(height,std::vector<wchar_t>(width,L' '));
    // ==========================================
    // Bitmap → wchar_t
    // ==========================================
    unsigned char* data =static_cast<unsigned char*>(pixels);
    for(int py = 0; py < height; py++){
        for(int px = 0;px < width;px++){
            int index =(py * width + px) * 4;
            unsigned char B =data[index + 0];
            unsigned char G = data[index + 1];
            unsigned char R = data[index + 2];
            int brightness =(static_cast<int>(R) +static_cast<int>(G) +static_cast<int>(B)) / 3;
            if(brightness > 80) result[py][px] =L'\u2588';
            else result[py][px] =L' ';
        }
    }

    // ==========================================
    // Очистка
    // ==========================================
    DeleteObject(font);
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(nullptr,hdc);
    return result;
}