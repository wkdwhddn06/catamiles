#pragma once

ANIMATION createAnimation(int args, ...)
{
	ANIMATION animation;
	va_list ap;
	
	va_start(ap, args);

	for (int i = 0; i < args; i++)    // 가변 인자 개수만큼 반복
	{
		int num = va_arg(ap, int);    // int 크기만큼 가변 인자 목록 포인터에서 값을 가져옴
									  // ap를 int 크기만큼 순방향으로 이동
		animation.frames[i] = num;    // 가변 인자 값 출력
	}
	va_end(ap);

	animation.size = args;
	return animation;
}

ANIMATION_TASK createAnimationTask(ANIMATION ani, int length)
{
	ANIMATION_TASK task;
	task.animation = ani;
	task.cursor = 0;
	task.length = length;
	task.timer = 0;
	return task;
}

int isAnimationStarted(ANIMATION_TASK task)
{
	if (task.timer != 0)
		return 1;
	else if (task.cursor != 0)
		return 1;

	return 0;
}

int showAnimation(ANIMATION_TASK * task)
{
	if (task->cursor < task->animation.size-1)
	{
		if (task->timer == task->length)
		{
			task->cursor += 1;
			task->timer = 0;
		}
		task->timer += 1;
		return task->animation.frames[task->cursor];
	}
	else
		return 1;
}