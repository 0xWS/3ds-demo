#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <3ds.h>
#include <citro2d.h>
#include <stdarg.h>

#include "debug.hpp"
#include "spritesheet.hpp"

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

void printfile(const char* path)
{
	FILE* f = fopen(path, "r");
	if (f)
	{
		char mystring[100];
		while (fgets(mystring, sizeof(mystring), f))
		{
			int a = strlen(mystring);
			if (mystring[a-1] == '\n')
			{
				mystring[a-1] = 0;
				if (mystring[a-2] == '\r')
					mystring[a-2] = 0;
			}
			puts(mystring);
		}
		fclose(f);
	}
}

int main()
{
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

	Result rc = romfsInit();
	if (rc)
		printf("romfsInit: %08lX\n", rc);
	else
	{
		printf("Bouncing ball demo!\n\n");
		printfile("romfs:/folder/file.txt");
	}

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Create screens
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	SpriteManager spriteManager;
	if (!spriteManager.loadSpriteSheet("romfs:/output.t3x")) {
		printf("Error loading sprite sheet");
		return 0;
	}

	C2D_Image ballImage = spriteManager.getBallSprite();
	C2D_Image paddleImage = spriteManager.getPaddleSprite();

	int x_change = 1;
	int y_change = 1;

	float x = (TOP_SCREEN_WIDTH - ballImage.subtex->width) / 2.0f;
    float y = (TOP_SCREEN_HEIGHT - ballImage.subtex->height) / 2.0f;

	float playerOneX = 5.0f;
	float playerOneY = (TOP_SCREEN_HEIGHT / 2.0f);

	float playerTwoX = (TOP_SCREEN_WIDTH - 32.0f);
	float playerTwoY = (TOP_SCREEN_HEIGHT / 2.0f);

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		consoleClear();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break;
		}
		if (kDown & KEY_DUP) {
			playerOneY-=20;
			debugPrint(0,1, "DUP PRESSED");
		}
		if (kDown & KEY_DDOWN) {
			playerOneY+=20;
			debugPrint(0,1, "DDOWN PRESSED");
		}
		if (kDown & KEY_Y) {
			playerTwoY-=20;
			debugPrint(0,1, "DUP PRESSED");
		}
		if (kDown & KEY_B) {
			playerTwoY+=20;
			debugPrint(0,1, "DDOWN PRESSED");
		}
		

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0, 0.0, 1.0, 1.0));
		C2D_SceneBegin(top);

        // Draw the image
        C2D_DrawImageAt(ballImage, x, y, 0.5f, NULL, 1.0f, 1.0f);
		C2D_DrawImageAt(paddleImage, playerOneX, playerOneY, 0.5f, NULL, 1.0f, 1.0f);
		C2D_DrawImageAt(paddleImage, playerTwoX, playerTwoY, 0.5f, NULL, 1.0f, 1.0f);

		x += x_change * 2.0f;
		y += y_change * 2.0f;

		//Ball bounce
		if(x >= TOP_SCREEN_WIDTH - ballImage.subtex->width) x_change = -1;
		if(x <= 0) x_change = 1;
		if(y >= TOP_SCREEN_HEIGHT - ballImage.subtex->height) y_change = -1;
		if(y <= 0) y_change = 1;

		


		debugPrint(0, 0, "Ball pos: (%.2f, %.2f)", x, y);

        C3D_FrameEnd(0);
	}

	romfsExit();
	gfxExit();
	return 0;
}