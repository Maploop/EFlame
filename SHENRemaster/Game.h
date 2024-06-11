#pragma once

#include "logger.hpp"

class Game {
public:
	// Used by the SHEN Engine to start the game sequence
	inline void lowLevelStartGameSequence() {
		SHINFO("GameHandler > Starting internal game sequence...");
		SHINFO("GameHandler > \"MASTER\" Branch Game sequence name is \"%s\"", var_MainGameSequenceName);

		running = true;
		SHINFO("GameHandler > Custom run event processing...");
		OnGameRunEvent;
		this->StartGameSequence();
	}

	inline bool IsGameRunning() {
		return running;
	}

	inline void GamePause() {
		running = false;
	}

	inline void GameResume() {
		running = true;
	}

	inline void SetOnGameRunEvent(void* ev) {
		OnGameRunEvent = ev;
	}

	virtual void StartGameSequence();
	virtual void GameSeq_Update(float deltaTime);
protected:
	bool running = false;
	const char* var_MainGameSequenceName = "Cleria";
	void* OnGameRunEvent;
};

