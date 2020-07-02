#pragma once

#define ENEMY_DRONI 1

//적의 행동이 모여있는 함수 입니다
#define STATE_IDLE 1
#define STATE_FOLLOW_PLAYER 2
#define STATE_SHOOT_TO_PLAYER 3
#define STATE_FOLLOW_PLAYER_WITH_DISTANCE 4
#define STATE_MOVE_RANDOM 5
#define STATE_CHARGE_AND_BOOM 6
#define STATE_FOLLOW_AND_SHOOT_WITH_DISTANCE 7

typedef struct ENEMYS
{
	ENEMY_TYPE DRONI;
	ENEMY_TYPE Mideduk;
	ENEMY_TYPE Nup;
}ENEMYS;

STATE createState(int type, int leng)
{
	STATE task;
	task.type = type;
	task.length = leng * 10;

	return task;
}

STATE startState(STATE_PATTERN* tmp)
{
	if (tmp->cursor == tmp->size)
	{
		tmp->cursor = 0;
	}
	else if (tmp->states[tmp->cursor].timer != tmp->states[tmp->cursor].state.length)
	{
		tmp->states[tmp->cursor].timer += 1;
	}
	else 
	{
		tmp->states[tmp->cursor].timer = 0;
		tmp->cursor += 1;
	}

	return tmp->states[tmp->cursor].state;
}

void addStatePattern(STATE_PATTERN * tmp, STATE state)
{
	STATE_TASK task;
	task.state = state;
	task.timer = 0;

	tmp->states[tmp->size] = task;
	tmp->size += 1;
}

ENEMY createEnemy(HINSTANCE hInst, POINT point, int sprite, STATE_PATTERN pattern, ENEMY_TYPE type)
{
	ENEMY temp = { getColider(hInst,point,sprite), point, sprite, pattern, type};
	return temp;
}

ENEMY_TYPE createEnemyType(int health, int type, GUN gun, ANIMATION_TASK hited_task, ANIMATION_TASK idle_task, ANIMATION_TASK death_task)
{
	ENEMY_TYPE tmp;
	tmp.health_point = health;
	tmp.type = type;
	tmp.gun = gun;
	tmp.ani_hited_task = hited_task;
	tmp.ani_idle_task = idle_task;
	tmp.ani_death_task = death_task;
	return tmp;
}

void doEnemytask(HINSTANCE hInst, CAMERA * camera, ArrayList* enemy, ArrayList* list, ArrayList* bullet, ArrayList* coliders)
{
	ANIMATION_TASK * hited_task, * idle_task,  * death_task;
	COLIDE_INFO col;
	POINT point;
	STATE state;

	int w, h;

	for (int i = 0; i < enemy->size; i++)
	{
		// 애니메이션 등록
		hited_task = &(enemy->enemy_list[i].type.ani_hited_task);
		idle_task = &(enemy->enemy_list[i].type.ani_idle_task);
		death_task = &(enemy->enemy_list[i].type.ani_death_task);

		col = isColideForEnemy(enemy->enemy_list[i].colider, bullet); // 현재 충돌 정보 (충돌한 총알, 충돌 여부)
		state = startState(&(enemy->enemy_list[i].state));

		//패턴 반복
		if (!(enemy->enemy_list[i].type.health_point <= 0))
		{
			if (state.type == STATE_FOLLOW_PLAYER)
			{
				POINT v = MoveForward(enemy->enemy_list[i].point, list->game_list[0].point, 1);
				//위치 이동
				if(isColideWithMoving(&(enemy->enemy_list[i]),v,coliders))
					MoveWithColider(&(enemy->enemy_list[i]), v);
			}
			else if (state.type == STATE_MOVE_RANDOM)
			{
				POINT randomPoint = enemy->enemy_list[i].point;
				randomPoint.x += random(0, 2) * 10;
				randomPoint.y += random(0, 2) * 10;
				
				POINT v = MoveForward(enemy->enemy_list[i].point, randomPoint, 1);
				//위치 이동
				if (isColideWithMoving(&(enemy->enemy_list[i]), v, coliders))
					MoveWithColider(&(enemy->enemy_list[i]), v);
			}
			else if (state.type == STATE_FOLLOW_PLAYER_WITH_DISTANCE && getDistance(enemy->enemy_list[i].point, list->game_list[0].point) >= 400)
			{
				POINT v = MoveForward(enemy->enemy_list[i].point, list->game_list[0].point, 1);
				//위치 이동
				if (isColideWithMoving(&(enemy->enemy_list[i]), v, coliders))
					MoveWithColider(&(enemy->enemy_list[i]), v);
			}
			else if (state.type == STATE_SHOOT_TO_PLAYER)
			{
				ShootNoCamera(hInst,&(enemy->enemy_list[i].type.gun), enemy->enemy_list[i].point, list->game_list[0].point, list, bullet, TAG_ENEMY_BULLET);
			}	
			else if (state.type == STATE_CHARGE_AND_BOOM)
			{
				if (getDistance(enemy->enemy_list[i].point, list->game_list[0].point) <= 50)
				{
					enemy->enemy_list[i].type.health_point = -1;
				}
				else
				{
					POINT v = MoveForward(enemy->enemy_list[i].point, list->game_list[0].point, 3);
					//위치 이동
					if (isColideWithMoving(&(enemy->enemy_list[i]), v, coliders))
						MoveWithColider(&(enemy->enemy_list[i]), v);
				}
			}
			else if (state.type == STATE_FOLLOW_AND_SHOOT_WITH_DISTANCE && getDistance(enemy->enemy_list[i].point, list->game_list[0].point) >= 400)
			{
				ShootNoCamera(hInst, &(enemy->enemy_list[i].type.gun), enemy->enemy_list[i].point, list->game_list[0].point, list, bullet, TAG_ENEMY_BULLET);

				POINT v = MoveForward(enemy->enemy_list[i].point, list->game_list[0].point, 1);
				//위치 이동
				if (isColideWithMoving(&(enemy->enemy_list[i]), v, coliders))
					MoveWithColider(&(enemy->enemy_list[i]), v);
			}
		}

		if (col.isColide && col.bullet.type.explode.timer == 0) // 충돌한 총알에 처음 닿았을 때 HP 총알 데미지 만큼 감소
		{
			enemy->enemy_list[i].sprite = showAnimation(hited_task);
			enemy->enemy_list[i].type.health_point -= col.bullet.type.damage;
		}

		if (enemy->enemy_list[i].type.health_point <= 0)
		{
			if (death_task->cursor == 0 && death_task->timer == 0)
			{
				shakeCamera(camera, 10, 30);
			}
			else if (death_task->cursor == death_task->animation.size - 1)
			{
				int a;
				a = random(21, 30);
				StopSoundCUS(a);
				PlaySoundCUS("death1", a, 0);

				removeEnemyItem(enemy, i);
			}
			enemy->enemy_list[i].sprite = showAnimation(death_task);
		}
		else if (col.isColide && !(hited_task->cursor == hited_task->animation.size - 1)) //총알이랑 처음 충돌 했을 때
		{
			enemy->enemy_list[i].sprite = showAnimation(hited_task);
			//총알 맞으면 밀려나기 구현
			if (isColideWithMoving(&(enemy->enemy_list[i]), cp(col.bullet.direction.x * col.bullet.type.knockback, col.bullet.direction.y * col.bullet.type.knockback), coliders))
			{
				point = enemy->enemy_list[i].point;

				point.x += col.bullet.direction.x * col.bullet.type.knockback;
				point.y += col.bullet.direction.y * col.bullet.type.knockback;

				LerpForEnemy(&(enemy->enemy_list[i]), point, 2);

				w = enemy->enemy_list[i].colider.end_x - enemy->enemy_list[i].colider.start_x;
				h = enemy->enemy_list[i].colider.end_y - enemy->enemy_list[i].colider.start_y;

				enemy->enemy_list[i].colider.start_x = enemy->enemy_list[i].point.x;
				enemy->enemy_list[i].colider.end_x = enemy->enemy_list[i].point.x + w;
				enemy->enemy_list[i].colider.start_y = enemy->enemy_list[i].point.y;
				enemy->enemy_list[i].colider.end_y = enemy->enemy_list[i].point.y + h;
			}
		}
		else if(hited_task->cursor != 0 && hited_task->cursor != hited_task->animation.size - 1)
		{
			enemy->enemy_list[i].sprite = showAnimation(hited_task);
		}
		else if (hited_task->cursor == hited_task->animation.size - 1) //맞는 애니메이션 끝났을때
		{
			int a;
			a = random(21, 30);
			StopSoundCUS(a);
			PlaySoundCUS("death2", a, 0);
			enemy->enemy_list[i].sprite = hited_task->animation.frames[0];
			hited_task->cursor = 0;
			hited_task->timer = 0;
		}
		else //평상시
		{
			if (idle_task->cursor == idle_task->animation.size - 1)
			{
				idle_task->cursor = 0;
				idle_task->timer = 0;
			}
			enemy->enemy_list[i].sprite = showAnimation(idle_task);
		}
	}
}