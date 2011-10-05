/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


#include <hge.h>

using namespace hge;

HGE *g_hge = 0;

// This function will be called by HGE once per frame.
// Put your game loop code here. In this example we
// just check whether ESC key has been pressed.
bool FrameFunc()
{
	// By returning "true" we tell HGE
	// to stop running the application.
	if (g_hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	// Continue execution
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Here we use global pointer to HGE interface.
	// Instead you may use hgeCreate() every
	// time you need access to HGE. Just be sure to
	// have a corresponding g_hge->Release()
	// for each call to hgeCreate()
	g_hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	g_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

	// Set the window title
	g_hge->System_SetState(HGE_TITLE, "HGE Tutorial 01 - Minimal HGE application");
	
	// Run in windowed mode
	// Default window size is 800x600
	g_hge->System_SetState(HGE_WINDOWED, true);

	// Don't use BASS for sound
	g_hge->System_SetState(HGE_USESOUND, false);

	// Tries to initiate HGE with the states set.
	// If something goes wrong, "false" is returned
	// and more specific description of what have
	// happened can be read with System_GetErrorMessage().
	if(g_hge->System_Initiate())
	{
		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc().
		g_hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, g_hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free
	// all allocated resources
	g_hge->System_Shutdown();

	// Release the HGE interface.
	// If there are no more references,
	// the HGE object will be deleted.
	g_hge->Release();

	return 0;
}
