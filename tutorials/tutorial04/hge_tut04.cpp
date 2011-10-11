/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** g_hge.relishgames.com
**
** hge_tut04 - Using render targets
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

HGE *g_hge=nullptr;


hgeSprite*			spr;
hgeSprite*			spt;
hgeSprite*			tar;
hgeFont*			fnt;
hgeParticleSystem*	par;

HTEXTURE			tex;
HEFFECT				snd;

// HGE render target handle
HTARGET				target;

float x=100.0f, y=100.0f;
float dx=0.0f, dy=0.0f;

const float speed=90;
const float friction=0.98f;

void boom() {
	int pan=int((x-256)/2.56f);
	float pitch=(dx*dx+dy*dy)*0.0005f+0.2f;
	g_hge->Effect_PlayEx(snd,100,pan,pitch);
}

// This function will be called by HGE when
// render targets were lost and have been just created
// again. We use it here to update the render
// target's texture handle that changes during recreation.
bool GfxRestoreFunc()
{
	if(tar && target) tar->SetTexture(g_hge->Target_GetTexture(target));
	return false;
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
	if(x>496) {x=496-(x-496);dx=-dx;boom();}
	if(x<16) {x=16+16-x;dx=-dx;boom();}
	if(y>496) {y=496-(y-496);dy=-dy;boom();}
	if(y<16) {y=16+16-y;dy=-dy;boom();}

	// Update particle system
	par->info.nEmission=(int)(dx*dx+dy*dy);
	par->MoveTo(x,y);
	par->Update(dt);

	return false;
}


bool RenderFunc()
{
	int i;

	// Render graphics to the texture
	g_hge->Gfx_BeginScene(target);
	g_hge->Gfx_Clear(0);
	par->Render();
	spr->Render(x, y);
	g_hge->Gfx_EndScene();

	// Now put several instances of the rendered texture to the screen
	g_hge->Gfx_BeginScene();
	g_hge->Gfx_Clear(0);
	for(i=0;i<6;i++)
	{
		tar->SetColor(0xFFFFFF | (((5-i)*40+55)<<24));
		tar->RenderEx(i*100.0f, i*50.0f, i*M_PI/8, 1.0f-i*0.1f);
	}
	fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", g_hge->Timer_GetDelta(), g_hge->Timer_GetFPS());
	g_hge->Gfx_EndScene();

	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	g_hge = hgeCreate(HGE_VERSION);

	g_hge->System_SetState(HGE_LOGFILE, "hge_tut04.log");
	g_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	g_hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	g_hge->System_SetState(HGE_GFXRESTOREFUNC, GfxRestoreFunc);
	g_hge->System_SetState(HGE_TITLE, "HGE Tutorial 04 - Using render targets");
	g_hge->System_SetState(HGE_FPS, 100);
	g_hge->System_SetState(HGE_WINDOWED, true);
	g_hge->System_SetState(HGE_SCREENWIDTH, 800);
	g_hge->System_SetState(HGE_SCREENHEIGHT, 600);
	g_hge->System_SetState(HGE_SCREENBPP, 32);

	tar=0;
	//target = nullptr;

	if(g_hge->System_Initiate()) {
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

		spr=new hgeSprite(tex, 96, 64, 32, 32);
		spr->SetColor(0xFFFFA000);
		spr->SetHotSpot(16,16);

		fnt=new hgeFont("font1.fnt");

		spt=new hgeSprite(tex, 32, 32, 32, 32);
		spt->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spt->SetHotSpot(16,16);
		par=new hgeParticleSystem("trail.psi",spt);
		par->Fire();

		// Create a render target and a sprite for it
		target=g_hge->Target_Create(512,512,false);
		tar=new hgeSprite(g_hge->Target_GetTexture(target),0,0,512,512);
		tar->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);

		// Let's rock now!
		g_hge->System_Start();

		// Delete created objects and free loaded resources
		delete par;
		delete fnt;
		delete spt;
		delete spr;
		delete tar;
		g_hge->Target_Free(target);
		g_hge->Texture_Free(tex);
		g_hge->Effect_Free(snd);
	}

	// Clean up and shutdown
	g_hge->System_Shutdown();
	g_hge->Release();
	return 0;
}
