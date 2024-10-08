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

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 16

#define BALL_WIDTH 16
#define BALL_HEIGHT 16

int main()
{
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

	Result rc = romfsInit();
	if (rc) printf("romfsInit: %08lX\n", rc);

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

	int playerOnePts = 0;
	int playerTwoPts = 0;

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START) {
			break;
		}
		if (kHeld & KEY_DUP && playerOneY > 0) {
			playerOneY-=3;
		}
		if (kHeld & KEY_DDOWN && playerOneY < TOP_SCREEN_HEIGHT - PADDLE_HEIGHT) {
			playerOneY+=3;
		}
		if (kHeld & KEY_X && playerTwoY > 0) {
			playerTwoY-=3;
		}
		if (kHeld & KEY_B && playerTwoY < TOP_SCREEN_HEIGHT - PADDLE_HEIGHT) {
			playerTwoY+=3;
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
		if(x >= TOP_SCREEN_WIDTH - ballImage.subtex->width) {
			x = (TOP_SCREEN_WIDTH - ballImage.subtex->width) / 2.0f;
 			y = (TOP_SCREEN_HEIGHT - ballImage.subtex->height) / 2.0f;
			playerOnePts++;
		}
		if(x <= 0) {
			x = (TOP_SCREEN_WIDTH - ballImage.subtex->width) / 2.0f;
 			y = (TOP_SCREEN_HEIGHT - ballImage.subtex->height) / 2.0f;
			playerTwoPts++;
		}
		if(y >= TOP_SCREEN_HEIGHT - ballImage.subtex->height) y_change = -1;
		if(y <= 0) y_change = 1;

		//Bounce from paddle
		if (x <= playerOneX + PADDLE_WIDTH &&
		    x + BALL_WIDTH >= playerOneX &&
		    y + BALL_HEIGHT >= playerOneY &&
		    y <= playerOneY + PADDLE_HEIGHT) {
		    x_change = 1;
		}

		if (x + BALL_WIDTH >= playerTwoX &&
		    x <= playerTwoX + PADDLE_WIDTH &&
		    y + BALL_HEIGHT >= playerTwoY &&
		    y <= playerTwoY + PADDLE_HEIGHT) {
		    x_change = -1;
		}

		consoleClear();
		debugPrint(0, 0, "Ball pos: (%.2f, %.2f)", x, y);
		debugPrint(0, 1, "Player 1 pos: (%.2f, %.2f)", playerOneX, playerOneY);
		debugPrint(0, 2, "Player 2 pos: (%.2f, %.2f)", playerTwoX, playerTwoY);
		debugPrint(0, 4, "Player 1: %d", playerOnePts);
		debugPrint(0, 5, "Player 2: %d", playerTwoPts);

        C3D_FrameEnd(0);
	}

	romfsExit();
	gfxExit();
	return 0;
}