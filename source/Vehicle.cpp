#include "Vehicle.h"

#include "utils.h"
#include "Mod.h"
#include "Input.h"

static char buffer[512];

extern ISound* engineSound;

Vehicle::Vehicle(CVehicle* vehicle) {
	this->pVehicle = vehicle;
    this->engine = new Engine(); // inicializa o engine
}

void Vehicle::Update() {
    CVector moveSpeedGlobal = pVehicle->m_vecMoveSpeed;
    CVector moveSpeedLocal = GlobalToLocal(moveSpeedGlobal, pVehicle->m_matrix);

    float currentSpeedUnits = std::abs(moveSpeedLocal.z);
    float currentSpeedKmH = UnitsToKmH(currentSpeedUnits);

    float maxThrottle = currentSpeedKmH < 10 ? 1.0f : 1.0f;
    
    engine->throttle = Input::IsKeyDown(87) ? maxThrottle : 0.0f; // W
    

    if (Input::GetKeyDownThisFrame(VK_LSHIFT))
    {
        engine->ShiftUp();
    }
    if (Input::GetKeyDownThisFrame(VK_LCONTROL))
    {
        engine->ShiftDown();
    }

    engine->UpdateAndGetMaxSpeed(currentSpeedKmH);

    // max speed
    float maxSpeedKmH = engine->maxSpeedKmH;
    float maxSpeed = KmHToUnitsPerFrame(maxSpeedKmH);

    moveSpeedLocal.z = std::clamp(moveSpeedLocal.z, -maxSpeed, maxSpeed);

    //convert back
    moveSpeedGlobal = LocalToGlobal(moveSpeedLocal, pVehicle->m_matrix);

    // aplica
    pVehicle->m_vecMoveSpeed = moveSpeedGlobal;

    float maxRpm = 8000;
    float rpm = engine->outRpmForSound;

    // debug
    sprintf_s(buffer, "%.1f / %.1f km/h, %d, %.0f %.2f", currentSpeedKmH, maxSpeedKmH, engine->GetCurrentGear(), rpm, engine->lastAcceleration);
    CMessages::AddMessageJumpQ(buffer, 500, 0, false);

    //sprintf_s(buffer, "expected: %.1f, max: %.1f", engine->expectedSpeedKmH, engine->maxSpeedKmH);
    //CMessages::AddMessageJumpQ(buffer, 500, 0, false);

    // ajustar pitch do motor
    if (engineSound) {
        engineSound->setVolume(0.5f); // volume constante

        // mapeia velocidade 0 → 0.8, max → 1.8
        float minPitch = 0.6f;
        float maxPitch = 1.9f;

   
        float pitch = minPitch + (rpm / maxRpm) * (maxPitch - minPitch);
        engineSound->setPlaybackSpeed(pitch);
    }
}