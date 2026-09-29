#include "Helpers/Control/control.h"
#define OLC_PGE_APPLICATION

#include <vector>
#include "olcPixelGameEngine.h"

#include "models.h"
#include "./Helpers/CreateModel/createModel.h"
#include "./Helpers/Render/render.h"
#include "./Helpers/Math/math.h"

class Sim : public olc::PixelGameEngine
{
    
    public:
        Sim()
        {
            sAppName = "SpaceSim";
        }
        float *pDepthBuffer = nullptr;
        

    private:

        std::vector<mesh> ToDraw;
        Player PlayerInfo;

        mat4x4 matProj;
    public:
        bool OnUserCreate() override
        {
            pDepthBuffer = new float[ScreenWidth() * ScreenHeight()];

            matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

            mesh Sphere = CreateModel::CreateSphereMesh(9,9, 10.0f, PlayerInfo.p_Camera);
            mesh Cube = CreateModel::CreateCubeMesh(20.0f, {0.0f,0.0f,0.0f});
            ToDraw.push_back(Sphere);
            ToDraw.push_back(Cube);


            
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            HandleControls(*this, PlayerInfo, fElapsedTime, ToDraw);
            std::list<triangle> CreatedFrame =  CreateFrame(ToDraw, Sim::ScreenWidth(), Sim::ScreenHeight(), 90.0, PlayerInfo, matProj, *pDepthBuffer);
            RenderFrame(*this, CreatedFrame, PlayerInfo);

            return true;
        }
};

int main()
{
    Sim TestSimDemo;

    if (TestSimDemo.Construct(256,240,4,4))
        TestSimDemo.Start();

    return 0;
}