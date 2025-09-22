#pragma once

#include "pch.h"

// Função para limitar a magnitude de um CVector
inline CVector LimitCVector(const CVector& vec, float maxLength) {
    float len = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    if (len > maxLength && len > 0.0f) {
        float scale = maxLength / len;
        return CVector{ vec.x * scale, vec.y * scale, vec.z * scale };
    }

    return vec; // já dentro do limite
}

// --- Função para limitar a velocidade forward ---
inline CVector LimitForwardSpeed(const CVector& moveSpeed, const CVector& forward, float maxForward) {
    // projeta moveSpeed no vetor forward
    float forwardVel = moveSpeed.x * forward.x + moveSpeed.y * forward.y + moveSpeed.z * forward.z;

    // limita a magnitude da velocidade forward
    forwardVel = std::clamp(forwardVel, -maxForward, maxForward);

    // reconstrói o vetor de velocidade apenas na direção forward
    return CVector{ forward.x * forwardVel, forward.y * forwardVel, forward.z * forwardVel };
}

inline std::string CVectorToString(const CVector& vec, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision);
    ss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return ss.str();
}

// Converte velocidade global em local (X=lateral, Y=forward, Z=altura)
inline CVector GlobalToLocal(const CVector& globalSpeed, const CMatrix* matrix) {
    CVector localSpeed;

    // eixo X local (lados)
    localSpeed.x = globalSpeed.x * matrix->right.x +
        globalSpeed.y * matrix->right.y +
        globalSpeed.z * matrix->right.z;

    // eixo Y local (frente/tras)
    localSpeed.y = globalSpeed.x * matrix->at.x +
        globalSpeed.y * matrix->at.y +
        globalSpeed.z * matrix->at.z;

    // eixo Z local (altura)
    localSpeed.z = globalSpeed.x * matrix->up.x +
        globalSpeed.y * matrix->up.y +
        globalSpeed.z * matrix->up.z;

    return localSpeed;
}

// Converte velocidade local (X=lados, Y=forward/back, Z=altura) para global
inline CVector LocalToGlobal(const CVector& localSpeed, const CMatrix* matrix) {
    CVector globalSpeed;

    // soma das componentes multiplicadas pelos vetores da matriz
    globalSpeed.x = localSpeed.x * matrix->right.x +
        localSpeed.y * matrix->at.x +
        localSpeed.z * matrix->up.x;

    globalSpeed.y = localSpeed.x * matrix->right.y +
        localSpeed.y * matrix->at.y +
        localSpeed.z * matrix->up.y;

    globalSpeed.z = localSpeed.x * matrix->right.z +
        localSpeed.y * matrix->at.z +
        localSpeed.z * matrix->up.z;

    return globalSpeed;
}

// escala perceptiva do GTA: 1 unit ≈ 100 km/h
static constexpr float unitsToKMScale = 100.0f;

inline float KmHToUnitsPerFrame(float kmh) {
    return kmh / unitsToKMScale;
}

inline float UnitsToKmH(float unitsPerFrame) {
    return unitsPerFrame * unitsToKMScale;
}

inline std::string GetPathFromPluginFolder(const std::string& relativePath) {
    // PLUGIN_PATH é a macro do seu mod que retorna a pasta do plugin
    std::string fullPath = std::string(PLUGIN_PATH("")) + relativePath;

    // opcional: converter barras para Windows-style
    for (auto& c : fullPath) {
        if (c == '/') c = '\\';
    }

    return fullPath;
}