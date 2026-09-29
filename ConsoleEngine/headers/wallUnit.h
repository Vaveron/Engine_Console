#pragma once
#include "../headers/mapunit.h"
class WallUnit : public MapUnit{
    public:
        WallUnit(){
            typeUnit = "Wall";
        }
        std::unique_ptr<MapUnit> Clone() const override{
            return std::make_unique<WallUnit>(*this);
        }
};