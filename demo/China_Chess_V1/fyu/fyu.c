#include "fyu.h"

INT chessboard[ROW][COL] = {
	{G_GHARRY, G_HORSE, G_ELEPHANT, G_SERGEANCY, G_MARSHAL, G_SERGEANCY, G_ELEPHANT, G_HORSE, G_GHARRY},
	{G_NOP,    G_NOP,   G_NOP,      G_NOP,       G_NOP,     G_NOP,       G_NOP,      G_NOP,   G_NOP},
	{G_NOP, G_CANNON,   G_NOP,      G_NOP,       G_NOP,     G_NOP,       G_NOP,      G_CANNON,   G_NOP},
	{G_SOLDIER,G_NOP,G_SOLDIER,G_NOP,G_SOLDIER,G_NOP,G_SOLDIER,G_NOP,G_SOLDIER},
	{G_NOP,    G_NOP,   G_NOP,      G_NOP,       G_NOP,     G_NOP,       G_NOP,      G_NOP,   G_NOP},
	

	{R_NOP,    R_NOP,   R_NOP,      R_NOP,       R_NOP,     R_NOP,       R_NOP,      R_NOP,   R_NOP},
	{R_SOLDIER,R_NOP,R_SOLDIER,R_NOP,R_SOLDIER,R_NOP,R_SOLDIER,R_NOP,R_SOLDIER},
	{R_NOP, R_CANNON,   R_NOP,      R_NOP,       R_NOP,     R_NOP,       R_NOP,      R_CANNON,   R_NOP},
	{R_NOP,    R_NOP,   R_NOP,      R_NOP,       R_NOP,     R_NOP,       R_NOP,      R_NOP,   R_NOP},
	{R_GHARRY, R_HORSE, R_ELEPHANT, R_SERGEANCY, R_MARSHAL, R_SERGEANCY, R_ELEPHANT, R_HORSE, R_GHARRY},
};

void print_arry(void)
{
	INT x, y;
//	system("clear");
	for(y = 0; y < ROW; y++)
	{
		for(x = 0; x < COL; x++)
				printf("%5d", chessboard[y][x]);
		putchar('\n');
	}
}
INT save_chess(struct pos_t *src, struct pos_t *dst)
{
	INT src_chess, dst_chess, src_flag = 0, dst_flag = 0;
	src_chess = chessboard[src->y][src->x];
	dst_chess = chessboard[dst->y][dst->x];

	if (dst_chess)
	{
		if (src_chess < 8)
			src_flag = 1;
		else 
			src_flag = 0;
		if (dst_chess > 7)
			dst_flag = 1;
		else
			dst_flag =0;
	}
	if (src_flag == dst_flag)
	{
		chessboard[dst->y][dst->x] = src_chess;
		chessboard[src->y][src->x] = NOP;
		if ((dst_chess == G_MARSHAL) || (dst_chess == R_MARSHAL)) 
			return GAME_OVER;
		else
			return SUCCESS;
	}
	return NUL;
}

INT soldier(BOOL style, struct pos_t *src, struct pos_t *dst)
{
	if (style == RED)
	{
		if (dst->y > src->y)
			return NUL;
		if (src->y > ROW / 2 -1)
			if ((src->y - dst->y == 1) && (src->x == dst->x))
				return save_chess(src, dst);
			else
				return NUL;
		else
		{
			if ((src->y - dst->y == 1) && (src->x == dst->x))
				return save_chess(src, dst);
			if ((src->y == dst->y) && (abs(src->x - dst->x) == 1))
				return save_chess(src, dst);
			else
				return NUL;
		}
	}
	else
	{
		if (dst->y < src->y)
			return NUL;
		if (src->y < ROW / 2)
			if ((dst->y - src->y == 1) && (src->x == dst->x))
				return save_chess(src, dst);
			else
				return NUL;

		else
		{
			if ((dst->y - src->y == 1) && (src->x == dst->x))
				return save_chess(src, dst);
			if ((src->y == dst->y) && (abs(src->x - dst->x) == 1))
				return save_chess(src, dst);
			else
				return NUL;
				
		}
	}
	return NUL;
}

INT cannon(struct pos_t *src, struct pos_t *dst)
{
	INT min, max, x, y, count = 0;
	if (src->x == dst->x)
	{
		if (src->y == dst->y)
			return NUL;
		min = src->y > dst->y ? dst->y: src->y;
		max = src->y > dst->y ? src->y: dst->y;
		if (max == min + 1)
		{
			if (chessboard[dst->y][dst->x] != NOP)
				return NUL;
			else
				return save_chess(src, dst);
		}
		for (y = min + 1; y < max; y++)
			if (chessboard[y][src->x] != NOP)
				count++;
		if ((count == 1) && (chessboard[dst->y][dst->x] != NOP))
		{	
			return save_chess(src, dst);
		}
		if ((count == 0) && (chessboard[dst->y][dst->x] == NOP))
			return save_chess(src, dst);
	}
	if (src->y == dst->y)
	{
		min = src->x > dst->x ? dst->x: src->x;
		max = src->x > dst->x ? src->x: dst->x;
		if (max == min + 1)
		{
			if (chessboard[dst->y][dst->x] != NOP)
				return NUL;
			else
				return save_chess(src, dst);
		}
		for (x = min + 1; x < max; x++)
			if (chessboard[src->y][x] != NOP)
				count++;
		if ((count == 1) && (chessboard[dst->y][dst->x] != NOP))
		{
			return save_chess(src, dst);
		}
		if ((count == 0) && (chessboard[dst->y][dst->x] == NOP))
			return save_chess(src, dst);
	}
	return NUL;
}
INT gharry(struct pos_t *src, struct pos_t *dst)
{
	INT min, max, x, y, count = 0;
	
	if (src->x == dst->x)
	{
		if (src->y == dst->y)
			return NUL;
		min = src->y > dst->y ? dst->y: src->y;
		max = src->y > dst->y ? src->y: dst->y;
		for (y = min + 1; y < max; y++)
			if (chessboard[y][src->x] !=NOP)
				count++;
		if (count == 0)  
			return save_chess(src, dst);
	}
	if (src->y == dst->y)
	{
		min = src->x > dst->x ? dst->x: src->x;
		max = src->x > dst->x ? src->x: dst->x;
		for (x = min + 1; x < max; x++)
			if (chessboard[src->y][x] !=NOP)
				count++;
		if (count == 0)  
			return save_chess(src, dst);
	}
	return NUL;
}

INT horse(struct pos_t *src, struct pos_t *dst)
{
	// |x1 -x0| ==1 && |y1 -y0| == 2
	// |x1 -x0| ==2 && |y1 -y0| ==1
	if (((abs(dst->x - src->x) == 1) && (abs(dst->y - src->y) == 2)) || ((abs(dst->x - src->x) == 2) && (abs(dst->y - src->y) == 1)))
	{
		//x0 + (x1 - x0 +1)%2, y0 + (y1 - y0 +1)%2
		if (!(chessboard[src->y + (dst->y - src->y + 1) % 2][src->x + (dst->x - src->x + 1) % 2]))
			return save_chess(src, dst);
	}
	return NUL;
}

INT elephant(INT style, struct pos_t *src, struct pos_t *dst)
{
	if (style == GREEN)
	{
		if (dst->y > ROW / 2 - 1) 
			return NUL;
	}
	else
	{
		if (dst->y < ROW / 2)
			return NUL;
	}
	if ((abs(dst->x - src->x) == 2) && (abs(dst->y - src->y) == 2))
	{
		if (!(chessboard[(dst->y + src->y) / 2][(dst->x + src->x) / 2]))
			return save_chess(src, dst);
	}
	return NUL;
}

INT sergeancy(INT style, struct pos_t *src, struct pos_t *dst)
{
	if (style == GREEN)
	{
		if ((dst->x > (COL + 1) / 2) || (dst->x < COL / 2 - 1) || (dst->y > 2))
			return NUL;
		if ((abs(dst->x - src->x) == 1) && (abs(dst->y -src->y) == 1))
			return save_chess(src, dst);
	}
	if (style == RED)
	{
		if ((dst->x > (COL + 1) / 2) || (dst->x < COL / 2 - 1) || (dst->y < ROW - 3))
			return NUL;
		if ((abs(dst->x - src->x) == 1) && (abs(dst->y -src->y) == 1))
			return save_chess(src, dst);
	}
	return NUL;
}
INT marshal(INT style, struct pos_t *src, struct pos_t *dst)
{
	INT count = 0, y;
	if (style == GREEN)
	{
		if (chessboard[dst->y][dst->x] == R_MARSHAL)
		{
			for (y = src->y + 1; y < dst->y; y++)
				if(chessboard[y][src->x])
					count++;
			if (count == 0)
				return save_chess(src, dst);
			else
				return NUL;
		}
		if ((dst->x > (COL + 1) / 2) || (dst->x < COL / 2 - 1) || (dst->y > 2))
			return NUL;
		if (((abs(dst->x - src->x) == 1) && (abs(dst->y -src->y) == 0)) || ((abs(dst->x - src->x) == 0) && (abs(dst->y -src->y) == 1)))
			return save_chess(src, dst);
	}
	if (style == RED)
	{
		if (chessboard[dst->y][dst->x] == G_MARSHAL)
		{
			for (y = dst->y + 1; y < src->y; y++)
				if(chessboard[y][src->x])
					count++;
			if (count == 0)
				return save_chess(src, dst);
			else
				return NUL;
		}
		if ((dst->x > (COL + 1) / 2) || (dst->x < COL / 2 - 1) || (dst->y < ROW - 3))
			return NUL;
		if (((abs(dst->x - src->x) == 1) && (abs(dst->y -src->y) == 0)) || ((abs(dst->x - src->x) == 0) && (abs(dst->y -src->y) == 1)))
			return save_chess(src, dst);
	}
	return NUL;
}

INT analysis(struct st_chess *chess)
{
	switch(chess->name)
	{
		case G_SOLDIER:
			return soldier(GREEN, &chess->src_pos, &chess->dst_pos);
		case R_SOLDIER:
			return soldier(RED, &chess->src_pos, &chess->dst_pos);
		case G_CANNON:
		case R_CANNON:
			return cannon(&chess->src_pos, &chess->dst_pos);
		case G_GHARRY:
		case R_GHARRY:
			return gharry(&chess->src_pos, &chess->dst_pos);
		case G_HORSE:
		case R_HORSE:
			return horse(&chess->src_pos, &chess->dst_pos);
		case G_ELEPHANT:
			return elephant(GREEN, &chess->src_pos, &chess->dst_pos);
		case R_ELEPHANT:
			return elephant(RED, &chess->src_pos, &chess->dst_pos);
		case G_SERGEANCY:
			return sergeancy(GREEN, &chess->src_pos, &chess->dst_pos);
		case R_SERGEANCY:
			return sergeancy(RED, &chess->src_pos, &chess->dst_pos);
		case G_MARSHAL:
			return marshal(GREEN, &chess->src_pos, &chess->dst_pos);
		case R_MARSHAL:
			return marshal(RED, &chess->src_pos, &chess->dst_pos);
	}
	return NUL;
}

