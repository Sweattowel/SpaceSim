#pragma once

#include <string>
#include <vector>
#include "olcPixelGameEngine.h"

struct mat4x4
{
    float m[4][4] = { 0 };
};

struct vec2d
{
    float u = 0.0f;
    float v = 0.0f;
    float w = 1.0f;
};
struct vec3d 
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};

struct triangle
{
    vec3d p[3];
    vec2d t[3];
    wchar_t sym;
    olc::Pixel col = olc::WHITE;
};

struct mesh 
{
    vec3d Position = { 0.0f,0.0f,0.0f,0.0f };
    float Yaw = 0.0f;
    float Theta = 0.0f;
    float d_Yaw = 1.0f;
    float d_Theta = 0.0f;
    std::vector<triangle> triangles;
};

struct Planet
{
    float p_x = 0.0f;
    float p_y = 0.0f;
    float p_z = 0.0f;
    
    float p_vx = 0.0f;
    float p_vy = 0.0f;
    float p_vz = 0.0f;

    float rotX = 0.0f;
    float rotY = 0.0f;
    
    mesh p_mesh;
};
enum DrawFlag
{
    Vec,
    Tri,
    FillTri
};
struct Player
{
    vec3d p_Camera;
    vec3d vLookDir;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float p_PlayerSpeed = 4.0f;
    
    // Flags for rendering
    DrawFlag DrawChoice = FillTri;
    bool ShowPlayerInfo = true;
    float Scale = 1.0f;
};