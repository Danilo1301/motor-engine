#include "pch.h"

using namespace plugin;

class Mod {
public:
    static int updateDeltaTime;
    static ISoundEngine* soundEngine;

    Mod();

    static void OnInit();
    static void OnProcessScripts();
};