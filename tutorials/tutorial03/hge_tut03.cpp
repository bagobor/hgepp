/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** g_hge.relishgames.com
**
** hge_tut03 - Using helper classes
*/


// Copy the files "particles.png", "menu.wav",
// "font1.fnt", "font1.png" and "trail.psi" from
// the folder "precompiled" to the folder with
// executable file. Also copy g_hge.dll and bass.dll
// to the same folder.


#include <hgepp.h>
#include <hgepp/sprite.h>
#include <hgepp/font.h>
#include <hgepp/particle.h>

using namespace hge;

// Pointer to the HGE interface.
// Helper classes require this to work.
HGE * g_hge=nullptr;


// Pointers to the HGE objects we will use
hgeSprite*			spr;
hgeSprite*			spt;
hgeFont*			fnt;
hgeParticleSystem*	par;

// Handles for HGE resourcces
HTEXTURE			tex;
HEFFECT				snd;

// Some "gameplay" variables
float x=100.0f, y=100.0f;
float dx=0.0f, dy=0.0f;

const float speed=90;
const float friction=0.98f;

// Play sound effect
void boom() {
	int pan=int((x-400)/4);
	float pitch=(dx*dx+dy*dy)*0.0005f+0.2f;
	g_hge->Effect_PlayEx(snd,100,pan,pitch);
}

bool FrameFunc()
{
	float dt=g_hge->Timer_GetDelta();

	// Process keys
	if (g_hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	if (g_hge->Input_GetKeyState(HGEK_LEFT)) dx-=speed*dt;
	if (g_hge->Input_GetKeyState(HGEK_RIGHT)) dx+=speed*dt;
	if (g_hge->Input_GetKeyState(HGEK_UP)) dy-=speed*dt;
	if (g_hge->Input_GetKeyState(HGEK_DOWN)) dy+=speed*dt;

	// Do some movement calculations and collision detection	
	dx*=friction; dy*=friction; x+=dx; y+=dy;
	if(x>784) {x=784-(x-784);dx=-dx;boom();}
	if(x<16) {x=16+16-x;dx=-dx;boom();}
	if(y>584) {y=584-(y-584);dy=-dy;boom();}
	if(y<16) {y=16+16-y;dy=-dy;boom();}

	// Update particle system
	par->info.nEmission=(int)(dx*dx+dy*dy)*2;
	par->MoveTo(x,y);
	par->Update(dt);

	return false;
}


bool RenderFunc()
{
	// Render graphics
	g_hge->Gfx_BeginScene();
	g_hge->Gfx_Clear(0);
	par->Render();
	spr->Render(x, y);
	fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", g_hge->Timer_GetDelta(), g_hge->Timer_GetFPS());
	g_hge->Gfx_EndScene();

	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	g_hge = hgeCreate(HGE_VERSION);

	g_hge->System_SetState(HGE_LOGFILE, "hge_tut03.log");
	g_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	g_hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	g_hge->System_SetState(HGE_TITLE, "HGE Tutorial 03 - Using helper classes");
	g_hge->System_SetState(HGE_FPS, 100);
	g_hge->System_SetState(HGE_WINDOWED, true);
	g_hge->System_SetState(HGE_SCREENWIDTH, 800);
	g_hge->System_SetState(HGE_SCREENHEIGHT, 600);
	g_hge->System_SetState(HGE_SCREENBPP, 32);

	if(g_hge->System_Initiate()) {

		// Load sound and texture
		snd=g_hge->Effect_Load("menu.wav");
		tex=g_hge->Texture_Load("particles.png");
		if(!snd || !tex)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nMENU.WAV, PARTICLES.PNG, FONT1.FNT, FONT1.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			g_hge->System_Shutdown();
			g_hge->Release();
			return 0;
		}

		// Create and set up a sprite
		spr=new hgeSprite(tex, 96, 64, 32, 32);
		spr->SetColor(0xFFFFA000);
		spr->SetHotSpot(16,16);

		// Load a font
		fnt=new hgeFont("font1.fnt");

		// Create and set up a particle system
		spt=new hgeSprite(tex, 32, 32, 32, 32);
		spt->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spt->SetHotSpot(16,16);
		par=new hgeParticleSystem("trail.psi",spt);
		par->Fire();

		// Let's rock now!
		g_hge->System_Start();

		// Delete created objects and free loaded resources
		delete par;
		delete fnt;
		delete spt;
		delete spr;
		g_hge->Texture_Free(tex);
		g_hge->Effect_Free(snd);
	}

	// Clean up and shutdown
	g_hge->System_Shutdown();
	g_hge->Release();
	return 0;
}
