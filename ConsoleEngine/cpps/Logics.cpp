#include "../headers/Logics.h"
#include "../headers/DoorUnit.h"
#include <random>

void Logics::UpdateAImain(){
    for(auto& Entity : Entities){
        Entity->Update();
        if (Entity->StandartModeBehavoir == "Angry"){
            float disX = PlayerPosX - Entity->positionX;
            float disY = PlayerPosY - Entity->positionY;
            float Gip = sqrt((disX*disX)+(disY*disY));
            if (Gip <= 10.0f){
                float newX = Entity->positionX + (disX/Gip) * Entity->moveSpeed;
                float newY = Entity->positionY + (disY/Gip) * Entity->moveSpeed;
                auto* unitX = Map[static_cast<int>(Entity->positionY)][static_cast<int>(newX)].get();
                auto* unitY = Map[static_cast<int>(newY)][static_cast<int>(Entity->positionX)].get();
                if((unitX->typeUnit != "Wall" && unitX->typeUnit != "Door") || (unitX->typeUnit == "Door" && dynamic_cast<DoorUnit*>(unitX)->Door_is_opened == true)) Entity->positionX = newX;
                if((unitY->typeUnit != "Wall" && unitY->typeUnit != "Door") || (unitY->typeUnit == "Door" && dynamic_cast<DoorUnit*>(unitY)->Door_is_opened == true)) Entity->positionY = newY;
            }
            
        }
    }
}

void Logics::StartfuncEntity(){
    for(auto& Entity : Entities){
        Entity->Start();
    }
}