#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>
#include "../json.hpp"
#include <memory>
#include "System.h"
#include "graphics.h"
#include "Logics.h"
#include "UI.h"
using json = nlohmann::json;
struct KeyBinding{
    std::function<void()> function;
    bool hold;
};
class Engine{
    public:
        std::string name;
        RayCastReturn RayCast(float angle){
            return gr.RayCast(angle);
        }
        int& FOV = gr.FOV;
        float& cameraHeight = gr.cameraHeight;
        float& StartAngle = lo.StartAngle;
        float& PlayerPosX = lo.PlayerPosX;
        float& PlayerPosY = lo.PlayerPosY;
        Engine(std::string name, int w, int h, int FPS);
        void StartEngine();
        void MapImport(std::wstring Map,std::vector<MapUnit*> MapUnits, int x, int y);
        void MapImport(int MapID,std::vector<MapUnit*> MapUnits);
        void BindKey(int key,bool hold, std::function<void()> function);
        void BindFunction(std::function<void()> function, int type);
        std::vector<std::vector<std::unique_ptr<MapUnit>>>* Map = &lo.Map;
        std::vector<std::unique_ptr<EntityUnit>>* Entities = &lo.Entities;
        std::vector<std::unique_ptr<UIunit>>* Panels = &ui.Panels;
    private:
        systemE sys;
        graphics gr;
        Logics lo;
        UI ui;
        int FPS;
        int* SizeWindows = gr.SizeWindows;
        std::unordered_map<int,KeyBinding> KeyBindings;
        std::vector<std::function<void()>> MainLoopFunctions;
        std::vector<std::function<void()>> MainStartFunctions;
        std::chrono::milliseconds frameDuration;
};

