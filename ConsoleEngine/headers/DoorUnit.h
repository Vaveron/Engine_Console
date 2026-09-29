#pragma once
#include "../headers/mapunit.h"

class DoorUnit : public MapUnit{
    public:
        DoorUnit(){
            typeUnit = "Door";
        }
        bool Doorangle;
        float doorPosition = 1.0f;
        bool Door_is_opened = false;
        float DoorSpeed = 0.05f;
        RayCastReturn RayCastSelf(int mapX, int mapY, int side, float PlayerPosX, float PlayerPosY, int stepX, int stepY, float rayDirX, float rayDirY) override;
        void JsonImport(int resourceID) override;
        std::unique_ptr<MapUnit> Clone() const override{
            return std::make_unique<DoorUnit>(*this);
        }
};