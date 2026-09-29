#include "../headers/DoorUnit.h"


RayCastReturn DoorUnit::RayCastSelf(int mapX, int mapY, int side, float PlayerPosX, float PlayerPosY, int stepX, int stepY, float rayDirX, float rayDirY){
    // ==========================================
    // ДВЕРЬ
    // ==========================================
    float DoorOffset = 0.5f;
    float DoorOpenOffset = 0.1f;
    float DoorThickness = 0.15f;
    float doorDistance;
    float textureX;
    // ======================================
    // Ширина двери
    //
    // doorPosition = 1 -> 1.0
    // doorPosition = 0 -> 0.1
    // ======================================
    float doorWidth =DoorOpenOffset +(1.0f - DoorOpenOffset) *doorPosition;
    // ======================================
    // Горизонтальная дверь
    // Дверь занимает X
    // Толщина по Y
    // ======================================
    if(Doorangle == false){
        // ----------------------------------
        // Дверь прижата к правой стороне
        // клетки и уезжает влево
        // ----------------------------------
        float doorStart =mapX +(1.0f - doorWidth);
        float doorEnd =mapX + 1.0f;

        // ----------------------------------
        // Толщина
        // ----------------------------------
        float doorY1 =mapY +DoorOffset -DoorThickness / 2.0f;
        float doorY2 = mapY + DoorOffset + DoorThickness / 2.0f;
        // ==================================
        // ПЕРЕДНЯЯ / ЗАДНЯЯ ГРАНЬ
        // ==================================
        if(abs(rayDirY) > 0.0001f){
            float doorY;
            // Луч идёт вниз
            if(rayDirY > 0){
                doorY = doorY1;
            }
            // Луч идёт вверх
            else{
                doorY = doorY2;
            }
            doorDistance =(doorY - PlayerPosY) / rayDirY;
            if(doorDistance > 0.0f){
                float hitX = PlayerPosX + doorDistance * rayDirX;
                if(hitX >= doorStart && hitX <= doorEnd){
                    textureX = (hitX - doorStart -(1.0f-doorPosition)) / doorWidth*0.8f;
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 1;
                    return result;
                }
            }
        }
        // ==================================
        // ЛЕВАЯ БОКОВАЯ ГРАНЬ
        // ==================================
        if(abs(rayDirX) > 0.0001f){
            doorDistance =(doorStart - PlayerPosX) / rayDirX;
            if(doorDistance > 0.0f){
                float hitY =PlayerPosY +doorDistance * rayDirY;
                if(hitY >= doorY1 && hitY <= doorY2){
                    textureX =(hitY - doorY1) /DoorThickness +(width- width/4);
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 0;
                    return result;
                }
            }
            // ==================================
            // ПРАВАЯ БОКОВАЯ ГРАНЬ
            // ==================================
            doorDistance =(doorEnd - PlayerPosX) /rayDirX;
            if(doorDistance > 0.0f){
                float hitY =PlayerPosY +doorDistance * rayDirY;
                if(hitY >= doorY1 &&hitY <= doorY2){
                    textureX =(hitY - doorY1) /DoorThickness +(width- width/4);
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 0;
                    return result;
                }
            }
        }
    }
    // ======================================
    // ВЕРТИКАЛЬНАЯ ДВЕРЬ
    // Дверь занимает Y
    // Толщина по X
    // ======================================
    else{
        // ----------------------------------
        // Дверь прижата к нижней стороне
        // клетки и уезжает вверх
        // ----------------------------------
        float doorStart =mapY +(1.0f - doorWidth);
        float doorEnd =mapY + 1.0f;
        // ----------------------------------
        // Толщина двери
        // ----------------------------------
        float doorX1 =mapX +DoorOffset -DoorThickness / 2.0f;
        float doorX2 =mapX +DoorOffset +DoorThickness / 2.0f;
        // ==================================
        // ПЕРЕДНЯЯ / ЗАДНЯЯ ГРАНЬ
        // ==================================
        if(abs(rayDirX) > 0.0001f){
            float doorX;
            // Луч идёт вправо
            if(rayDirX > 0){
                doorX = doorX1;
            }
            // Луч идёт влево
            else{
                doorX = doorX2;
            }
            doorDistance = (doorX - PlayerPosX) / rayDirX;
            if(doorDistance > 0.0f){
                float hitY = PlayerPosY + doorDistance * rayDirY;
                if(hitY >= doorStart && hitY <= doorEnd){
                    textureX = (hitY - doorStart - (1.0f-doorPosition)) / doorWidth *0.8f;
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 0;
                    return result;
                }
            }
        }
        // ==================================
        // ВЕРХНЯЯ БОКОВАЯ ГРАНЬ
        // ==================================
        if(abs(rayDirY) > 0.0001f){
            doorDistance = (doorStart - PlayerPosY) / rayDirY;
            if(doorDistance > 0.0f){
                float hitX = PlayerPosX + doorDistance * rayDirX;
                if(hitX >= doorX1 && hitX <= doorX2){
                    textureX = (hitX - doorX1) / DoorThickness +(width- width/4);
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 1;
                    return result;
                }
            }
            // ==================================
            // НИЖНЯЯ БОКОВАЯ ГРАНЬ
            // ==================================
            doorDistance = (doorEnd - PlayerPosY) / rayDirY;
            if(doorDistance > 0.0f){
                float hitX = PlayerPosX + doorDistance * rayDirX;
                if(hitX >= doorX1 &&
                hitX <= doorX2){
                    textureX =(hitX - doorX1) / DoorThickness +(width- width/4);
                    RayCastReturn result;
                    result.dis = doorDistance;
                    result.textureX = textureX;
                    result.side = 1;
                    return result;
                }
            }
        }
    }
    // ======================================
    // Луч не попал в дверь
    // ======================================
    RayCastReturn result;
    result.dis = -1;
    result.textureX = 0;
    result.side = 0;
    return result;
}


void DoorUnit::JsonImport(int resourceID){
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
    if (Data.contains("type-rc") && Data["type-rc"] == 1){
        if (Data.contains("unit")){
            std::string STR = Data["unit"];
            Unit = STR.empty() ? L' ' : STR[0];
        }
        if (Data.contains("doorAngle")) Doorangle = Data["doorAngle"];
        if (Data.contains("width")) width = Data["width"];
        if (Data.contains("height")) height = Data["height"];
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