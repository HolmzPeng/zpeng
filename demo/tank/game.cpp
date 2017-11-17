#include "public.h"
/* ------------------main loop----------------- */
void
Play_Game (GameEngine * ge)
{
	int i, j, k;
	int randnum;
	int object;
	int temp;

	static int dx[4] = { 0, -1, 0, 1 };
	static int dy[4] = { -1, 0, 1, 0 };
	int x, y;
	SDL_Rect res_rect, des_rect;

	ge->clock = 0;
	Disp_TankLeft (ge);			//显示剩下的坦克
	BorePlayer (ge, &player[0],0);  //产生player[0] 及palyer[1]
	if(players==1)				//double players
		BorePlayer (ge, &player[1],1);
	ge->last_time = SDL_GetTicks ();
	while (game_state == GAME_RUNNING || game_state == GAME_WIN
	        || game_state == GAME_OVER)
	{

		/* Get Input */
		SDL_PollEvent (&(ge->event));
		if (key_state.Quit)
			game_state = GAME_EXIT;

		/* Process player's tank */
		for (i = 0; i < 2; i++)
		{
			//if(game_state==GAME_OVER)continue;
			if (!player[i].active)		//如果死亡跳过
				continue;
			if (ge->player_left[i] < 0)      //如果没有left跳过
				continue;
			/* fire! */
			if (key_state.Fire[i])             //fire key
				PlayerFire (ge, &player[i], i);

			if (player[i].boring)               //正在产生坦克
			{
				player[i].clock++;
				if (player[i].clock % 20 < 10)
				{
					DrawSprite (ge, &player[i]);           //使坦克闪烁
				}
				else
				{
					ClearSprite (ge, &player[i]);
				}
				if (player[i].clock > 60)                  //结束闪烁
				{
					player[i].clock = 0;
					player[i].boring = 0;
				}
				continue;
			}

			/* Do game logic */
			//if(player[i].active!=1 && ge->player_left>=0)BorePlayer(ge,&player[i]);
			//Move !!!!
			if (key_state.Move[i])           //如果有移动键!!则移动
			{
				x = player[i].x + player[i].speed * dx[player[i].dir];
				y = player[i].y + player[i].speed * dy[player[i].dir];
				player[i].ox = player[i].x;
				player[i].oy = player[i].y;
				if (MoveAble (&player[i], x, y))
				{
					player[i].x = x;
					player[i].y = y;
					if (!PlaceAble (&player[i]))
					{
						player[i].x = player[i].ox;
						player[i].y = player[i].oy;
					}
				}
				player[i].timer1++;
				player[i].timer1 %= 4;
			}
			else
				player[i].timer1 = 0;
			ClearSprite (ge, &player[i]);                    //刷新坦克
			DrawSprite (ge, &player[i]);
		}

		/* Process player's bullet */                        //我军炮弹 i<4 !!!
		for (i = 0; i < 4; i++)
		{
			if (bullet[i].active)
			{
				bullet[i].ox = bullet[i].x;
				bullet[i].oy = bullet[i].y;
				x = bullet[i].x + bullet[i].speed * dx[bullet[i].dir];
				y = bullet[i].y + bullet[i].speed * dy[bullet[i].dir];
				object = HitObject (&bullet[i], x, y);
				if (object == 0)
				{
					bullet[i].x = x;
					bullet[i].y = y;
					ClearSprite (ge, &bullet[i]);
					DrawSprite (ge, &bullet[i]);
				}
				/* Out off screen */
				else if (object == -1)
				{
					ClearSprite (ge, &bullet[i]);
					bullet[i].active = 0;
				}
				/* Hit the hawk! */
				else if (object == 8)
				{
					Burst (ge, &bullet[i]);
					ExplodeHawk (ge);
					PlaySound (Snd_bom);
					printf ("Game over!\n");
					game_state = GAME_OVER;
				}

				/* Hit the wall */
				else
				{
					ClearSprite (ge, &bullet[i]);                     //击中木墙
					bullet[i].active = 0;
					if (object == 1)
					{
						HitTile (ge, &bullet[i], x, y);
						PlaySound (Snd_dow);
					}
					if (object == 9)                                        //击中钢墙
						PlaySound (Snd_ding);
				}
				for (j = 0; j < ge->max_enemys + 2; j++) 	//击中敌人
				{
					if (enemy[j].active)
					{
						/* Hit the enemy! */
						if (HitTest (&bullet[i], &enemy[j]) && !enemy[j].boring)
						{
							enemy[j].armor--;
							if (enemy[j].armor >= 0)
							{
								Burst (ge, &bullet[i]);
								PlaySound (Snd_ding);
							}
							else
							{
								Burst (ge, &bullet[i]);                                           //爆炸
								Burst (ge, &enemy[j]);
								Explode (ge, &enemy[j]);
								PlaySound (Snd_bom);
								/* player=i/2 */
								Add_Score (ge, i, &enemy[j]);

								ge->enemy_left--;
								if (ge->enemy_left < 1)
								{
									printf ("Round clear!\n");
									game_state = GAME_WIN;
								}
							}
						}
					}
					if (bullet[j + 4].active == 1)
					{
						/* Hit the enemy's bullet! */
						if (HitTest (&bullet[i], &bullet[j + 4]))
						{
							Burst (ge, &bullet[i]);
							Burst (ge, &bullet[j + 4]);
						}
					}
				}
			}
		}
		//--------------------------------------------------------------------------------------

		/* Process enemys */		//处理敌人
		for (i = 0; i < ge->max_enemys; i++)
		{
			if (enemy[i].active != 1)
				continue;
			/* Flash on boring */
			if (enemy[i].boring)
			{
				enemy[i].clock++;
				if (enemy[i].clock % 20 < 10)
				{
					DrawSprite (ge, &enemy[i]);
				}
				else
				{
					ClearSprite (ge, &enemy[i]);
				}
				if (enemy[i].clock > 60)
				{
					enemy[i].clock = 0;
					enemy[i].boring = 0;
				}
				continue;
			}

			x = enemy[i].x + enemy[i].speed * dx[enemy[i].dir];
			y = enemy[i].y + enemy[i].speed * dy[enemy[i].dir];
			enemy[i].ox = enemy[i].x;
			enemy[i].oy = enemy[i].y;
			enemy[i].timer1++;
			enemy[i].timer1 %= 4;

			/*
			   for(j=0;j<MAX_ENEMYS;j++){
			   // Two enemy tank touch each onther then change directtion
			   if(enemy[j].active!=1)continue;
			   if(i==j)continue;
			   if(InterSect(x,y,enemy[j].x,enemy[j].y)){
			   randnum=(int)(100.0*rand()/(RAND_MAX+1.0));
			   if(randnum<5 || randnum>95){
			   ClearSprite(ge,&enemy[i]);
			   ChangeDirection(&enemy[i],-1);
			   DrawSprite(ge,&enemy[i]);
			   }
			   }
			   }
			 */
			for(j=0;j<2;j++)
			{
				/* When touch player'tank then fire! */
				if (InterSect (x, y, player[j].x, player[j].y))
				{
					EnemyFire (ge, &enemy[i], i);
				}
				/* Can move around */
				if (MoveAble (&enemy[i], x, y))
				{
					/*
					   for(j=0;j<ge->max_enemys;j++){
					   // Two enemy tank touch each onther then change directtion
					   if(enemy[j].active!=1)continue;
					   if(i==j)continue;
					   //if(InterSect(x,y,enemy[j].x,enemy[j].y)){
					   if(HitTest(&enemy[i],&enemy[j])){
					   randnum=(int)(100.0*rand()/(RAND_MAX+1.0));
					   if(randnum<20){
					   ClearSprite(ge,&enemy[i]);
					   ChangeDirection(&enemy[i],-1);
					   DrawSprite(ge,&enemy[i]);
					   }
					   }
					   else {
					   enemy[i].x=x;
					   enemy[i].y=y;
					   ClearSprite(ge,&enemy[i]);
					   DrawSprite(ge,&enemy[i]);
					   }
					   }
					 */
					//if(PlaceAble(&enemy[i])){
					enemy[i].x = x;
					enemy[i].y = y;
					ClearSprite (ge, &enemy[i]);
					DrawSprite (ge, &enemy[i]);
					//}
				}
				else
				{
					/* Agaist the wall,fire or change direction */
					randnum = (int) (100.0 * rand () / (RAND_MAX + 1.0));
					if (randnum < 45)
						EnemyFire (ge, &enemy[i], i);
					else if (randnum > 55)
					{
						ClearSprite (ge, &enemy[i]);
						ChangeDirection (&enemy[i], -1);
						DrawSprite (ge, &enemy[i]);
					}
				}

				/* Sometimes change directtion */
				randnum = (int) (100.0 * rand () / (RAND_MAX + 1.0));
				if (randnum < 2)
				{
					ClearSprite (ge, &enemy[i]);
					ChangeDirection (&enemy[i], -1);
					DrawSprite (ge, &enemy[i]);
				}
				/* And sometimes whill fire */
				if (randnum > 96)
				{
					EnemyFire (ge, &enemy[i], i);
				}
			}
		}
		/* Process big tank */		//装甲坦克!!
		for (i = ge->max_enemys; i < ge->max_enemys + 2; i++)
		{
			//if(i==ge->max_enemys)continue;
			if (enemy[i].active == 1)
			{
				x =enemy[i].x
				   +(enemy[i].dir ==DIR_RIGHT) * enemy[i].speed
				   -(enemy[i].dir == DIR_LEFT) * enemy[i].speed;
				randnum = (int) (100.0 * rand () / (RAND_MAX + 1.0));
				if (x < 48)
				{
					enemy[i].dir = DIR_RIGHT;
				}
				else if (x > 360)
				{
					enemy[i].dir = DIR_LEFT;
				}
				/* Change direction */
				else if (randnum < 2)
					enemy[i].dir = (enemy[i].dir + 2) % 4;
				else
				{
					ClearSprite (ge, &enemy[i]);
					//enemy[i].x+=enemy[i].speed;
					enemy[i].x = x;
					DrawSprite (ge, &enemy[i]);
					enemy[i].ox = enemy[i].x;
				}
				randnum = (int) (100.0 * rand () / (RAND_MAX + 1.0));
				/* Bigtank fire */
				if (randnum < 20)
					BigTankFire (ge, &enemy[i], i);
			}
		}

		/* Process enemy's bullet */	//敌人的子弹  i>=4
		for (i = 0 + 4; i < MAX_ENEMYS + 4 + 2; i++)
		{
			if (bullet[i].active != 1)
				continue;
			bullet[i].ox = bullet[i].x;
			bullet[i].oy = bullet[i].y;
			x = bullet[i].x + bullet[i].speed * dx[bullet[i].dir];
			y = bullet[i].y + bullet[i].speed * dy[bullet[i].dir];
			object = HitObject (&bullet[i], x, y);
			if (object == 0)
			{
				bullet[i].x = x;
				bullet[i].y = y;
				ClearSprite (ge, &bullet[i]);
				DrawSprite (ge, &bullet[i]);
			}
			/* Out off screen */
			else if (object == -1)
			{
				ClearSprite (ge, &bullet[i]);
				bullet[i].active = 0;
			}
			/* Hit the hawk! */
			else if (object == 8)
			{
				Burst (ge, &bullet[i]);
				ExplodeHawk (ge);
				PlaySound (Snd_bom);
				game_state = GAME_OVER;
			}
			else
			{
				ClearSprite (ge, &bullet[i]);
				bullet[i].active = 0;
				if (object == 1)
				{
					HitTile (ge, &bullet[i], x, y);
					PlaySound (Snd_dow);
				}
				if (object == 9)
					PlaySound (Snd_ding);
			}
			/* Hit player's tank */
			for(j=0;j<2;j++)
				if (HitTest (&bullet[i], &player[j]) && !player[j].boring
				        && game_state != GAME_OVER && player[j].active && bullet[i].active)
				{
					Burst (ge, &bullet[i]);
					Burst (ge, &player[j]);
					Explode (ge, &player[j]);
					PlaySound (Snd_bom);
					ge->player_left[j]--;
					if(ge->player_left[j]>0)	//产生新坦克
						BorePlayer (ge, &player[j], j);
					else
						ge->player_left[j]=-10;	//只是需要一个负数
					if (ge->player_left[j] < 0 && ge->player_left[1-j]<0)
					{
						printf ("Game over!\n");
						game_state = GAME_EXIT;
					}

				}

		}

		/* Process explode 爆炸效果
		*/
		for (i = 0; i < 4; i++)
		{
			if (explode[i].active == 1)
			{
				explode[i].timer1++;
				explode[i].timer1 %= 14;
				if (explode[i].timer1 >= 13)
				{
					ClearSprite (ge, &explode[i]);
					explode[i].timer1 = 0;
					explode[i].active = 0;
				}
				ClearSprite (ge, &explode[i]);
				DrawSprite (ge, &explode[i]);
			}
		}
		/* Produce some enemys ... */
		static Uint32 last_time;
		Uint32 cur_time = SDL_GetTicks ();
		if (cur_time - last_time > 1500)
		{
			last_time = cur_time;
			BoreEnemy (ge);
		}

		/* Show some status */
		Disp_TankLeft (ge);
		Show_Score (ge);

		if (game_state == GAME_OVER)
		{
			ShowGameOver (ge);
			ge->clock++;
			if (ge->clock > 200)
				break;

		}
		if (game_state == GAME_WIN)
		{
			ShowRoundClear (ge);
			ge->clock++;
			if (ge->clock > 200)
				break;
		}
		/* Blender frame */
		SDL_UpdateRect (ge->screen, 0, 0, 0, 0);

		/* Delay */
		SDL_Delay (TimeLeft ());

	}
}
