#include "../headers/graphics.h"
#include <windows.h>
#include <cmath>
void graphics::ConsoleExport(){
    std::vector<CHAR_INFO> buffer(SizeWindows[0] * SizeWindows[1]);

    for(int y = 0; y < SizeWindows[1]; y++){

        for(int x = 0; x < SizeWindows[0]; x++){

            int index =
                y * SizeWindows[0] + x;

            buffer[index].Char.UnicodeChar =
                Frame[y][x];

            buffer[index].Attributes =
                ColorFrame[y][x];
        }
    }

    COORD bufferSize;
    bufferSize.X = SizeWindows[0];
    bufferSize.Y = SizeWindows[1];

    SMALL_RECT writeRegion;
    writeRegion.Left = 0;
    writeRegion.Top = 0;
    writeRegion.Right = SizeWindows[0] - 1;
    writeRegion.Bottom = SizeWindows[1] - 1;

    WriteConsoleOutputW(
        sys->hConsole,
        buffer.data(),
        bufferSize,
        sys->coord,
        &writeRegion
    );
}

RayCastReturn graphics::RayCast(float angle){
    float rayDirX = cos(angle);
    float rayDirY = sin(angle);
    // ==========================================
    // Текущая клетка карты
    // ==========================================
    int mapX = static_cast<int>(lo->PlayerPosX);
    int mapY = static_cast<int>(lo->PlayerPosY);
    // ==========================================
    // Расстояние между пересечениями луча
    // ==========================================
    float deltaDistX;
    float deltaDistY;
    if(rayDirX == 0.0f) deltaDistX = 1e30f;
    else deltaDistX = abs(1.0f / rayDirX);
    if(rayDirY == 0.0f) deltaDistY = 1e30f;
    else deltaDistY = abs(1.0f / rayDirY);
    // ==========================================
    // Направление движения луча
    // ==========================================
    int stepX;
    int stepY;
    // ==========================================
    // Расстояние до первой границы клетки
    // ==========================================
    float sideDistX;
    float sideDistY;
    if(rayDirX < 0){
        stepX = -1;
        sideDistX = (lo->PlayerPosX - mapX) * deltaDistX;
    }
    else{
        stepX = 1;
        sideDistX = (mapX + 1.0f - lo->PlayerPosX) * deltaDistX;
    }

    if(rayDirY < 0){
        stepY = -1;
        sideDistY = (lo->PlayerPosY - mapY) * deltaDistY;
    }
    else{
        stepY = 1;
        sideDistY = (mapY + 1.0f - lo->PlayerPosY) * deltaDistY;
    }

    int side = 0;

    // ==========================================
    // Стена
    // ==========================================
    RayCastReturn Wall;
    Wall.dis = 0;
    Wall.textureX = 0.0f;
    Wall.side = 0;
    Wall.entity = nullptr;
    Wall.unit = nullptr;

    // ==========================================
    // RayCast карты
    // ==========================================
    while(true){
        // ==========================================
        // Следующая клетка
        // ==========================================
        if(sideDistX < sideDistY){
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        }
        else{
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        // ==========================================
        // Проверка границ карты
        // ==========================================
        if(mapX < 0 || mapX >= static_cast<int>(lo->Map[0].size()) || mapY < 0 || mapY >= static_cast<int>(lo->Map.size())) break;
        // ==========================================
        // Пустая клетка
        // ==========================================
        if(lo->Map[mapY][mapX]->Unit == L' ')continue;
        // ==========================================
        // RayCast стены
        // ==========================================
        Wall = lo->Map[mapY][mapX]->RayCastSelf( mapX,mapY,side,lo->PlayerPosX,lo->PlayerPosY,stepX,stepY,rayDirX,rayDirY);
        if(Wall.dis != -1){
            Wall.unit = lo->Map[mapY][mapX].get();
            break;
        }
    }
    // ==========================================
    // Проверяем Entity
    // ==========================================
    for(auto& entity : lo->Entities){
        // ==========================================
        // Проверка указателя
        // ==========================================
        if(!entity) continue;
        // ==========================================
        // RayCast Entity
        // ==========================================
        RayCastReturn Entity = entity->RayCastSelf( lo->PlayerPosX, lo->PlayerPosY, rayDirX, rayDirY);
        // ==========================================
        // Луч ничего не пересёк
        // ==========================================
        if(Entity.dis < 0.0f) continue;
        // ==========================================
        // Entity ближе стены
        // ==========================================
        if(Entity.dis < Wall.dis){
            Entity.entity = entity.get();
            Wall.dis_s = Wall.dis;
            Wall.textureX_s = Wall.textureX;
            Wall.side_s = Wall.side;
            Wall.entity = Entity.entity;
            Wall.dis = Entity.dis;
            Wall.side = Entity.side;
            Wall.textureX = Entity.textureX;
        }
    }

    // ==========================================
    // Результат
    // ==========================================
    return Wall;
}

void graphics::ChangeFrame(){
    // ==========================================
    // Очистка кадра
    // ==========================================
    for(int y = 0; y < SizeWindows[1]; y++){
        for(int x = 0; x < SizeWindows[0]; x++){
            Frame[y][x] = L' ';
            ColorFrame[y][x] = 0;
        }
    }
    // ==========================================
    // FOV
    // ==========================================
    const float PI = 3.14159265359f;
    float fovRadians = FOV * PI / 180.0f;
    float startAngle = lo->StartAngle - fovRadians / 2.0f;
    // ==========================================
    // Горизонт
    // ==========================================
    int horizon = SizeWindows[1] / 2;
    // ==========================================
    // ==========================================
    // 1. РИСУЕМ ПОЛ
    // ==========================================
    // ==========================================
    //
    // Пол НЕ зависит от стены.
    //
    // Он начинается от горизонта и идёт
    // до нижней границы экрана.
    //
    // Поэтому никакого "скачка" пола
    // между разными стенами не будет.
    // ==========================================
    for(int ScreenX = 0; ScreenX < SizeWindows[0]; ScreenX++){
        for(int screenY = horizon; screenY < SizeWindows[1]; screenY++){
            Frame[screenY][ScreenX] = L'#';
            ColorFrame[screenY][ScreenX] = FOREGROUND_INTENSITY;
        }
    }
    // ==========================================
    // ==========================================
    // 2. RAYCAST СТЕН + ENTITY
    // ==========================================
    // ==========================================
    for(int ScreenX = 0; ScreenX < SizeWindows[0];ScreenX++){
        // ======================================
        // Угол луча
        // ======================================
        float rayPercent = static_cast<float>(ScreenX) / static_cast<float>(SizeWindows[0]);
        float angle = startAngle + rayPercent * fovRadians;
        // ======================================
        // Направление луча
        // ======================================
        float rayDirX =cos(angle);
        float rayDirY =sin(angle);
        // ======================================
        // RayCast
        // ======================================
        RayCastReturn wall = RayCast(angle);
        // ======================================
        // Расстояние
        // ======================================
        float distance = wall.dis;
        float distanceBack = wall.dis_s;
        // ======================================
        // Fish-eye correction
        // ======================================
        float angleDifference = angle - lo->StartAngle;
        float correctedDistance = distance *cos(angleDifference);
        float correctedDistanceBack = distanceBack *cos(angleDifference);
        if(correctedDistance < 0.1f)correctedDistance = 0.1f;
        // ======================================
        // ======================================
        // ВЫСОТА ОБЪЕКТА
        // ======================================
        // ======================================
        float objectHeight;
        float objectbackHeight;
        // ======================================
        // ENTITY
        // ======================================
        if(wall.entity != nullptr){
            EntityUnit* entity = wall.entity;
            objectHeight = (static_cast<float>(SizeWindows[1] ) / correctedDistance) * entity->sizeY;
            objectbackHeight = (static_cast<float>(SizeWindows[1]) / correctedDistanceBack) * 1.25f;
        }
        // ======================================
        // WALL
        // ======================================
        else{
            objectHeight = (static_cast<float>( SizeWindows[1]) / correctedDistance) *1.25f;
        }
        // ======================================
        // Ограничение
        // ======================================
        if(objectHeight > static_cast<float>(SizeWindows[1]) * 3.0f) objectHeight = static_cast<float>(SizeWindows[1]) * 3.0f;
        // ==========================================
        // ==========================================
        // WALL
        // ==========================================
        // ==========================================
        if(wall.entity == nullptr && wall.unit != nullptr){
            // ======================================
            // Стена физически стоит на полу.
            //
            // Камера находится на высоте
            // cameraHeight.
            //
            // Поэтому нижняя точка стены
            // должна соответствовать полу.
            // ======================================
            float projectedCameraHeight = (static_cast<float>(SizeWindows[1]) / correctedDistance) * cameraHeight;
            int wallBottom = horizon + static_cast<int>(projectedCameraHeight);
            int wallTop = wallBottom - static_cast<int>(objectHeight);
            // ======================================
            // Ограничения
            // ======================================
            if(wallTop < 0) wallTop = 0;
            if(wallBottom >= SizeWindows[1]) wallBottom = SizeWindows[1] - 1;
            // ======================================
            // Рисуем стену
            // ======================================
            for(int screenY = wallTop; screenY <= wallBottom; screenY++){
                if(wall.unit->width > 0 && wall.unit->height > 0 && !wall.unit->pixels.empty() && !wall.unit->colors.empty()){
                    // ==============================
                    // X текстуры
                    // ==============================
                    int texX =static_cast<int>(wall.textureX *wall.unit->width);
                    if(texX < 0)texX = 0;
                    if(texX >= wall.unit->width)texX =wall.unit->width - 1;
                    // ==============================
                    // Y текстуры
                    // ==============================
                    float texturePercent = static_cast<float>(screenY - wallTop) /static_cast<float>(wallBottom - wallTop + 1);
                    int texY = static_cast<int>( texturePercent * wall.unit->height);
                    if(texY < 0) texY = 0;
                    if(texY >= wall.unit->height) texY = wall.unit->height - 1;
                    // ==============================
                    // Символ
                    // ==============================
                    Frame[screenY][ScreenX] = wall.unit->pixels[texY][texX];
                    // ==============================
                    // Цвет
                    // ==============================
                    ColorFrame[screenY][ScreenX] = wall.unit->colors[texY][texX];
                }
                // ==================================
                // Нет текстуры
                // ==================================
                else{
                    Frame[screenY][ScreenX] =L'\u2588';
                    ColorFrame[screenY][ScreenX] = FOREGROUND_RED;
                }
            }
        }
        // ==========================================
        // ==========================================
        // ENTITY
        // ==========================================
        // ==========================================
        else if(wall.entity != nullptr){
            EntityUnit* entity = wall.entity;
            // ======================================
            // Стена физически стоит на полу.
            //
            // Камера находится на высоте
            // cameraHeight.
            //
            // Поэтому нижняя точка стены
            // должна соответствовать полу.
            // ======================================
            float projectedCameraHeightBack =(static_cast<float>(SizeWindows[1]) /correctedDistanceBack) *cameraHeight;
            int wallBottom =horizon +static_cast<int>(projectedCameraHeightBack);
            int wallTop = wallBottom - static_cast<int>(objectbackHeight);
            // ======================================
            // Ограничения
            // ======================================
            if(wallTop < 0) wallTop = 0;
            if(wallBottom >= SizeWindows[1]) wallBottom = SizeWindows[1] - 1;
            // ======================================
            // Рисуем стену
            // ======================================
            for(int screenY = wallTop; screenY <= wallBottom; screenY++){
                if( wall.unit->width > 0 && wall.unit->height > 0 && !wall.unit->pixels.empty() && !wall.unit->colors.empty()){
                    // ==============================
                    // X текстуры
                    // ==============================
                    int texX = static_cast<int>(wall.textureX_s * wall.unit->width);
                    if(texX < 0) texX = 0;
                    if(texX >= wall.unit->width) texX = wall.unit->width - 1;
                    // ==============================
                    // Y текстуры
                    // ==============================
                    float texturePercent = static_cast<float>( screenY - wallTop ) / static_cast<float>(wallBottom - wallTop + 1);
                    int texY = static_cast<int>( texturePercent * wall.unit->height );
                    if(texY < 0) texY = 0;
                    if(texY >= wall.unit->height) texY = wall.unit->height - 1;
                    // ==============================
                    // Символ
                    // ==============================
                    Frame[screenY][ScreenX] = wall.unit->pixels[texY][texX];
                    // ==============================
                    // Цвет
                    // ==============================
                    ColorFrame[screenY][ScreenX] = wall.unit->colors[texY][texX];
                }
                // ==================================
                // Нет текстуры
                // ==================================
                else{
                    Frame[screenY][ScreenX] = L'\u2588';
                    ColorFrame[screenY][ScreenX] = FOREGROUND_RED;
                }
            }
            // ======================================
            // Нижняя точка ENTITY
            //
            // ВАЖНО:
            // она находится НА ПОЛУ,
            // а не на горизонте.
            // ======================================
            float projectedCameraHeight = (static_cast<float>(SizeWindows[1]) /correctedDistance) * cameraHeight;
            int entityBottom = horizon + static_cast<int>( projectedCameraHeight);
            int entityTop = entityBottom - static_cast<int>( objectHeight );
            // ======================================
            // Ограничения
            // ======================================
            int drawTop = entityTop; 
            int drawBottom = entityBottom;
            if(drawTop < 0) drawTop = 0;
            if(drawBottom >= SizeWindows[1]) drawBottom = SizeWindows[1] - 1;
            // ======================================
            // Проверяем текстуру
            // ======================================
            if(!entity->pixels.empty() && !entity->colors.empty() && entity->width > 0 && entity->height > 0){
                int texHeight = static_cast<int>(entity->pixels.size());
                int texWidth = static_cast<int>( entity->pixels[0].size() );
                if(texWidth > 0 && texHeight > 0){
                    // ==================================
                    // X текстуры
                    // ==================================
                    int texX =
                        static_cast<int>( wall.textureX * texWidth);
                    if(texX < 0) texX = 0;
                    if(texX >= texWidth) texX = texWidth - 1;

                    // ==================================
                    // Y
                    // ==================================
                    for(int screenY = drawTop; screenY <= drawBottom; screenY++){

                        float texturePercent = static_cast<float>(screenY - entityTop) / static_cast<float>(objectHeight);
                        int texY = static_cast<int>(texturePercent * texHeight );
                        if(texY < 0) texY = 0;
                        if(texY >= texHeight) texY = texHeight - 1;
                        // ==============================
                        // Символ
                        // ==============================
                        if(entity->pixels[texY][texX] != L' ') Frame[screenY][ScreenX] = entity->pixels[texY][texX];
                        // ==============================
                        // Цвет
                        // ==============================
                        if(entity->pixels[texY][texX] != L' ') ColorFrame[screenY][ScreenX] = entity->colors[texY][texX];
                    }
                }
            }
            // ======================================
            // Нет текстуры
            // ======================================
            else{
                for(int screenY = drawTop; screenY <= drawBottom; screenY++){
                    Frame[screenY][ScreenX] = L'\u2588';
                    ColorFrame[screenY][ScreenX] = FOREGROUND_RED;
                }
            }
        }
    }
}