#pragma once

#include "pch.h"

class Engine {
private:
    int currentGear = 0;
    float gearMaxSpeeds[5] = { 20, 40, 60, 90, 120 };
    float minRpm = 800.f;
    float maxRpm = 8000.f;

    float rpmBoost = 0.0f;
    float rpmBoostTimer = 0.0f;
    const float rpmBoostDuration = 0.5f;
    const float rpmBoostStrength = 4000.f;

    float lastThrottle = 0.0f;

    // 👇 Novo: tempo desde última troca
    float timeSinceLastShift = 9999.f;
    const float shiftBlockDuration = 0.5f; // bloqueia boost por 0.5s

public:
    float expectedSpeedKmH = 0;
    float maxSpeedKmH = 0;

    float throttle = 0;
    float outRpm = 0;
    float outRpmForSound = 0;
    float lastAcceleration = 0.0f;

    Engine() {}

    void UpdateAndGetMaxSpeed(float currentSpeedKmH, float deltaTime = 0.016f) {
        // atualiza contador desde a última troca
        timeSinceLastShift += deltaTime;

        float maxSpeedThisGear = gearMaxSpeeds[currentGear];

        float baseAcceleration = 0.35f;
        float gearAccelFactor = 1.0f - currentGear * 0.10f;

        float speedRatio = expectedSpeedKmH / maxSpeedThisGear;
        float speedFactor = 1.0f - speedRatio;

        float minSpeedForGear = (currentGear == 0) ? 0.0f : gearMaxSpeeds[currentGear - 1] * 0.6f;
        float maxSpeedForGear = maxSpeedThisGear * 1.2f;

        float penalty = 1.0f;
        if (currentSpeedKmH < minSpeedForGear) {
            float diff = minSpeedForGear - currentSpeedKmH;
            penalty = 1.0f / (1.0f + diff * 0.3f);
        }
        else if (currentSpeedKmH > maxSpeedForGear) {
            float diff = currentSpeedKmH - maxSpeedForGear;
            penalty = 1.0f / (1.0f + diff * 0.2f);
        }

        lastAcceleration = 0.0f;
        if (throttle > 0) {
            lastAcceleration = throttle * baseAcceleration * gearAccelFactor * speedFactor * penalty;
            expectedSpeedKmH += lastAcceleration;

            // Blip só quando throttle sai de 0 E não trocou marcha recentemente
            if (lastThrottle <= 0.0f && throttle > 0.0f && timeSinceLastShift > shiftBlockDuration) {
                float gearFactor = std::lerp(1.0f, 0.2f, currentGear / 4.0f);
                rpmBoost = rpmBoostStrength * gearFactor;
                rpmBoostTimer = rpmBoostDuration;
            }
        }

        // Decaimento do boost
        if (rpmBoostTimer > 0.0f) {
            rpmBoostTimer -= deltaTime;
            if (rpmBoostTimer <= 0.0f) {
                rpmBoost = 0.0f;
            }
        }

        if (currentSpeedKmH < maxSpeedKmH) {
            expectedSpeedKmH = currentSpeedKmH;
            maxSpeedKmH = currentSpeedKmH;
        }

        expectedSpeedKmH = std::clamp(expectedSpeedKmH, 0.0f, maxSpeedThisGear);
        maxSpeedKmH = expectedSpeedKmH;

        speedRatio = std::clamp(currentSpeedKmH / maxSpeedThisGear, 0.f, 1.f);
        outRpm = minRpm + speedRatio * (maxRpm - minRpm);

        outRpm += rpmBoost * (rpmBoostTimer / rpmBoostDuration);

        float lerpFactor = 0.1f;
        outRpmForSound += (outRpm - outRpmForSound) * lerpFactor;

        lastThrottle = throttle;
    }

    // Troca manual de marcha (sobe ou desce)
    void ShiftUp() {
        if (currentGear < 4) {
            currentGear++;
            timeSinceLastShift = 0.0f; // reseta timer
        }
    }

    void ShiftDown() {
        if (currentGear > 0) {
            currentGear--;
            timeSinceLastShift = 0.0f; // reseta timer
        }
    }

    int GetCurrentGear() const {
        return currentGear + 1;
    }

    void SetGearMaxSpeed(int gearIndex, float maxSpeedKmH) {
        if (gearIndex >= 0 && gearIndex < 5) {
            gearMaxSpeeds[gearIndex] = maxSpeedKmH;
        }
    }

    float GetMaxSpeedThisGear() const {
        return gearMaxSpeeds[currentGear];
    }
};