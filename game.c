# include "game.h"
#include "tsi.h"
#define delay_ms(x) for(uint32_t i=0; i<x*10000;i++)__nop();
#define BAR_LENGTH 10
#define BAR_THICK 4
#define PLAYER_HEIGHT 2
unsigned char misses = 0, flag = 0, ballMove = 0;
char txtBuf[20];
extern unsigned char fb[140][25];
struct Bar 
	{
		unsigned char id;
		unsigned char x;
		unsigned char y;
	}bars[57];
	
struct Player
	{
		unsigned char x;
		unsigned char y;
		unsigned char length;
	}player;
	
struct Ball 
{
	unsigned char x;
	unsigned char y;
	unsigned char speed;	
	unsigned char dir;	
}ball;

void mainLoop()
{
	unsigned char val = 0;
	while(1)
	{
		clearScreen();
		gamePrintPlayer(1);
		gamePrintBars();
		gamePrintBall(1);
		delay_ms(3000);
		ballMove = 1;
		while(misses<5)
		{
			val = TSI_ReadSlider();
			if(val>0)gameMovePlayer(val *1.85);
			if(flag == 1)
			{
				flag = 0;
				calculateBallPosition();
			}
			delay_ms(10);
		}
	}
}

void gameInit()
{
	misses = 0;
		TSI_Init();
	player.length = 20;
	player.x = 90;
	player.y = 125;
	
	ball.x = 100;
	ball.y = 124;
	ball.speed = 1;
	ball.dir = 1;
	//gameWelcomeScreen();
	//delay_ms(3000);

	mainLoop();
}
void gameWelcomeScreen()
{
	clearScreen();
	printString("KL-05z VGA", 7, 3);
	delay_ms(3000);
	printString("Simple Ball Game", 5, 40);
	delay_ms(3000);
	printString("Designed by:", 7, 70);
	printString("Mikolaj Pienko", 6, 90);
	printString("Lukasz Przybylik", 5, 100);
	delay_ms(3000);
}
void gamePrintPlayer(unsigned char visibility)
{
	for(unsigned char i=0;i<PLAYER_HEIGHT;i++)
	{
		printLine(player.x, player.y+i,player.x+player.length, player.y+i, visibility);
	}
}
void gameMovePlayer(unsigned char x)
{
	gamePrintPlayer(0);
	player.x = x;
	gamePrintPlayer(1);
}
void gamePrintBars()
{
	for(unsigned char i = 0; i< sizeof(bars)/sizeof(bars[0]);i++)
	{
		bars[i].id = i;
		
	
		if(i>19 && i<=38)
		{
			bars[i].x = i*BAR_LENGTH - 200;
			bars[i].y = BAR_THICK + 2;
		}
		else if(i>38)
		{
			bars[i].x = (i*BAR_LENGTH-390);
			bars[i].y = BAR_THICK*2 + 3;
		}
		else
		{
			bars[i].x = i*BAR_LENGTH;
			bars[i].y = 1;
		}
		printRectangle(bars[i].x+1, bars[i].y, (bars[i].x)+BAR_LENGTH-1, bars[i].y+BAR_THICK, 1);
	}
}
void removeBar(unsigned char id)
{
	printRectangle(bars[id].x+1, bars[id].y, (bars[id].x)+BAR_LENGTH-1, bars[id].y+BAR_THICK, 0);
}
void gamePrintBall(unsigned char visible)
{
	printPoint(ball.x, ball.y, visible);
}
void gameMoveBall(unsigned char x, unsigned char y)
{
	gamePrintBall(0);
	ball.x = x;
	ball.y = y;
	gamePrintBall(1);
}
unsigned char checkForCollision(unsigned char x, unsigned char y)
{
		//fb[y][(uint8_t)x/8] |= (1<<7)>>x%8;	
	if((fb[y][(uint8_t)x/8] & (1<<7)>>x%8))
	{
		if(ball.y>=125)
		{
			if(x<player.x+(player.length/2))
			{
				ball.dir = 4;
			}
			else ball.dir = 1;
		}
		else if(y <120)
		{
			switch(ball.dir)
			{
				case 1:
					for(unsigned int i = 0; i< sizeof(bars)/sizeof(bars[0]);i++)
					{
						if(x < bars[i].x+BAR_LENGTH  && x>bars[i].x && y>=bars[i].y && y<=bars[i].y+BAR_THICK)
						{
							removeBar(i);
							if(y == bars[i].y+BAR_THICK-1)ball.dir = 2;
							else ball.dir = 4;
						}
					}
					break;
				case 2:
					for(unsigned int i = 0; i< sizeof(bars)/sizeof(bars[0]);i++)
					{
						if(x < bars[i].x+BAR_LENGTH  && x>bars[i].x&& y>=bars[i].y && y<=bars[i].y+BAR_THICK)
						{
							removeBar(i);
							if( y == bars[i].y)	ball.dir = 1;
							else ball.dir = 3;
						}
					}
					break;
				case 3:
					for(unsigned int i = 0; i< sizeof(bars)/sizeof(bars[0]);i++)
					{
						if(x < bars[i].x+BAR_LENGTH  && x>bars[i].x&& y>=bars[i].y && y<=bars[i].y+BAR_THICK)
						{
							removeBar(i);
							if( y == bars[i].y)	ball.dir = 4;
							else ball.dir = 2;
						}
					}
					break;
				case 4:
					for(unsigned int i = 0; i< sizeof(bars)/sizeof(bars[0]);i++)
					{
						if(x < bars[i].x+BAR_LENGTH  && x>bars[i].x&& y>=bars[i].y && y<=bars[i].y+BAR_THICK)
						{
							removeBar(i);
							if( y == bars[i].y+BAR_THICK-1)	ball.dir = 3;
							else ball.dir = 1;
						}
					}
					break;
				default:
					
					break;
			}
		}
		return 1;
	}
	else if(x<=1)
	{
		switch(ball.dir)
		{
			case 3:
				ball.dir = 2;
				break;
			case 4:
				ball.dir = 1;
				break;
			default:
				
				break;
		}
		return 1;
	}
	else if(x >= 200)
	{
		switch(ball.dir)
		{
			case 1:
				ball.dir = 4;
				break;
			case 2:
				ball.dir = 3;
				break;
			default:
				
				break;
		}
		return 1;
	}
	else if(y<=1)
	{
		switch(ball.dir)
		{
			case 1:
				ball.dir = 2;
				break;
			case 4:
				ball.dir = 3;
				break;
			default:
				
				break;
		}
		return 1;
	}
	else if(y>=130)
	{
		misses ++;
		if(misses <5)
		{
			sprintf(txtBuf, "Missed  %d/5", misses);
			printString(txtBuf, 2, 80);
			delay_ms(3000);
			printString("            ", 2, 80);
			gameMoveBall(player.x + 10, player.y-1);
			ball.dir =  1;
		}
		else
		{
			clearScreen();
			printString("GAME OVER!", 2, 60);
			delay_ms(5000);
			gameInit();
		}
		return 1;
	}
	else return 1;
}
void calculateBallPosition()
{
	if(ballMove)
	{
		switch(ball.dir)
		{
			case 1:
				if(checkForCollision(ball.x+1, ball.y-1))
				{
					gamePrintBall(0);
					ball.x += 1;
					ball.y -= 1;
					gamePrintBall(1);
				}
				break;
			case 2:
				if(checkForCollision(ball.x+1, ball.y+1))
				{
					gamePrintBall(0);
					ball.x += 1;
					ball.y += 1;
					gamePrintBall(1);
				}
			break;
			case 3:
				if(checkForCollision(ball.x-1, ball.y+1))
				{
					gamePrintBall(0);
					ball.x -= 1;
					ball.y += 1;
					gamePrintBall(1);
				}
				break;
			case 4:
				if(checkForCollision(ball.x-1, ball.y-1))
				{
					gamePrintBall(0);
					ball.x -= 1;
					ball.y -= 1;
					gamePrintBall(1);
				}
				break;
			
			default:
				
				break;
		}
	}
}
void updateFlagSet()
{
	flag = 1;
}