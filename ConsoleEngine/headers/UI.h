#include "Logics.h"
#include "graphics.h"
#include "System.h"
#include <vector>
#include <map>
#include <memory>
#include "UIunit.h"

class UI{
    friend class Engine;
    friend class graphics;
    private:
        std::vector<std::unique_ptr<UIunit>> Panels;
        void AddUItoFrame();
        std::vector<std::vector<wchar_t>> RenderChar(wchar_t character, int width, int height);
        Logics* lo;
        graphics* gr;
        systemE* sys;
}; 