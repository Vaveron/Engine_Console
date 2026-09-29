#pragma once
class MapUnit;
class EntityUnit;
struct RayCastReturn{
    float dis;
    float textureX;
    int side;
    float dis_s;
    float textureX_s;
    int side_s;
    MapUnit* unit;
    EntityUnit* entity = nullptr;
};