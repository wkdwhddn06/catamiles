#pragma once

typedef struct CAMERA_SHAKE
{
	int length;
	int cursor;
	int power;
	POINT first;
}CAMERA_SHAKE;


typedef struct CAMERA
{
	POINT anchor;
	POINT def;
	POINT margin;
	CAMERA_SHAKE shake;
}CAMERA;

CAMERA_SHAKE createCameraShake(int leng)
{
	CAMERA_SHAKE tmp;
	tmp.length = leng;
	tmp.cursor = 0;
	return tmp;
}

CAMERA createCamera(POINT center)
{
	CAMERA tmp;
	tmp.anchor = center;
	tmp.shake = createCameraShake(0);
	return tmp;
}

void attachToCamera(CAMERA * camera, GAME_OBJECT* player)
{
	camera->def = player->point;
	POINT tmp = { camera->anchor.x - camera->def.x, camera->anchor.y - camera->def.y };
	player->point = tmp;
}

void shakeCamera(CAMERA* camera, int leng, int power)
{
	CAMERA_SHAKE shake;
	shake.cursor = 0;
	shake.first = camera->def;
	shake.length = leng;
	shake.power = power;
	camera->shake = shake;
}

void doCameraTask(CAMERA* camera)
{
	int a = 0;

	if (camera->shake.length != 0)
	{
		if (camera->shake.length == camera->shake.cursor)
		{
			camera->shake.cursor = 0;
			camera->shake.length = 0;
		}
		else
		{
			int x_ran = random(-camera->shake.power, camera->shake.power);
			int y_ran = random(-camera->shake.power, camera->shake.power);
			camera->margin.x += x_ran;
			camera->margin.y += y_ran;
			camera->shake.cursor += 1;
		}
	}
}