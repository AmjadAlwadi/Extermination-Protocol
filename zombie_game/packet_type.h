#pragma once

enum class packet_type {
	NOTHING,
	START_GAME,
	PLAYER_INFO,
	PLAYER_JOIN,
	SET_DIFFICULTY_LEVEL,
	PLAYER_DATA,
	BULLET_DATA,
	ZOMBIE_DATA
};