#include "../headers/engine.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include <map>

Engine::Engine(std::string name, int w, int h, int FPS){
    gr.lo = &lo;
    gr.sys = &sys;
    ui.gr = &gr;
    ui.lo = &lo;
    ui.sys = &sys;
    this->name = name;
    this->FPS = FPS;
    this->SizeWindows[0] = w;  this->SizeWindows[1] = h;
    sys.SetConsoleSize(w, h, 5, name);
}
void Engine::StartEngine(){
    for(auto& function : MainStartFunctions) function();
    int msPerFrame = 1000 / this->FPS;
    this->frameDuration = std::chrono::milliseconds(msPerFrame);
    gr.Frame.resize(SizeWindows[1], std::vector<wchar_t>(SizeWindows[0]));
    gr.ColorFrame.resize(SizeWindows[1], std::vector<WORD>(SizeWindows[0])); 
    RayCastReturn wallSeelast{};
    lo.StartfuncEntity();
    while(true){
        //sys.Cursor();
        auto frameStart = std::chrono::steady_clock::now();
        for(auto& key : KeyBindings){
            if(key.second.hold){
                if(GetAsyncKeyState(key.first) & 0x8000) key.second.function();
            }
            else{
                if(GetAsyncKeyState(key.first) & 1) key.second.function();
            }
        }
        for(auto& function : MainLoopFunctions) function();
        RayCastReturn wallSee = RayCast(StartAngle);
        if(wallSee.unit != nullptr){

            if(wallSeelast.unit != nullptr && wallSeelast.unit == wallSee.unit){
                wallSee.unit->watching(&wallSee);
            }
            else{
                wallSee.unit->Startwatching(&wallSee);
            }

            wallSeelast = wallSee;
        }
        lo.UpdateAImain();
        gr.ChangeFrame();
        ui.AddUItoFrame();
        gr.ConsoleExport();
        
        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (elapsedTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsedTime);
        }
    }
}
void Engine::BindKey(int key,bool hold,std::function<void()> function){
    KeyBindings[key] = {function, hold};
}
void Engine::BindFunction(std::function<void()> function, int type){
    switch (type){
    case 0:
        MainLoopFunctions.push_back(function);
    case 1:
        MainStartFunctions.push_back(function);
    }
}

void Engine::MapImport(std::wstring Map,std::vector<MapUnit*> MapUnits, int x, int y){
    this->Map->resize(y);
    for(int y_ = 0; y_<y; y_++){
        this->Map[y_].resize(x);
        for(int x_ = 0; x_<x; x_++){
            for(MapUnit* unit : MapUnits){
                if(unit->Unit == Map[x_+(y_*x)]){
                    (*this->Map)[y_][x_] = unit->Clone();
                }
            }
            //this->Map[y_][x_] = Map[x_+(y_*x)];
        }
    }
}
void Engine::MapImport(int MapID, std::vector<MapUnit*> MapUnits){
    HRSRC resource =FindResourceW(nullptr,MAKEINTRESOURCEW(MapID),MAKEINTRESOURCEW(RT_RCDATA));
    if(!resource){
        std::cout<< MapID;
        std::system("pause");
        return;
    }
    HGLOBAL data =LoadResource(nullptr,resource);
    DWORD size =SizeofResource(nullptr,resource);
    const char* buffer = static_cast<const char*>(LockResource(data));
    json Data = json::parse(std::string(buffer, size));
    if(Data["type-rc"] == 0){
        this->Map->resize(Data["height"]);
        for(int y_ = 0; y_<Data["height"]; y_++){
            (*this->Map)[y_].resize(Data["width"]);
            std::string Map_X = Data["map"][y_];
            int size_Map = MultiByteToWideChar(CP_UTF8, 0,Map_X.c_str(),-1, nullptr, 0);
            std::wstring Map_X_W(size_Map,L'\0');
            MultiByteToWideChar( CP_UTF8, 0, Map_X.c_str(), -1, &Map_X_W[0], size_Map);
            Map_X_W.resize(size_Map - 1);
            for(int x_ = 0; x_<Data["width"]; x_++){
                for(MapUnit* unit : MapUnits){
                    if(unit->Unit == Map_X_W[x_]){
                        (*this->Map)[y_][x_] = unit->Clone();
                    }
                }
                //this->Map[y_][x_] = Map[x_+(y_*x)];
            }
        }
    }
}