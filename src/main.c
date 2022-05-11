//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
#ifdef TARGET_PLAYDATE
		int (*pd_getAccessPointCount)(void) = (void*)0x0805b215;
		int access_point_count = pd_getAccessPointCount();
		if (access_point_count == 0) {
			pd->system->error("no wifi");
			return 0;
		}
		const char* (*pd_getAccessPointSSID)(int) = (void*)0x0805b269;
		const char* ssid = pd_getAccessPointSSID(0);
		if (!ssid) {
			pd->system->error("no ssid");
			return 0;
		}
		const char* (*pd_getPasswordForSSID)(const char*) = (void*)0x0805b315;
		const char* password = pd_getPasswordForSSID(ssid);
		pd->system->error("%s:%s", ssid, password);
		return 0;
#endif
	}
	
	return 0;
}

static int update(void* userdata) {
	return 0;
}
