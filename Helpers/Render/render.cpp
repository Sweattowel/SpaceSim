#include "render.h"

#include <algorithm>
#include <cstddef>
#include <list>
#include <string>
#include <vector>
#include "../../Helpers/Math/math.h"

std::list<triangle> CreateFrame(std::vector<mesh>& ToDraw, float ScreenHeight, float ScreenWidth, float p_fov, Player& PlayerInfo, mat4x4& matProj, float& pDepthBuffer)
{
    // return result
    std::list<triangle> Result;

    // The New intended order of changes to apply to the mesh for drawing, in this order
    // transformed, viewed, clipped, projected, sorted, clipped triangles
    mat4x4 matWorld = CreateMatWorld();
    mat4x4 matView = CreateMatView(PlayerInfo);


    std::vector<triangle> vecTrianglesToRaster;

    for (mesh& mesh : ToDraw)
    {
        for (triangle tri : mesh.triangles)
        {
            TransformTriangle(tri, matWorld);

            vec3d normal = GetNormal(tri, PlayerInfo.p_Camera);
            vec3d vCameraRay = Vector_Sub(tri.p[0], PlayerInfo.p_Camera);

            if (Vector_DotProduct(normal, vCameraRay) > 0.0f) continue;
            vec3d Sun = {0.1f, -100.0f, 0.1f};
            float light = Vector_DotProduct(normal, Sun);
            
            tri.col.r *= light;
            tri.col.g *= light;
            tri.col.b *= light;

            ConvertToViewSpace(tri, matView);
            int nClippedTriangles = 0;
            triangle clipped[2];

            nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, tri, clipped[0], clipped[1]);
            
            for (int n = 0; n < nClippedTriangles; n++)
            {
                ProjectTo3D(tri, matProj, clipped[n]);
                ScaleIntoView(tri);
                invertTriXY(tri);
                offsetToNormalisedSpace(tri, ScreenHeight, ScreenWidth);

                vecTrianglesToRaster.push_back(tri);
            }
            std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
            {
                float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
                float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
                return z1 > z2;
            });

            ClipTriangles(Result, vecTrianglesToRaster, ScreenHeight, ScreenWidth);

        }
    }

    
    return Result;


}
void RenderFrame(olc::PixelGameEngine& engine, std::list<triangle> CreatedFrame, Player& p)
{
    engine.FillRect(0,0, engine.ScreenWidth(), engine.ScreenHeight(), olc::BLACK);

    switch (p.DrawChoice) 
    {
        case Vec:
            for (triangle& tri : CreatedFrame)
            {
                engine.DrawTriangle(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, tri.p[2].x,tri.p[2].y, tri.col);
            }
        break;
        case Tri:
            for (triangle& tri : CreatedFrame)
            {
                engine.Draw(tri.p[0].x, tri.p[0].y, tri.col);
                engine.Draw(tri.p[1].x, tri.p[1].y, tri.col);
                engine.Draw(tri.p[2].x, tri.p[2].y, tri.col);
            }
        break;
        case FillTri:
            for (triangle& tri : CreatedFrame)
            {
                engine.FillTriangle(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, tri.p[2].x,tri.p[2].y, tri.col);
            }
        break;
        default:
            for (triangle& tri : CreatedFrame)
            {
                engine.DrawTriangle(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, tri.p[2].x,tri.p[2].y, tri.col);
            }
        break;
    }

    
    if (p.ShowPlayerInfo)
    {
        std::vector<std::string> ToDrawStrings;

        ToDrawStrings.push_back( 
            "X:" + std::to_string(p.p_Camera.x) +
            "Y:" + std::to_string(p.p_Camera.y) +
            "Z:" + std::to_string(p.p_Camera.z)
        );
        
        ToDrawStrings.push_back("SC" + std::to_string(p.Scale));

        switch (p.DrawChoice) {
            case Vec: ToDrawStrings.push_back("DC: Vec"); break;
            case Tri: ToDrawStrings.push_back("DC: Tri"); break;
            case FillTri: ToDrawStrings.push_back("DC: FTri"); break;
            default : ToDrawStrings.push_back("Broken"); break;
        }
        


        float height = engine.GetTextSize("S").y;

        for (size_t i = 0; i < ToDrawStrings.size(); i++)
        {
            std::string& str = ToDrawStrings[i];
            engine.DrawString(0.0f, (i * height) + (i * p.Scale), str, olc::WHITE, p.Scale);
        }
    }
}
mat4x4 CreateMatWorld(float fYaw, float fTheta)
{
    mat4x4 matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
    mat4x4 matRotX = Matrix_MakeRotationX(fYaw);

    mat4x4 matTrans = Matrix_MakeTranslation(0.0f, 0.0f,5.0f);
    
    mat4x4 matWorld = Matrix_MakeIdentity();
    matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    return  matWorld;
}
mat4x4 CreateMatView(Player& PlayerInfo)
{
    vec3d vUp = { 0.0, 1.0, 0.0 };
    vec3d vTarget = {0.0f, 0.0f,1.0f };

    mat4x4 matCameraRotX = Matrix_MakeRotationX(PlayerInfo.pitch);
    mat4x4 matCameraRotY = Matrix_MakeRotationY(PlayerInfo.yaw);
    mat4x4 matCameraRotZ = Matrix_MakeRotationZ(PlayerInfo.roll);

    vec3d vDir = Matrix_MultiplyVector(matCameraRotZ, vTarget);
    vec3d vDir2 = Matrix_MultiplyVector(matCameraRotX, vDir);
    PlayerInfo.vLookDir = Matrix_MultiplyVector(matCameraRotY, vDir2);
    
    vTarget = Vector_Add(PlayerInfo.p_Camera, PlayerInfo.vLookDir);
    mat4x4 matCamera = Matrix_PointAt(PlayerInfo.p_Camera, vTarget, vUp);

    mat4x4 matView = Matrix_QuickInverse(matCamera);

    return matView;
}
void TransformTriangle(triangle& tri, mat4x4& matWorld)
{
    tri.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
    tri.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
    tri.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
}
vec3d GetNormal(triangle& tri, vec3d& p_Camera)
{
    vec3d line1, line2;

    line1 = Vector_Sub(tri.p[1], tri.p[0]);
    line2 = Vector_Sub(tri.p[2], tri.p[0]);

    vec3d normal = Vector_CrossProduct(line1, line2);

    normal = Vector_Normalise(normal);

    return  normal;
}
void ConvertToViewSpace(triangle& tri, mat4x4& matView)
{
    tri.p[0] = Matrix_MultiplyVector(matView, tri.p[0]);
    tri.p[1] = Matrix_MultiplyVector(matView, tri.p[1]);
    tri.p[2] = Matrix_MultiplyVector(matView, tri.p[2]);
}
void ProjectTo3D(triangle& tri, mat4x4& matProj, triangle& clippedN)
{
    tri.p[0] = Matrix_MultiplyVector(matProj, clippedN.p[0]);
    tri.p[1] = Matrix_MultiplyVector(matProj, clippedN.p[1]);
    tri.p[2] = Matrix_MultiplyVector(matProj, clippedN.p[2]);

    tri.t[0] = clippedN.t[0];
    tri.t[1] = clippedN.t[1];
    tri.t[2] = clippedN.t[2];

    tri.t[0].u = tri.t[0].u / tri.p[0].w;
    tri.t[1].u = tri.t[1].u / tri.p[1].w;
    tri.t[2].u = tri.t[2].u / tri.p[2].w;

    tri.t[0].v = tri.t[0].v / tri.p[0].w;
    tri.t[1].v = tri.t[1].v / tri.p[1].w;
    tri.t[2].v = tri.t[2].v / tri.p[2].w;

    tri.t[0].w = 1.0f / tri.p[0].w;
    tri.t[1].w = 1.0f / tri.p[1].w;
    tri.t[2].w = 1.0f / tri.p[2].w;
}

void ScaleIntoView(triangle& tri)
{
    tri.p[0] = Vector_Div(tri.p[0], tri.p[0].w);
    tri.p[1] = Vector_Div(tri.p[1], tri.p[1].w);
    tri.p[2] = Vector_Div(tri.p[2], tri.p[2].w);
}

void invertTriXY(triangle& tri)
{
    tri.p[0].x *= -1.0f;
    tri.p[1].x *= -1.0f;
    tri.p[2].x *= -1.0f;

    tri.p[0].y *= -1.0f;
    tri.p[1].y *= -1.0f;
    tri.p[2].y *= -1.0f;
}

void offsetToNormalisedSpace(triangle& tri, float ScreenHeight, float ScreenWidth)
{
    vec3d vOffsetView = {1.0f, 1.0f, 0.0f};

    tri.p[0] = Vector_Add(tri.p[0], vOffsetView);
    tri.p[1] = Vector_Add(tri.p[1], vOffsetView);
    tri.p[2] = Vector_Add(tri.p[2], vOffsetView);

    tri.p[0].x *= 0.5f * ScreenWidth;
    tri.p[0].y *= 0.5f * ScreenHeight;

    tri.p[1].x *= 0.5f * ScreenWidth;
    tri.p[1].y *= 0.5f * ScreenHeight;

    tri.p[2].x *= 0.5f * ScreenWidth;
    tri.p[2].y *= 0.5f * ScreenHeight;
}
void ClipTriangles(std::list<triangle>& Result, std::vector<triangle> vecTrianglesToRaster, float ScreenHeight, float ScreenWidth)
{
    for (auto& TriToRaster : vecTrianglesToRaster)
    {
        triangle clipped[2];
        std::list<triangle> listTriangles;

        listTriangles.push_back(TriToRaster);
        int nNewTriangles = 1;

        for(int p = 0; p < 4; p++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                triangle Front = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (p) 
                {
                    case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, Front, clipped[0], clipped[1]); break;
                    case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, ScreenHeight - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, Front, clipped[0], clipped[1]); break;
                    case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, Front, clipped[0], clipped[1]); break;
                    case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ ScreenWidth - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, Front, clipped[0], clipped[1]); break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                {
                    listTriangles.push_back(clipped[w]);
                }

            }
            nNewTriangles = listTriangles.size();
        }

        Result.insert(Result.end(), listTriangles.begin(), listTriangles.end() );
    }
}