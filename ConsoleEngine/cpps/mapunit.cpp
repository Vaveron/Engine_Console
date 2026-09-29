#include "../headers/mapunit.h"

int MapUnit::CharToColor(wchar_t c){ 
    if(c >= L'0' && c <= L'9') return c - L'0'; 
    if(c >= L'A' && c <= L'F') return c - L'A' + 10; 
    if(c >= L'a' && c <= L'f') return c - L'a' + 10; return 0; 
}

void MapUnit::TextureImport(std::wstring Texture_str,std::wstring Texture_color_str,int x,int y){
    pixels.resize(y,std::vector<wchar_t>(x));
    colors.resize(y,std::vector<WORD>(x));
    width = x;
    height = y;
    for(int y_ = 0;y_ < y;y_++){
        for(int x_ = 0;x_ < x;x_++){
            int index = x_ + (y_ * x);
            pixels[y_][x_] = Texture_str[index];
            int colorNumber = CharToColor(Texture_color_str[index]);
            colors[y_][x_] = Foreground[colorNumber];
        }
    }
}
void MapUnit::JsonImport(int resourceID){
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
        //if (Data.contains("doorAngle")) Doorangle = Data["doorAngle"];
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
RayCastReturn MapUnit::RayCastSelf(int mapX, int mapY, int side, float PlayerPosX, float PlayerPosY, int stepX, int stepY, float rayDirX, float rayDirY){
    float distance;
    // ======================================
    // Перпендикулярное расстояние
    // ======================================
    if(side == 0){
        distance =(mapX - PlayerPosX +(1 - stepX) / 2.0f) /rayDirX;
    }
    else{
        distance = (mapY - PlayerPosY + (1 - stepY) / 2.0f) / rayDirY;
    }
    // ======================================
    // Координата попадания по текстуре
    // ======================================
    float wallX;
    if(side == 0){
        wallX = PlayerPosY + distance * rayDirY;
    }
    else{
        wallX = PlayerPosX + distance * rayDirX;
    }
    // Только дробная часть
    wallX -= floor(wallX);
    // Зеркалирование текстуры
    if(side == 0 && rayDirX > 0) wallX = 1.0f - wallX;
    if(side == 1 && rayDirY < 0) wallX = 1.0f - wallX;
    RayCastReturn result;
    result.dis = distance;
    result.textureX = wallX;
    result.side = side;
    return result;
}