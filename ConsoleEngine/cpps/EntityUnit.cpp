#include "../headers/EntityUnit.h"
#include <iostream>
#include <cmath>
#include <algorithm>
int EntityUnit::CharToColor(wchar_t c){ 
    if(c >= L'0' && c <= L'9') return c - L'0'; 
    if(c >= L'A' && c <= L'F') return c - L'A' + 10; 
    if(c >= L'a' && c <= L'f') return c - L'a' + 10; return 0; 
}
void EntityUnit::JsonImport(int resourceID){
    HRSRC resource =FindResourceW(nullptr,MAKEINTRESOURCEW(resourceID),MAKEINTRESOURCEW(RT_RCDATA));
    if(!resource){
        std::cout<< resourceID;
       std::system("pause");
        return;
    }
    HGLOBAL data =LoadResource(nullptr,resource);
    DWORD size =SizeofResource(nullptr,resource);
    const char* buffer = static_cast<const char*>(LockResource(data));
    json Data = json::parse(std::string(buffer, size));
    if (Data.contains("type-rc") && Data["type-rc"] == 2){
        //if (Data.contains("doorAngle")) Doorangle = Data["doorAngle"];
        if (Data.contains("width")) width = Data["width"];
        if (Data.contains("height")) height = Data["height"];
        if (Data.contains("sizeX")) sizeX = Data["sizeX"];
        if (Data.contains("sizeY")) sizeY = Data["sizeY"];
        if (Data.contains("positionX")) positionX = Data["positionX"];
        if (Data.contains("positionY")) positionY = Data["positionY"];
        if (Data.contains("moveSpeed")) moveSpeed = Data["moveSpeed"];
        if (Data.contains("StandartModeBehavoir")) StandartModeBehavoir = Data["StandartModeBehavoir"];
        if(Data.contains("pixels") && Data.contains("colors")){
            pixels.resize(height,std::vector<wchar_t>(width));
            colors.resize(height,std::vector<WORD>(width));
            for(int y_ = 0;y_ < height ;y_++){
                std::string Texture_str = Data["pixels"][y_];
                int size_Texture_str = MultiByteToWideChar(CP_UTF8, 0,Texture_str.c_str(),-1, nullptr, 0);
                std::wstring Texture_str_w(size_Texture_str,L'\0');
                MultiByteToWideChar( CP_UTF8, 0, Texture_str.c_str(), -1, &Texture_str_w[0], size_Texture_str);
                Texture_str_w.resize(size_Texture_str - 1);
                std::string Texture_color_str = Data["colors"][y_];
                for(int x_ = 0;x_ < width;x_++){
                    pixels[y_][x_] = Texture_str_w[x_];
                    colors[y_][x_] = Foreground[CharToColor(Texture_color_str[x_])];
                }
            }
        }
    }
}

RayCastReturn EntityUnit::RayCastSelf(float PlayerPosX,float PlayerPosY,float rayDirX,float rayDirY){

    RayCastReturn result;

    // ==========================================
    // По умолчанию попадания нет
    // ==========================================
    result.dis = -1.0f;
    result.textureX = 0.0f;
    result.side = 0;
    result.entity = nullptr;
    result.unit = nullptr;

    // ==========================================
    // Направление от Entity к игроку
    // ==========================================
    float normalX = positionX - PlayerPosX;
    float normalY = positionY - PlayerPosY;

    // ==========================================
    // Длина направления
    // ==========================================
    float normalLength =
        sqrt(
            normalX * normalX +
            normalY * normalY
        );

    if(normalLength < 0.00001f){
        return result;
    }

    // ==========================================
    // Нормализуем направление
    // ==========================================
    normalX /= normalLength;
    normalY /= normalLength;

    // ==========================================
    // Направление вправо относительно Entity
    // ==========================================
    float rightX = -normalY;
    float rightY = normalX;

    // ==========================================
    // Положение Entity относительно игрока
    // ==========================================
    float entityX =
        positionX - PlayerPosX;

    float entityY =
        positionY - PlayerPosY;

    // ==========================================
    // Расстояние Entity по направлению взгляда
    // ==========================================
    float cameraDistance =
        entityX * normalX +
        entityY * normalY;

    // ==========================================
    // Entity находится за игроком
    // ==========================================
    if(cameraDistance <= 0.0f){
        return result;
    }

    // ==========================================
    // Пересечение луча с плоскостью спрайта
    // ==========================================
    float denominator =
        rayDirX * normalX +
        rayDirY * normalY;

    if(fabs(denominator) < 0.00001f){
        return result;
    }

    // ==========================================
    // Расстояние по лучу
    // ==========================================
    float distance =
        cameraDistance / denominator;

    if(distance <= 0.0f){
        return result;
    }

    // ==========================================
    // Точка попадания луча
    // ==========================================
    float hitX =
        PlayerPosX +
        rayDirX * distance;

    float hitY =
        PlayerPosY +
        rayDirY * distance;

    // ==========================================
    // Положение точки попадания относительно Entity
    // ==========================================
    float localX =
        (hitX - positionX) * rightX +
        (hitY - positionY) * rightY;

    // ==========================================
    // Проверяем ширину спрайта
    // ==========================================
    float halfWidth =
        sizeX / 2.0f;

    if(localX < -halfWidth ||
       localX > halfWidth){
        return result;
    }

    // ==========================================
    // X текстуры
    // ==========================================
    float textureX =
        (localX + halfWidth) / sizeX;
    // ==========================================
    // Ограничиваем X текстуры
    // ==========================================
    if(textureX < 0.0f){
        textureX = 0.0f;
    }

    if(textureX > 1.0f){
        textureX = 1.0f;
    }
    if (static_cast<int>(textureX * width) == L' ') return result;
    // ==========================================
    // Результат
    // ==========================================
    result.dis = distance;
    result.textureX = textureX;
    result.side = 0;
    result.entity = this;

    return result;
}