#include "Input.h"

// Inicialização dos arrays estáticos
std::array<bool, 256> Input::previousState = {};
std::array<bool, 256> Input::currentState = {};