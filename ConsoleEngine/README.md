Артем пока для тебя пишу. 
Тема такая буду разбирать на примере кода: 
```
#include "Engine/headers/engine.h" <= основной API
#include <windows.h>
Engine engine = Engine("Game", 600, 175, 30); <= Создаем экземпляр движка с именем Game и размером окна 600 на 175 символов + 30 кадров в секунду (пока не сделал управление размером символа да и адаптивное не сделал и фпс адаптивный тоже)

bool E_pressed;
RayCastReturn Door; <= То что отдает любой rayCast можно посмотреть в RayCastReturn.h
DoorUnit* Door_; <= Наследник от MapUnit по сути стандартная дверь из вуфельштейн 3д

void Update(){
    bool ENow = GetAsyncKeyState('E') & 0x8000;
    if(ENow && !E_pressed){
        Door = engine.RayCast(engine.StartAngle);
        if(Door.unit != nullptr && Door.unit->typeUnit == "Door"){
            Door_ = dynamic_cast<DoorUnit*>(Door.unit);
            Door_->Door_is_opened = !Door_->Door_is_opened;
        }
    }
    E_pressed = ENow;
    if(Door_ && Door.unit != nullptr && Door.dis <= 3.0f){
        if(Door_->Door_is_opened){
            Door_->doorPosition -= Door_->DoorSpeed;

            if(Door_->doorPosition < 0.0f) Door_->doorPosition = 0.0f;
        }
        else{
            Door_->doorPosition += Door_->DoorSpeed;
            if(Door_->doorPosition > 1.0f) Door_->doorPosition = 1.0f;
        }
    }   
}
int main(int argc, char const *argv[])
{
    WallUnit Wall; Wall.JsonImport(1); <= Создаем экзампляры шаблонов клеток для карты (JsonImport работает с id из resources.rc)
    DoorUnit Door_; Door_.JsonImport(2);
    DoorUnit Door; Door.JsonImport(3);
    MapUnit None; None.JsonImport(4);
    engine.MapImport(5, {&Wall,&Door_, &Door, &None}); <= Есть две функции где принимает id ресурса и текст
    engine.PlayerPosX = 2.0f; engine.PlayerPosY = 2.0f;
    float moveSpeed = 0.1f;
    float rotSpeed = 0.05f;
    engine.BindKey('W', true, [&](){ <= это функция биндит на определенную кнопку функцию
        float newX = engine.PlayerPosX + cos(engine.StartAngle) * moveSpeed;
        float newY = engine.PlayerPosY + sin(engine.StartAngle) * moveSpeed;
        auto* unitX = (*engine.Map)[static_cast<int>(engine.PlayerPosY)][static_cast<int>(newX)].get();
        auto* unitY = (*engine.Map)[static_cast<int>(newY)][static_cast<int>(engine.PlayerPosX)].get();
        if((unitX->typeUnit != "Wall" && unitX->typeUnit != "Door") || (unitX->typeUnit == "Door" && dynamic_cast<DoorUnit*>(unitX)->Door_is_opened == true)) engine.PlayerPosX = newX;
        if((unitY->typeUnit != "Wall" && unitY->typeUnit != "Door") || (unitY->typeUnit == "Door" && dynamic_cast<DoorUnit*>(unitY)->Door_is_opened == true)) engine.PlayerPosY = newY;
    });
    engine.BindKey('S', true, [&](){
        engine.PlayerPosX -= cos(engine.StartAngle) * moveSpeed;
        engine.PlayerPosY -= sin(engine.StartAngle) * moveSpeed;
    });
    engine.BindKey('A', true, [&](){
        engine.StartAngle -= rotSpeed;
    });
    engine.BindKey('D', true, [&](){
        engine.StartAngle += rotSpeed;
    });
    engine.BindKey(VK_ESCAPE, false, [&](){
        exit(0);
    });
    engine.BindFunction(Update, 0); <= Функция биндит на события другие функции
    engine.StartEngine(); <= Старт игры
    return 0;
}
```