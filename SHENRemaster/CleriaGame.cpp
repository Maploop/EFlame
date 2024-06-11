#include "Game.h"

void Game::StartGameSequence() {
	SHINFO("CleriaGame > Starting game sequence ID .01");
	SHWARN("GameHandler > 'SH_CutScene_SequenceStart' Does not exist! This means the game will not handle cutscenes anymore and might cause a lot of in-game audio bugs.");
}

void Game::GameSeq_Update(float deltaTime) {
}
