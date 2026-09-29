#pragma once

#include "../../olcPixelGameEngine.h"
#include "../../models.h"
#include "../../Helpers/Math/math.h"
#include <vector>
static bool ShowPlayerInfo = false;

void HandleControls(olc::PixelGameEngine& Engine, Player& PlayerInfo, float fElapsedTime, std::vector<mesh>& toDraw);