//#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
//#endif

#include "Engine.h"
#include "Game.h"
int main(int argc, char *argv []) {
	B00289996B00227422::Engine::GetInstance().Start();
	B00289996B00227422::Engine::GetInstance().SetGame(std::make_shared<B00289996B00227422::Game>());
	while(B00289996B00227422::Engine::GetInstance().IsRunning()) {
		B00289996B00227422::Engine::GetInstance().Run();
	}
	return 1;
}