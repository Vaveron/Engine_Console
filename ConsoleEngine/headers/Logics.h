#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "../headers/mapunit.h"
#include "../headers/EntityUnit.h"
class Logics{
    friend class Engine;
    friend class graphics;
    friend class UI;
    public:
        /*Logics(){
            for(int i =0; i<3;i++){
                Entities.push_back(
                    std::make_unique<EntityUnit>()
                );

                EntityUnit* Test = Entities.back().get();

                // ==========================================
                // Положение
                // ==========================================
                Test->positionX = 2.0f;
                Test->positionY = 2.0f+i+1;

                // ==========================================
                // Размер текстуры
                // ==========================================
                Test->width = 8;
                Test->height = 8;

                // ==========================================
                // Выделяем текстуру
                // ==========================================
                Test->pixels.resize(
                    Test->height,
                    std::vector<wchar_t>(Test->width)
                );

                Test->colors.resize(
                    Test->height,
                    std::vector<WORD>(Test->width)
                );
                Test->StandartModeBehavoir = "Angry";
                Test->moveSpeed = 0.005f;
                // ==========================================
                // Заполняем Entity
                // ==========================================
                for(int y = 0; y < Test->height; y++){

                    for(int x = 0; x < Test->width; x+=2){
                        Test->pixels[y][x] = L' ';
                        Test->pixels[y][x+1] = L'#';
                        Test->colors[y][x] = 0;
                        Test->colors[y][x+1] =
                            FOREGROUND_RED |
                            FOREGROUND_INTENSITY;
                    }
                }
            }
            
        }*/
    private:
        void StartfuncEntity();
        void UpdateAImain();
        float StartAngle;
        float PlayerPosX;
        float PlayerPosY;
        std::vector<std::vector<std::unique_ptr<MapUnit>>> Map;
        std::vector<std::unique_ptr<EntityUnit>> Entities;

};