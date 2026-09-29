#pragma once

#include <vector>
#include "../../../SpaceSim/olcPixelGameEngine.h"
#include "../../../SpaceSim/models.h"

std::list<triangle>  CreateFrame(std::vector<mesh>& ToDraw, float ScreenHeight, float ScreenWidth, float p_fov, Player& PlayerInfo, mat4x4& matProj, float& pDepthBuffer);
void RenderFrame(olc::PixelGameEngine& engine, std::list<triangle> CreatedFrame, Player& PlayerInfo);

mat4x4 CreateMatWorld(float fTheta = 0.0f, float fYaw = 0.0f);
mat4x4 CreateMatView(Player& PlayerInfo);

void TransformTriangle(triangle& tri, mat4x4& matWorld);
vec3d GetNormal(triangle& tri, vec3d& p_Camera);
void ConvertToViewSpace(triangle& tri, mat4x4& matView);
void ProjectTo3D(triangle& tri, mat4x4& matProj, triangle& clippedN);
void ScaleIntoView(triangle& tri);
void invertTriXY(triangle& tri);
void offsetToNormalisedSpace(triangle& tri, float ScreenHeight, float ScreenWidth);
void ClipTriangles(std::list<triangle>& Result, std::vector<triangle> vecTrianglesToRaster, float ScreenHeight, float ScreenWidth);
