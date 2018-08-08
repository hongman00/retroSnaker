
void snake_setSideLength(unsigned int len) {
	m_sideLength = len + 2;

	m_interval = 400 / (len * len / 2);

	m_strLen = m_sideLength * (m_sideLength + 1) * 2 + 128;
	m_mapStr = (char *)calloc(m_strLen, sizeof(char));
}

void snake_drawMap() {
	int i = 0;
	int j = 0;
	char *temp = m_mapStr;
	unsigned int len = m_strLen;

	sprintf_s(temp, len, "%s", "          【RETROSNAKER】\n");
	temp += strlen("          【RETROSNAKER】\n");
	len -= strlen("          【RETROSNAKER】\n");
	for (i = 0; i < m_sideLength; ++i) {
		for (j = 0; j < m_sideLength; ++j) {
			if (i > 0 && i < m_sideLength - 1 && j > 0 && j < m_sideLength - 1) {
				if (true == m_snakeMap[i][j].isFood) {
					sprintf_s(temp, len, "%s", " *");
				}
				else if (true == m_snakeMap[i][j].isSnake) {
					sprintf_s(temp, len, "%s", " O");
				}
				else if (true == m_snakeMap[i][j].isHead) {
					sprintf_s(temp, len, "%s", " @");
				}
				else {
					sprintf_s(temp, len, "%s", "  ");
				}
			}
			else {
				if (true == m_snakeMap[i][j].isHead) {
					sprintf_s(temp, len, "%s", " @");
				}
				else {
					sprintf_s(temp, len, "%s", " .");
				}
			}
			temp += 2;
			len -= 2;
		}
		sprintf_s(temp, len, "%s", "\n");
		temp += 1;
		len -= 1;
	}
	if (false == m_isWin && false == m_isDead && SNAKE_PAUSE != m_moveDir) {
		sprintf_s(temp, len, "%s", "      【↑ ↓ ← → p:pause】\n");
	}
	printf("%s", m_mapStr);
}

unsigned int snake_randNum() {
	return (rand() % (m_sideLength - 2)) + 1;
}

void snake_setFood() {
	int i = 0;
	int j = 0;

	i = snake_randNum();
	j = snake_randNum();

	while (true == m_snakeMap[i][j].isSnake || true == m_snakeMap[i][j].isHead) {
		i = snake_randNum();
		j = snake_randNum();
	}

	m_snakeMap[i][j].isFood = true;

	if (m_duration > 100) {
		m_duration -= m_interval;
	}
}

void snake_add(Point p) {
	m_snakeHead->pLeft = (Node *)calloc(1, sizeof(Node));
	m_snakeHead->pLeft->p = p;
	m_snakeHead->pLeft->pLeft = NULL;
	m_snakeHead->pLeft->pRight = m_snakeHead;
	m_snakeHead = m_snakeHead->pLeft;
}

void snake_remove() {
	m_snakeMap[m_snakeTail->p.y][m_snakeTail->p.x].isSnake = false;
	m_snakeTail = m_snakeTail->pLeft;
	free(m_snakeTail->pRight);
	m_snakeTail->pRight = NULL;
}

int snake_doMove(Point p) {
	m_snakeMap[m_snakeHead->p.y][m_snakeHead->p.x].isHead = false;
	m_snakeMap[m_snakeHead->p.y][m_snakeHead->p.x].isSnake = true;

	snake_add(p);

	if (true == m_snakeMap[p.y][p.x].isFood) {
		m_snakeMap[p.y][p.x].isFood = false;
		m_snakeMap[p.y][p.x].isHead = true;

		if ((m_sideLength - 2) * (m_sideLength - 2) == ++m_eatCnt) {
			return 2;
		}
		snake_setFood();
	}
	else {
		snake_remove();
		if (true == m_snakeMap[p.y][p.x].isSnake) {
			m_snakeMap[p.y][p.x].isHead = true;
			m_snakeMap[p.y][p.x].isSnake = false;
			return 0;
		}
	}
	m_snakeMap[p.y][p.x].isHead = true;

	return 1;
}

int snake_moveUp() {
	Point p;
	int ret = -1;

	p.x = m_snakeHead->p.x;
	p.y = m_snakeHead->p.y - 1;
	
	ret = snake_doMove(p);

	if (0 == p.y) {
		return 0;
	}

	return ret;
}

int snake_moveDown() {
	Point p;
	int ret = -1;

	p.x = m_snakeHead->p.x;
	p.y = m_snakeHead->p.y + 1;

	ret = snake_doMove(p);

	if (m_sideLength - 1 == p.y) {
		return 0;
	}

	return ret;
}

int snake_moveLeft() {
	Point p;
	int ret = -1;

	p.x = m_snakeHead->p.x - 1;
	p.y = m_snakeHead->p.y;
	
	ret = snake_doMove(p);

	if (0 == p.x) {
		return 0;
	}

	return ret;
}

int snake_moveRight() {
	Point p;
	int ret = -1;

	p.x = m_snakeHead->p.x + 1;
	p.y = m_snakeHead->p.y;
	
	ret = snake_doMove(p);

	if (m_sideLength - 1 == p.x) {
		return 0;
	}

	return ret;
}

void snake_move(void *) {
	int ret = -1;
	unsigned int dir = 0;

	while (true) {
		dir = m_moveDir;
		dir = dir == SNAKE_UP ? SNAKE_DOWN == m_lastMoveDir ? SNAKE_DOWN : SNAKE_UP : dir == SNAKE_DOWN ? SNAKE_UP == m_lastMoveDir ? SNAKE_UP : SNAKE_DOWN : dir == SNAKE_LEFT ? SNAKE_RIGHT == m_lastMoveDir ? SNAKE_RIGHT : SNAKE_LEFT : dir == SNAKE_RIGHT ? SNAKE_LEFT == m_lastMoveDir ? SNAKE_LEFT : SNAKE_RIGHT : SNAKE_PAUSE;
		if (SNAKE_PAUSE == dir) {
			continue;
		}

		m_lastMoveDir = dir;
		switch (dir) {
		case SNAKE_UP:
			ret = snake_moveUp();
			break;
		case SNAKE_DOWN:
			ret = snake_moveDown();
			break;
		case SNAKE_LEFT:
			ret = snake_moveLeft();
			break; 
		case SNAKE_RIGHT:
			ret = snake_moveRight();
			break;
		default:
			continue;
		}
		m_isStart = true;

		if (0 == ret) {
			m_isDead = true;
			system("cls");
			snake_drawMap();
			snake_destroy();
			printf("              【DEAD】\n         r:restart|q:quit");
			return;
		}
		else if (2 == ret) {
			m_isWin = true;
			system("cls");
			snake_drawMap();
			snake_destroy();
			printf("              【WIN】\n         r:restart|q:quit");
			return;
		}
		else {
			system("cls");
			snake_drawMap();
			Sleep(m_duration);
		}
	}
}

void snake_destroy() {
	int i = 0;

	if (NULL == m_snakeHead) {
		return;
	}

	while (m_snakeHead->pRight) {
		m_snakeHead = m_snakeHead->pRight;
		free(m_snakeHead->pLeft);
		m_snakeHead->pLeft = NULL;
	}
	free(m_snakeHead);
	m_snakeHead = NULL;

	for (i = 0; i < m_sideLength; ++i) {
		free(m_snakeMap[i]);
		m_snakeMap[i] = NULL;
	}
	free(m_snakeMap);
	m_snakeMap = NULL;

	free(m_mapStr);
	m_mapStr = NULL;
}

void snake_init() {
	int i = 0;
	int j = 0;
	Node *pNode = NULL;

	m_duration = 500;

	m_snakeMap = (snake_node **)calloc(m_sideLength, sizeof(snake_node *));
	for (i = 0; i < m_sideLength; ++i) {
		m_snakeMap[i] = (snake_node *)calloc(m_sideLength, sizeof(snake_node));
	}

	for (i = 0; i < m_sideLength; ++i) {
		for (j = 0; j < m_sideLength; ++j) {
			m_snakeMap[i][j].isFood = false;
			m_snakeMap[i][j].isSnake = false;
			m_snakeMap[i][j].isHead = false;
		}
	}

	i = snake_randNum();
	j = snake_randNum();
	m_snakeMap[j][i].isHead = true;
	m_snakeTail = m_snakeHead = (Node *)calloc(1, sizeof(Node));
	m_snakeTail->p.x = i;
	m_snakeTail->p.y = j;
	m_snakeTail->pLeft = NULL;
	m_snakeTail->pRight = NULL;

	snake_setFood();

	snake_drawMap();

	_beginthread(snake_move, 0, NULL);
}

char snake_getMoveDir() {
	int c;

	while (true) {
		c = _getch();
		if (SNAKE_PAUSE == c) {
			if (true == m_isStart) {
				m_moveDir = SNAKE_PAUSE;
				system("cls");
				snake_drawMap();
				printf("              【PAUSE】\n");
			}
		}
		else if (SNAKE_REPEAT == c) {
			if (true == m_isDead || true == m_isWin) {
				m_isDead = false;
				m_isStart = false;
				m_moveDir = 0;
				m_lastMoveDir = 0;
				m_eatCnt = 1;
				system("cls");
				return 'r';
			}
		}
		else if (SNAKE_QUIT == c) {
			if (true == m_isDead || true == m_isWin) {
				return 'q';
			}
		}
		else if (SNAKE_SPEC == c){
			c = _getch();
			switch (c) {
			case SNAKE_UP:
			case SNAKE_DOWN:
			case SNAKE_LEFT:
			case SNAKE_RIGHT:
				break;
			default:
				continue;
			}
			m_moveDir = (unsigned int)c;
			Sleep(10);
		}
	}
}