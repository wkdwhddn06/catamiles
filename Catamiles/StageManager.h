#pragma once

typedef struct GAME_INFO
{
	int hard;
	int stage;
}GAME_INFO;

GAME_INFO startGame()
{
	GAME_INFO tmp;
	tmp.hard = 1;
	tmp.stage = 0;
	return tmp;
}

void beginStage(HINSTANCE hInst, MAP* test, ArrayList* ptrCol, ArrayList* enemy_list, GAME_INFO * info, ENEMYS enemys, MAP_ROOM_COMPONENT comp)
{
	STATE follow_player = createState(STATE_FOLLOW_PLAYER, 40);
	STATE shoot_to_player = createState(STATE_SHOOT_TO_PLAYER, 20);
	STATE shoot_player_with_dist = createState(STATE_FOLLOW_PLAYER_WITH_DISTANCE, 40);
	STATE move_random = createState(STATE_MOVE_RANDOM, 20);
	STATE charge_to_player_and_boom = createState(STATE_CHARGE_AND_BOOM, 40);
	STATE follow_and_shoot_with_distance = createState(STATE_FOLLOW_AND_SHOOT_WITH_DISTANCE, 40);

	info->stage++;
	info->hard++;
	POINT room_size = cp(random(3, 5) * 8, random(3, 5) * 6);
	drawSqureRoom(room_size, &(test->layer[0]), ptrCol, comp);

	drawPillar(cp(room_size.x / 3, room_size.y / 3), cp(random(6, 14), random(6, 14)), &(test->layer[0]), ptrCol, comp);

	//¸êÄ¡
	for (int i = 0; i < info->hard % 8;)
	{
		STATE_PATTERN state;
		state.size = 0;
		state.cursor = 0;

		addStatePattern(&state, shoot_player_with_dist);
		addStatePattern(&state, shoot_to_player);
		addStatePattern(&state, shoot_player_with_dist);
		addStatePattern(&state, shoot_to_player);

		ENEMY tmp = createEnemy(hInst, cp(random(5, room_size.x - 5) * 64, random(5, room_size.y - 5) * 64), IDB_DRONI_IDLE_1, state, enemys.DRONI);
		
		if (isColideWithColiders(tmp.colider, ptrCol))
		{
			addEnemyItem(enemy_list, tmp);
			i++;
		}
		Sleep(10);
	}

	//¹Ì´õ´ö
	for (int i = 0; i < random(0, 4) && info->hard > 5;)
	{
		STATE_PATTERN state;
		state.size = 0;
		state.cursor = 0;

		addStatePattern(&state, charge_to_player_and_boom);
		addStatePattern(&state, charge_to_player_and_boom);

		ENEMY tmp = createEnemy(hInst, cp(random(5, room_size.x - 5) * 64, random(5, room_size.y - 5) * 64), IDB_MIDEDUK, state, enemys.Mideduk);

		if (isColideWithColiders(tmp.colider, ptrCol))
		{
			addEnemyItem(enemy_list, tmp);
			i++;
		}
		Sleep(10);
	}

	//³ÒÄ¡
	for (int i = 0; i < random(0, 3) && info->hard > 10;)
	{
		STATE_PATTERN state;
		state.size = 0;
		state.cursor = 0;

		addStatePattern(&state, follow_and_shoot_with_distance);
		addStatePattern(&state, follow_and_shoot_with_distance);

		ENEMY tmp = createEnemy(hInst, cp(random(5, room_size.x - 5) * 64, random(5, room_size.y - 5) * 64), IDB_NUP_MOVE_1, state, enemys.Nup);

		if (isColideWithColiders(tmp.colider, ptrCol))
		{
			addEnemyItem(enemy_list, tmp);
			i++;
		}
		Sleep(10);
	}

}