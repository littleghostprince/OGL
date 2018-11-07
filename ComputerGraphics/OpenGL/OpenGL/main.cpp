#include "Core/engine.h"
#include "Scenes//scene01.h"
#include "Scenes//scene02.h"
#include "Scenes//scene03.h"
#include"Scenes/scene04.h"
#include "Scenes/scene05.h"
#include "Scenes/scene06.h"

int main(int argc, char** argv)
{

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	Engine engine;
	engine.Initialize();
	engine.Get<FileSystem>()->SetPathname("resources\\");

	//Scene* scene = new Scene01(&engine);
	//scene->Initialize();

	//Scene* scene2 = new Scene02(&engine);
	//scene2->Initialize();
	
	Scene* scene = new Scene06(&engine);
	scene->Initialize();

	while (!engine.ForceQuit())
	{
		engine.Update();
		scene->Update();
		//scene2->Update();
		//scene03->Update();

		scene->Render();
		//scene2->Render();
		//scene03->Render();
	}
	scene->Shutdown();
	//scene2->Shutdown();
	//scene03->Shutdown();

	engine.Shutdown();

	return 0;
}

