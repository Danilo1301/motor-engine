#pragma once

#include <Windows.h>
#include <array>

class Input {
private:
    static std::array<bool, 256> previousState; // frame anterior
    static std::array<bool, 256> currentState;  // frame atual

public:
    // Atualiza todas as teclas → chamar **uma vez por frame**
    static void Update() {
        for (int key = 0; key < 256; key++) {
            previousState[key] = currentState[key];
            currentState[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
        }
    }

    // True apenas no frame que a tecla foi pressionada
    static bool GetKeyDownThisFrame(int key) {
        return currentState[key] && !previousState[key];
    }

    // True enquanto a tecla estiver sendo segurada
    static bool IsKeyDown(int key) {
        return currentState[key];
    }

    // True apenas no frame que a tecla foi solta
    static bool GetKeyUpThisFrame(int key) {
        return !currentState[key] && previousState[key];
    }
};