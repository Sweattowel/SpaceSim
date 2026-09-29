#include "createModel.h"


#include "../../olcPixelGameEngine.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

mesh CreateModel::CreateSphereMesh(int columns, int rows, float size, vec3d pos)
{
    std::cout << "Creating sphere mesh" << "\n";
    
    float PI = 3.1415926535f;
    float radius = size / 2;

    mesh Result;

    std::vector<vec3d> Cache;

    for (size_t i = 0; i < rows; i++)
    {
        float theta = PI * i / ( rows - 1 );

        for (size_t j = 0; j < columns; j++)
        {
            float phi = 2 * PI * j / (columns - 1);
        
            vec3d newPoint;

            newPoint.x = radius * sinf(theta) * cosf(phi) + pos.x;
            newPoint.y = radius * sinf(theta) * sinf(phi) + pos.y;
            newPoint.z = radius * cosf(theta) + pos.z;
            
            Cache.push_back(newPoint);
        }
    }

    for (int i = 0; i < rows - 1; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            int nextJ = (j + 1) % columns;

            int p0 = i * columns + j;
			int p1 = i * columns + nextJ;
			int p2 = (i + 1) * columns + j;
			int p3 = (i + 1) * columns + nextJ;

            triangle triA = {Cache[p0], Cache[p2], Cache[p1]};
            triangle triB = {Cache[p1], Cache[p2], Cache[p3]};

            Result.triangles.push_back(triA);
            Result.triangles.push_back(triB);
        }
    }

    return Result;
};
mesh CreateModel::CreateCubeMesh(float size, vec3d pos)
{
    mesh Result;

        
    Result.triangles = 
    {
    // SOUTH
    { 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,}, 
    { 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
                                                                                                
    // EAST           																			   
    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
                                                                                                
    // NORTH           																			   
    { 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
                                                                                                
    // WEST            																			   
    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
                                                                                                
    // TOP             																			   
    { 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
    { 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
                                                                                                
    // BOTTOM          																			  
    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

    };
    
    for (triangle& tri : Result.triangles)
    {
        tri.p[0].x += pos.x;
        tri.p[0].y += pos.y;
        tri.p[0].z += pos.z;

        tri.p[1].x += pos.x;
        tri.p[1].y += pos.y;
        tri.p[1].z += pos.z;
        
        tri.p[2].x += pos.x;
        tri.p[2].y += pos.y;
        tri.p[2].z += pos.z;

        continue;
        tri.p[0].x *= size;
        tri.p[0].y *= size;
        tri.p[0].z *= size;

        tri.p[1].x *= size;
        tri.p[1].y *= size;
        tri.p[1].z *= size;
        
        tri.p[2].x *= size;
        tri.p[2].y *= size;
        tri.p[2].z *= size;
    }

    return Result;
}