#pragma once

//게임의 토대가 되는 물리, 충돌등의 시스템을 담당하는 헤더입니다

//사이에 있으면 TRUE 반환
#define BETWEEN(a,b,c) ((a <= b)&&(b <= c))

//충돌처리 방향
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

//충돌 정보를 전달하기위한 구조체입니다
typedef struct COLIDE_INFO
{
	int isColide;
	BULLET bullet;
	int tag;
} COLIDE_INFO;

//콜라이더와 함께 오브젝트를 이동시킵니다(오류 존재함)
void MoveWithColider(GAME_OBJECT* obj, POINT v)
{
	obj->point.x += v.x;
	obj->point.y += v.y;

	obj->colider.end_x += v.x;
	obj->colider.end_y += v.y;
	obj->colider.start_x += v.x;
	obj->colider.start_y += v.y;
}

int isColide(COLIDER a, COLIDER b)
{
	const int isInSameX = (BETWEEN(a.start_x, b.start_x, a.end_x) || BETWEEN(a.start_x, b.end_x, a.end_x));
	const int isInSameY = (BETWEEN(a.start_y, b.start_y, a.end_y) || BETWEEN(a.start_y, b.end_y, a.end_y));

	if (isInSameX)
	{
		if (BETWEEN(a.start_y, b.start_y, a.end_y))
			return 0;
		else if (BETWEEN(a.start_y, b.end_y, a.end_y))
			return 0;
	}

	else if (isInSameY)
	{
		if (BETWEEN(a.start_y, b.start_y, a.end_y))
			return 0;
		else if (BETWEEN(a.start_y, b.end_y, a.end_y))
			return 0;
	}
	else
		return 1;
}

//충돌 처리 함수들 입니다
int isColideForBullet(COLIDER player, ArrayList * pool, ArrayList * enemy)
{
	for (int i = 0; i < enemy->size; i++)
	{
		COLIDER colider = enemy->enemy_list[i].colider;
		const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
		const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

		if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
		{
			return 0;
		}
		else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
		{
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
		{
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
		{
			return 0;
		}
	}

	return 1;
}

int isColideWithMoving(GAME_OBJECT * player, POINT nextMove, ArrayList* coliders)
{
	for (int i = 0; i < coliders->size; i++)
	{
		COLIDER colider = coliders->colider_list[i];
		COLIDER player_col = addPointToColider(player->colider, nextMove);
		const int isInSameX = (BETWEEN(colider.start_x, player_col.start_x, colider.end_x) || BETWEEN(colider.start_x, player_col.end_x, colider.end_x));
		const int isInSameY = (BETWEEN(colider.start_y, player_col.start_y, colider.end_y) || BETWEEN(colider.start_y, player_col.end_y, colider.end_y));

		if (isInSameX && BETWEEN(colider.start_y, player_col.start_y, colider.end_y))
		{
			//왼쪽 면에 닿았을때
			return 0;
		}
	    else if (isInSameX && BETWEEN(colider.start_y, player_col.end_y, colider.end_y))
		{
			//아래쪽 면에 닿았을떄
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player_col.start_x, colider.end_x))
		{
			//위쪽 면에 닿았을때
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player_col.end_x, colider.end_x))
		{
			//오른쪽 면에 닿았을때
			return 0;
		}
	}

	return 1;
}

int isColideWithColiders(COLIDER player, ArrayList* coliders)
{
	for (int i = 0; i < coliders->size; i++)
	{
		COLIDER colider = coliders->colider_list[i];
		const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
		const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

		if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
		{
			return 0;
		}
		else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
		{
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
		{
			return 0;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
		{
			return 0;
		}
	}

	return 1;
}

int isColideForMath(COLIDER player, COLIDER colider)
{
	const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
	const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

	if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
	{
		return 0;
	}
	else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
	{
		return 0;
	}
	else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
	{
		return 0;
	}
	else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
	{
		return 0;
	}

	return 1;
}

//적을 위한 충돌 처리 함수입니다
COLIDE_INFO isColideForEnemy(COLIDER colider, ArrayList* pool)
{
	COLIDE_INFO info;
	for (int i = 0; i < pool->size; i++)
	{
		if (pool->bullet_list[i].tag == TAG_BULLET)
		{
			COLIDER player = pool->bullet_list[i].colider;
			const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
			const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

			if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
			{
				info.isColide = 1;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
			{
				info.isColide = 2;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
			{
				info.isColide = 3;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
			{
				info.isColide = 4;
				info.bullet = pool->bullet_list[i];
				return info;
			}
		}
	}
	info.isColide = 0;
	return info;
}

//플레이어를 위한 충돌 처리 함수입니다
COLIDE_INFO isColideForPlayerAndBullet(COLIDER colider, ArrayList* pool, ArrayList* enemy)
{
	COLIDE_INFO info;
	for (int i = 0; i < pool->size; i++)
	{
		if (pool->bullet_list[i].tag == TAG_ENEMY_BULLET)
		{
			COLIDER player = pool->bullet_list[i].colider;
			const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
			const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

			if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
			{
				info.isColide = 1;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
			{
				info.isColide = 2;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
			{
				info.isColide = 3;
				info.bullet = pool->bullet_list[i];
				return info;
			}
			else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
			{
				info.isColide = 4;
				info.bullet = pool->bullet_list[i];
				return info;
			}
		}
	}

	for (int i = 0; i < enemy->size; i++)
	{
		COLIDER player = enemy->enemy_list[i].colider;
		const int isInSameX = (BETWEEN(colider.start_x, player.start_x, colider.end_x) || BETWEEN(colider.start_x, player.end_x, colider.end_x));
		const int isInSameY = (BETWEEN(colider.start_y, player.start_y, colider.end_y) || BETWEEN(colider.start_y, player.end_y, colider.end_y));

		if (isInSameX && BETWEEN(colider.start_y, player.start_y, colider.end_y))
		{
			info.isColide = 1;
			return info;
		}
		else if (isInSameX && BETWEEN(colider.start_y, player.end_y, colider.end_y))
		{
			info.isColide = 2;
			return info;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.start_x, colider.end_x))
		{
			info.isColide = 3;
			return info;
		}
		else if (isInSameY && BETWEEN(colider.start_x, player.end_x, colider.end_x))
		{
			info.isColide = 4;
			return info;
		}
	}
	info.isColide = 0;
	return info;
}

//선형보간을 사용하여 오브젝트를 이동시킵니다
void Lerp(GAME_OBJECT* target, POINT destination,int speed)
{
	LONG x = target->point.x;
	LONG y = target->point.y;

	LONG dx = destination.x;
	LONG dy = destination.y;

	target->point.x += (dx - x) * (0.016 * speed);
	target->point.y += (dy - y) * (0.016 * speed);
}

//선형보간 값을 구하는 함수 입니다
POINT LerpForMath(POINT start, POINT destination, int speed)
{
	LONG x = start.x;
	LONG y = start.y;

	LONG dx = destination.x;
	LONG dy = destination.y;

	POINT tmp;
	tmp.x = (dx - x) * (speed);
	tmp.y = (dy - y) * (speed);

	return tmp;
}

//선형보간을 사용하여 포인트를 이동시킵니다
POINT LerpPoint(POINT target, POINT destination, int speed)
{
	POINT tmp = target;

	LONG x = target.x;
	LONG y = target.y;

	LONG dx = destination.x;
	LONG dy = destination.y;

	tmp.x += (dx - x) * (0.016 * speed);
	tmp.y += (dy - y) * (0.016 * speed);

	return tmp;
}

//적용 선형보간입니다
void LerpForEnemy(ENEMY* target, POINT destination, int speed)
{
	LONG x = target->point.x;
	LONG y = target->point.y;

	LONG dx = destination.x;
	LONG dy = destination.y;

	target->point.x += (dx - x) * (0.016 * speed);
	target->point.y += (dy - y) * (0.016 * speed);
}

//일반적인 이동 함수 입니다
POINT MoveForward(POINT target, POINT d, int speed)
{
	LONG x = target.x;
	LONG y = target.y;	

	POINT res;
	POINT direction;
	direction.x = (d.x - x);
	direction.y = (d.y - y);

	double length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

	double movement_direction_normalized_x = (double)direction.x / length;
	double movement_direction_normalized_y = (double)direction.y / length;

	res.x = movement_direction_normalized_x * (0.016 * 100 * speed);
	res.y = movement_direction_normalized_y * (0.016 * 100 * speed);
	return res;
}

//VECTOR 구조체를 사용하여 정확도를 향상시킨 이동 함수 입니다
VECTOR MoveForwardVector(POINT target, POINT d, int speed)
{
	LONG x = target.x;
	LONG y = target.y;

	VECTOR res;
	VECTOR direction;
	direction.x = (d.x - x);
	direction.y = (d.y - y);

	double length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

	double movement_direction_normalized_x = (double)direction.x / length;
	double movement_direction_normalized_y = (double)direction.y / length;

	res.x = movement_direction_normalized_x * (0.016 * 100 * speed);
	res.y = movement_direction_normalized_y * (0.016 * 100 * speed);
	return res;
}

//콜라이더와 함께 이동하는 함수 입니다
void TransformWithColider(GAME_OBJECT* obj, POINT v)
{
	POINT move;
	move = subtractPoint(v, obj->point);

	obj->point = v;

	obj->colider.end_x += move.x;
	obj->colider.end_y += move.y;
	obj->colider.start_x += move.x;
	obj->colider.start_y += move.y;
}

//특정위치로 이동하기 위한 첫번째 이동 값을 구합니다
POINT getFirstStep(POINT target, POINT destination, int speed)
{
	//speed += 50;
	POINT tmp;
	tmp.x = (destination.x - target.x)/ (100 - speed);
	tmp.y = (destination.y - target.y)/ (100 - speed);

	return tmp;
}

//Bresenham알고리즘을 활용하여 특정위치로 완벽하게 이동합니다
//하지만 이동하는 모습이 예쁘지 않아 사용하지 않습니다
POINT getFirstStepBresenham(BRESENHAM * data)
{
	POINT tmp;
	
	tmp.x = 2;
	tmp.y = 0;

	// 중단점이 0보다 작으면 그에 맞는 공식으로 판별식 갱신, y값은 그대로
	if (data->F < 0)
	{
		data->F += data->dF1;
	}
	else // 중단점이 0보다 크거나 같으면 그에 맞는 공식으로 판별식 갱신, y값은 증가
	{
		tmp.y = 2;
		data->F += data->dF2;
	}

	return tmp;
}

//점과 점사이의 거리를 구하는 함수입니다
int getDistance(POINT a, POINT b) 
{
	POINT tmp;
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	return sqrt(pow(tmp.x, 2) + pow(tmp.y, 2));
}