#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE, (float)DISPLAY_WIDTH/DISPLAY_HEIGHT, NEAR, FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();
	scn->Resize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	scn->AddShape(scn->Plane, -1, scn->TRIANGLES);
	scn->MoveCamera(0, scn->zTranslate, 10);

	display.SetScene(scn);

	scn->AddTexture("../res/textures/box0.bmp", false);
	//scn->AddTexture("../res/textures/grass.bmp", false);
	//scn->AddTexture("../res/textures/bricks.jpg", false);
	//scn->AddTexture("../res/textures/snake1.png", false);

	while(!display.CloseWindow())
	{
		scn->SetShapeTex(0, 0);
		scn->Draw(1, 0, scn->BACK, true, false);

		//scn->SetShapeTex(0, 0);
		//scn->Draw(1, 0, scn->BACK, true, false, 0);
		//
		//scn->SetShapeTex(0, 1);
		//scn->Draw(1, 0, scn->BACK, false, false, 1);
		//
		//scn->SetShapeTex(0, 2);
		//scn->Draw(1, 0, scn->BACK, false, false, 2);
		//
		//scn->SetShapeTex(0, 3);
		//scn->Draw(1, 0, scn->BACK, false, false, 3);

		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();
	}
	delete scn;
	return 0;
}
