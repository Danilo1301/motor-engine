#include "Mod.h"

#include "Vehicle.h"
#include "utils.h"

int m_frame = 0;
Vehicle* mainVehicle = nullptr;
unsigned int lastTime;

// ----------------------

Mod mod;
int Mod::updateDeltaTime = 0;
ISoundEngine* Mod::soundEngine = nullptr;

ISound* engineSound = nullptr;

// ----------------------

Mod::Mod() {
    Log::Init();
    Log::file << "Mod iniciado" << std::endl;

    lastTime = CTimer::m_snTimeInMilliseconds;

    Events::initGameEvent += OnInit;
}

void Mod::OnInit() {
    soundEngine = irrklang::createIrrKlangDevice();
    if (!soundEngine) {
        Log::file << "Erro ao criar o IrrKlang sound engine!" << std::endl;
        return;
    }

    // toca "engine.wav" em loop
    engineSound = soundEngine->play2D(GetPathFromPluginFolder("engine.wav").c_str(), true, false, true);
    if (!engineSound) {
        Log::file << "Erro ao carregar o engine.wav" << std::endl;
        return;
    }
    engineSound->setVolume(0.0f);

    Events::processScriptsEvent += OnProcessScripts;
}

void Mod::OnProcessScripts() {
    m_frame++;

    auto now = CTimer::m_snTimeInMilliseconds;
    updateDeltaTime = (now - lastTime);
    lastTime = now;

    Input::Update();

    //Log::file << "Frame atual: " << m_frame << std::endl;

    auto vehicle = FindPlayerVehicle(0, false);
    if (vehicle && mainVehicle == nullptr)
    {
        mainVehicle = new Vehicle(vehicle);
    }
    
    if (!vehicle)
    {
        mainVehicle = nullptr;
    }

    if (mainVehicle)
    {
        mainVehicle->Update();
    }
    else {
        static char msg[255];
        sprintf_s(msg, "No car found! Frame %d, dt %d", m_frame, updateDeltaTime);
        CMessages::AddMessageJumpQ(msg, 500, 0, false);
    }
}