#pragma once

#include "../../models.h"


namespace CreateModel
{
    mesh CreateSphereMesh(int columns, int rows, float size, vec3d pos);
    mesh CreateCubeMesh(float size, vec3d pos);

}