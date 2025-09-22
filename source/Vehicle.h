#pragma once

#include "pch.h"

#include "Engine.h"

class Vehicle {
public:
	CVehicle* pVehicle;
	Engine* engine;

	Vehicle(CVehicle* vehicle);

	void Update();
};