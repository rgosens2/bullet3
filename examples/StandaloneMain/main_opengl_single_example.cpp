/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2015 Google Inc. http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "../CommonInterfaces/CommonExampleInterface.h"
#include "../CommonInterfaces/CommonGUIHelperInterface.h"
#include "../Utils/b3Clock.h"

#include "../OpenGLWindow/SimpleOpenGL3App.h"
#include <stdio.h>
#include "../ExampleBrowser/OpenGLGuiHelper.h"


///////////////////////////////////////
// RG: globals
CommonGraphicsApp* g_app = nullptr;

// NOTE: Declared in ForkLiftDemo.cpp
extern bool g_renderGrid;
///////////////////////////////////////





CommonExampleInterface* example;
int gSharedMemoryKey = -1;

b3MouseMoveCallback prevMouseMoveCallback = 0;
static void OnMouseMove(float x, float y)
{
	bool handled = false;
	handled = example->mouseMoveCallback(x, y);
	if (!handled)
	{
		if (prevMouseMoveCallback)
			prevMouseMoveCallback(x, y);
	}
}

b3MouseButtonCallback prevMouseButtonCallback = 0;
static void OnMouseDown(int button, int state, float x, float y)
{
	bool handled = false;

	handled = example->mouseButtonCallback(button, state, x, y);
	if (!handled)
	{
		if (prevMouseButtonCallback)
			prevMouseButtonCallback(button, state, x, y);
	}
}

class LessDummyGuiHelper : public DummyGUIHelper
{
	CommonGraphicsApp* m_app;

public:
	virtual CommonGraphicsApp* getAppInterface()
	{
		return m_app;
	}

	LessDummyGuiHelper(CommonGraphicsApp* app)
		: m_app(app)
	{
	}
};
int main(int argc, char* argv[])
{
    // RG: We do not get here for standalone build???
    // YESS: We now get here when we are using VS Code
    // So there is a main() after all.
    printf("QQQ\n");

	SimpleOpenGL3App* app = new SimpleOpenGL3App("Bullet Standalone Example", 1024, 768, true);

    // RG: global
    g_app = app;
    //printf("QQQ\n");

	prevMouseButtonCallback = app->m_window->getMouseButtonCallback();
	prevMouseMoveCallback = app->m_window->getMouseMoveCallback();

	app->m_window->setMouseButtonCallback((b3MouseButtonCallback)OnMouseDown);
	app->m_window->setMouseMoveCallback((b3MouseMoveCallback)OnMouseMove);

	OpenGLGuiHelper gui(app, false);
	//LessDummyGuiHelper gui(app);
	//DummyGUIHelper gui;

	CommonExampleOptions options(&gui);

	example = StandaloneExampleCreateFunc(options);
	example->processCommandLineArgs(argc, argv);

	example->initPhysics();
	example->resetCamera();

	b3Clock clock;

	do
	{
		app->m_instancingRenderer->init();
		app->m_instancingRenderer->updateCamera(app->getUpAxis());

		btScalar dtSec = btScalar(clock.getTimeInSeconds());
		if (dtSec > 0.1)
			dtSec = 0.1;

		example->stepSimulation(dtSec);
		clock.reset();

		example->renderScene();

        // RG: SHIT: even commenting out still draws the grid
        //printf("QQQ\n");
        // YESS: We now get here when we using VS Code

        // NOTE: We do this in ForkLiftDemo.cpp with a toggle
        // NONO: Do it here with a toggle from there
        //printf("No Grid\n");
        if (g_renderGrid) {
            printf("Render Grid\n");
            DrawGridData dg;
            dg.upAxis = app->getUpAxis();
            app->drawGrid(dg);
        }

		app->swapBuffer();
	} while (!app->m_window->requestedExit());

	example->exitPhysics();
	delete example;
	delete app;
	return 0;
}
