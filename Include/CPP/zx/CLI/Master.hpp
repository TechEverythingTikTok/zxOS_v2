// Master.hpp
// Written by TechEverything

/*
Header for Master.cpp
*/

#pragma once

#include "Integers.hpp"

#include "zx/Drivers/Keyboard.hpp"

namespace CLI {
    void Initialize();
    void HandleInput(Keyboard::Event event);
}