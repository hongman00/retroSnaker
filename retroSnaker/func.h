#pragma once
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include <conio.h>

#define SNAKE_UP		72
#define SNAKE_DOWN		80
#define SNAKE_LEFT		75
#define SNAKE_RIGHT		77
#define SNAKE_PAUSE		112
#define SNAKE_QUIT		113
#define SNAKE_REPEAT	114
#define SNAKE_SPEC		224

typedef struct tagSnake_node {
	bool isFood;
	bool isSnake;
	bool isHead;
}snake_node;

typedef struct tagPoint {
	unsigned int x;
	unsigned int y;
}Point;

typedef struct tagNode {
	Point p;
	struct tagNode *pLeft;
	struct tagNode *pRight;
}Node;

snake_node **m_snakeMap = NULL;
Node *m_snakeHead = NULL;
Node *m_snakeTail = NULL;
unsigned int m_moveDir = 0;
unsigned int m_lastMoveDir = 0;
unsigned int m_sideLength = 0;
bool m_isDead = false;
bool m_isWin = false;
bool m_isStart = false;
unsigned int m_duration = 500;
unsigned int m_interval = 0;
char *m_mapStr = NULL;
unsigned int m_strLen = 0;
unsigned int m_eatCnt = 1;

void snake_setSideLength(unsigned int len);

void snake_drawMap();

unsigned int snake_randNum();

void snake_setFood();

void snake_add(Point p);

void snake_remove();

int snake_doMove(Point p);

int snake_moveUp();

int snake_moveDown();

int snake_moveLeft();

int snake_moveRight();

void snake_move(void *);

void snake_init();

void snake_destroy();

char snake_getMoveDir();



