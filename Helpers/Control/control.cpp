#include "control.h"
#include "../../models.h"
#include "../CreateModel/createModel.h"
#include <cmath>

void HandleControls(olc::PixelGameEngine& Engine, Player& PlayerInfo, float fElapsedTime, std::vector<mesh>& toDraw)
{
    vec3d vTarget = {0.0f, 0.0f,1.0f };
    mat4x4 matCameraRot = Matrix_MakeRotationY(PlayerInfo.yaw);
    vec3d vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);

    vec3d vForward = Vector_Mul(vLookDir, 8.0f * fElapsedTime);
    vec3d vUp = { 0.0f, 1.0f, 0.0f};
    vec3d vRight = Vector_CrossProduct(vForward, vUp);
    vUp = Vector_Mul(vUp, 8.0f * fElapsedTime);

    // Rotate Camera
    if (Engine.GetKey(olc::Key::LEFT).bHeld) PlayerInfo.yaw -= PlayerInfo.p_PlayerSpeed * fElapsedTime;
    if (Engine.GetKey(olc::Key::RIGHT).bHeld) PlayerInfo.yaw += PlayerInfo.p_PlayerSpeed * fElapsedTime;
    if (Engine.GetKey(olc::Key::UP).bHeld) PlayerInfo.pitch -= PlayerInfo.p_PlayerSpeed * fElapsedTime;
    if (Engine.GetKey(olc::Key::DOWN).bHeld) PlayerInfo.pitch += PlayerInfo.p_PlayerSpeed * fElapsedTime;
    
    if (Engine.GetKey(olc::Key::Q).bHeld) PlayerInfo.roll -= PlayerInfo.p_PlayerSpeed * fElapsedTime;
    if (Engine.GetKey(olc::Key::E).bHeld) PlayerInfo.roll += PlayerInfo.p_PlayerSpeed * fElapsedTime;
    // Move Position
    if (Engine.GetKey(olc::Key::W).bHeld) PlayerInfo.p_Camera = Vector_Add(PlayerInfo.p_Camera, vForward);
    if (Engine.GetKey(olc::Key::S).bHeld) PlayerInfo.p_Camera = Vector_Sub(PlayerInfo.p_Camera, vForward);

    if (Engine.GetKey(olc::Key::A).bHeld) PlayerInfo.p_Camera = Vector_Sub(PlayerInfo.p_Camera, vRight);
    if (Engine.GetKey(olc::Key::D).bHeld) PlayerInfo.p_Camera = Vector_Add(PlayerInfo.p_Camera, vRight);

    if (Engine.GetKey(olc::Key::SPACE).bHeld) PlayerInfo.p_Camera = Vector_Add(PlayerInfo.p_Camera, vUp);
    if (Engine.GetKey(olc::Key::CTRL).bHeld) PlayerInfo.p_Camera = Vector_Sub(PlayerInfo.p_Camera, vUp);

    //if (Engine.GetKey(olc::Key::F).bHeld) PlayerInfo.p_Camera.z += PlayerInfo.p_PlayerSpeed * fElapsedTime;
    //if (Engine.GetKey(olc::Key::R).bHeld) PlayerInfo.p_Camera.z -= PlayerInfo.p_PlayerSpeed * fElapsedTime;
    /*
    if (PlayerInfo.p_Theta > 360) PlayerInfo.p_Theta = 0.0f;
    if (PlayerInfo.p_Theta < 0) PlayerInfo.p_Theta = 360.0f;
    
    if (PlayerInfo.p_Phi > 360) PlayerInfo.p_Phi = 0.0f;
    if (PlayerInfo.p_Phi < 0) PlayerInfo.p_Phi = 360.0f;
    */

    if (Engine.GetKey(olc::Key::R).bHeld) 
    {
        PlayerInfo.p_Camera = { 1.0f, 0.0f, 0.0f };
        PlayerInfo.yaw = 0.0f;
        PlayerInfo.pitch = 0.0f;
        PlayerInfo.roll = 0.0f;
    };
    if (Engine.GetKey(olc::Key::P).bPressed) PlayerInfo.ShowPlayerInfo = !PlayerInfo.ShowPlayerInfo;
    if (Engine.GetKey(olc::Key::O).bPressed) 
    {
        //    Vec,
        //    Tri,
        //    FillTri

        switch (PlayerInfo.DrawChoice) {
            case Vec:
                PlayerInfo.DrawChoice = Tri;
            break;
            case Tri:
                PlayerInfo.DrawChoice = FillTri;
            break;
            case FillTri:
                PlayerInfo.DrawChoice = Vec;
            break;
        }
    };


    if (Engine.GetKey(olc::Key::K).bPressed)
    {
        vec3d newPos = Vector_Add(PlayerInfo.p_Camera, vForward);
        
        mesh newSphere = CreateModel::CreateSphereMesh(10,10,10,newPos);
        toDraw.push_back((newSphere)); 
    }
    if (Engine.GetKey(olc::Key::L).bPressed)
    {
        vec3d newPos = Vector_Add(PlayerInfo.p_Camera, vForward);

        mesh newCube = CreateModel::CreateCubeMesh(10, newPos);
        toDraw.push_back((newCube)); 
    }
    if (Engine.GetKey(olc::Key::M).bPressed)
    {
        toDraw.clear();
    }

    if (Engine.GetKey(olc::Key::NP_ADD).bPressed) PlayerInfo.Scale += 1.0f;
    if (Engine.GetKey(olc::Key::NP_SUB).bPressed) PlayerInfo.Scale = std::fmax(0.0f, (PlayerInfo.Scale - 1.0f));
}