#include <stdio.h>

#include "GameManager.h"

int main(int argc, char* args[])
{
	GameManager* gameManager = &GameManager();
	GameManager::RegisterInstance(gameManager);

	if (!GameManager::Get()->Initialize())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	GameManager::Get()->PlayGame();

	GameManager::Get()->Destroy();
	GameManager::UnregisterInstance(gameManager);

	return 0;
}
