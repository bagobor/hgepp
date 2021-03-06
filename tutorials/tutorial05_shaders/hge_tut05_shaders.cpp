/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** g_hge.relishgames.com
**
** hge_tut05 - Using distortion mesh
*/
#if HGE_DIRECTX_VER < 9
#error Shaders example requires HGE built with DirectX 9
#endif

// Copy the files "particles.png", "menu.wav",
// "font1.fnt", "font1.png" and "trail.psi" from
// the folder "precompiled" to the folder with
// executable file. Also copy g_hge.dll and bass.dll
// to the same folder.


#include <hgepp.h>
#include <hgepp/font.h>
#include <hgepp/distort.h>

#include <cmath>

using namespace hge;

// Pointer to the HGE interface.
// Helper classes require this to work.
HGE *g_hge=nullptr;

HTEXTURE            tex;
HSHADER             shad1;
HSHADER             shad2;
HSHADER             shad3;
HSHADER             currShader;

// Pointers to the HGE objects we will use
hgeDistortionMesh*  dis;
hgeFont*            fnt;

// Some "gameplay" variables
const int nRows=16;
const int nCols=16;
const float cellw=512.0f/(nCols-1);
const float cellh=512.0f/(nRows-1);

const float meshx=144;
const float meshy=44;

bool useShader = false;

bool FrameFunc()
{
    float dt=g_hge->Timer_GetDelta();
    static float t=0.0f;
    static int trans=0;

    int i, j, col;
    float r, a, dx, dy;

    t+=dt;

    // Process keys
    switch(g_hge->Input_GetKey())
    {
        case HGEK_ESCAPE:
            return true;

        case HGEK_SPACE:
            if(++trans > 2) trans=0;
            dis->Clear(0xFF000000);
            useShader = false;
            break;

        case HGEK_1:
            currShader.reset();
            break;

        case HGEK_2:
            currShader = shad1;
            break;

        case HGEK_3:
            currShader = shad2;
            break;

        case HGEK_4:
            currShader = shad3;
            break;
    }
    
    // Calculate new displacements and coloring for one of the three effects
    switch(trans)
    {
        case 0: for(i=1;i<nRows-1;i++)
                    for(j=1;j<nCols-1;j++)
                    {
                        dis->SetDisplacement(j,i,std::cosf(t*10+(i+j)/2)*5,std::sinf(t*10+(i+j)/2)*5,HGEDISP_NODE);
                    }
                break;

        case 1: for(i=0;i<nRows;i++)
                    for(j=1;j<nCols-1;j++)
                    {
                        dis->SetDisplacement(j,i,std::cosf(t*5+j/2)*15,0,HGEDISP_NODE);
                        col=int((std::cosf(t*5+(i+j)/2)+1)*35);
                        dis->SetColor(j,i,0xFF<<24 | col<<16 | col<<8 | col);
                    }
                break;

        case 2: for(i=0;i<nRows;i++)
                    for(j=0;j<nCols;j++)
                    {
                        r=std::sqrtf(std::powf(j-(float)nCols/2,2)+std::powf(i-(float)nRows/2,2));
                        a=r*std::cosf(t*2)*0.1f;
                        dx=std::sinf(a)*(i*cellh-256)+std::cosf(a)*(j*cellw-256);
                        dy=std::cosf(a)*(i*cellh-256)-std::sinf(a)*(j*cellw-256);
                        dis->SetDisplacement(j,i,dx,dy,HGEDISP_CENTER);
                        col=int((cos(r+t*4)+1)*40);
                        dis->SetColor(j,i,0xFF<<24 | col<<16 | (col/2)<<8);
                    }
                    break;
    }

    return false;
}


bool RenderFunc()
{
    // Render graphics
    g_hge->Gfx_BeginScene();
    g_hge->Gfx_Clear(0);
    g_hge->Gfx_SetShader(currShader);
    dis->Render(meshx, meshy);
    g_hge->Gfx_SetShader(HSHADER());
    fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d\n\nPress SPACE,1,2,3,4!", g_hge->Timer_GetDelta(), g_hge->Timer_GetFPS());
    g_hge->Gfx_EndScene();

    return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    g_hge = hgeCreate(HGE_VERSION);

    g_hge->System_SetState(HGE_LOGFILE, "hge_tut05.log");
    g_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    g_hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    g_hge->System_SetState(HGE_TITLE, "HGE Tutorial 05 - Using distortion mesh");
    g_hge->System_SetState(HGE_WINDOWED, true);
    g_hge->System_SetState(HGE_SCREENWIDTH, 800);
    g_hge->System_SetState(HGE_SCREENHEIGHT, 600);
    g_hge->System_SetState(HGE_SCREENBPP, 32);
    g_hge->System_SetState(HGE_USESOUND, false);

    if(g_hge->System_Initiate()) {

        // Load sound and texture
        tex=g_hge->Texture_Load("texture.jpg");
        if(!tex)
        {
            // If one of the data files is not found, display
            // an error message and shutdown.
            MessageBox(NULL, "Can't load TEXTURE.JPG", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            g_hge->System_Shutdown();
            g_hge->Release();
            return 0;
        }
       
        shad1 = g_hge->Shader_Create("shader1.psh");
        shad2 = g_hge->Shader_Create("shader2.psh");
        shad3 = g_hge->Shader_Create("shader3.psh");

        // Create a distortion mesh
        dis=new hgeDistortionMesh(nCols, nRows);
        dis->SetTexture(tex);
        dis->SetTextureRect(0,0,512,512);
        dis->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_ZWRITE);
        dis->Clear(0xFF000000);

        // Load a font
        fnt=new hgeFont("font1.fnt");

        // Let's rock now!
        g_hge->System_Start();

        // Delete created objects and free loaded resources
        delete fnt;
        delete dis;
        g_hge->Texture_Free(tex);
    }

    // Clean up and shutdown
    g_hge->System_Shutdown();
    g_hge->Release();
    return 0;
}
