#pragma once

#define UI_TEXT 101
#define UI_BACKGROUND 102
#define UI_BUTTON 103

#define ALIGN_CENTER 0
#define ALIGN_BOTTOM 1

#define RELETIVE_TOP_OF 0
#define RELETIVE_BOTTOM_OF 1
#define RELETIVE_START_OF 2
#define RELETIVE_END_OF 3

#define MARGIN_TOP 0
#define MARGIN_BOTTOM 1
#define MARGIN_START 2
#define MARGIN_END 3

typedef struct TEXTVIEW
{
	COLIDER colider;
	POINT point;
	char* text;
	COLORREF color;
} TEXTVIEW;

typedef struct BACKGROUND
{
	int image;
} BACKGROUND;

typedef struct BUTTON
{
	COLIDER colider;
	POINT point;
	int image;
	int alpha;
} BUTTON;

typedef struct UI_OBJECT
{
	int type;
	TEXTVIEW text;
	BACKGROUND background;
	BUTTON button;
} UI_OBJECT;

UI_OBJECT createTextView(POINT point, char* text, COLORREF color)
{
	COLIDER colider = {0,0,0,0};
	UI_OBJECT temp = { UI_TEXT };
	TEXTVIEW tv = { colider,point,text,color };
	temp.text = tv;
	return temp;
}

UI_OBJECT createBackground(int image)
{
	UI_OBJECT temp = { UI_BACKGROUND };
	BACKGROUND bg = { image };
	temp.background = bg;
	return temp;
}

UI_OBJECT createButton(COLIDER colider, POINT point, int image, int alpha)
{
	UI_OBJECT temp = { UI_BUTTON };
	BUTTON btn = { colider, point, image, alpha };
	temp.button = btn;
	return temp;
}

POINT getAlignPoint(int type, POINT b_size, POINT s_size)
{
	POINT pos = { 0, 0 };
	switch (type)
	{
		case ALIGN_CENTER:
			pos.x = (s_size.x / 2) - (b_size.x / 2);
			pos.y = (s_size.y / 2) - (b_size.y / 2);
			break;
		case ALIGN_BOTTOM:
			break;
	}
	return pos;
}

POINT getMarinPoint(int type, POINT pos,int val)
{
	switch (type)
	{
	case MARGIN_TOP:
		pos.y += val;
		break;
	case MARGIN_BOTTOM:
		pos.y -= val;
		break;
	}
	return pos;
}

int isClicked(POINT cursor, COLIDER colider)
{
	if (BETWEEN(colider.start_x, cursor.x, colider.end_x) && BETWEEN(colider.start_y, cursor.y, colider.end_y))
		return 1;
	else
		return 0;
}