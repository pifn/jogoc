#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define x 9
#define y 9
//map increase
#define mapinc 4
//extra enemies
#define xe 4
//max enemies
#define ob 22
//empty spaces
#define es '-'
#define player 'p'
#define obstacle 'o'
#define reward 'r'
//speed increase (value by phase)
const int spdinc[] = {1,0,0,1,0,1};

void Map(int yaxis, int xaxis, char m[yaxis][xaxis], int p[2], int o[ob][2], int re[2]) {
	printf("\n");
	for (int i = 0; i < yaxis; i++) {
		for (int it = 0; it < xaxis; it++) {
			m[i][it] = (i == p[0] && it == p[1]) ? player : (i == re[0] && it == re[1] ? reward : es);
			for (int ite = 0; ite < ob; ite++) {
				if (i == o[ite][0] && it == o[ite][1]) {
					m[i][it] = obstacle;
				}
			}
			printf("%c ", m[i][it]);
		}
		printf("\n");
	}
	printf("\n");
}

int CheckPos(int p[2], int o[ob][2], int re[2]) {
	if(re[0] == p[0]&&re[1]==p[1])
		return 2; //reward found
	for (int i = 0; i < ob && o[i][0] != -1; i++) {
		if ((p[0] == o[i][0]) && (p[1] == o[i][1])) {
			return 0; //opp found
		}
	}
	return 1; //no opps
}
//round down division
int RDD(int a, int b) {
	return (a - a % b) / b;
}
int main() {
	int r[2];
	int g = 2;
	int speed = 0;
	int df;
	do {
		printf("Choose the game's difficulty(0/1/2): ");
		scanf("%d",&df);
	} while(df<0&&df>2);
	//enemies amount
	int ea = 2,f = 0;
	srand(time(NULL));
	while(ea<=ob&&g==2)
	{
		int axisx = x+f*mapinc, axisy = y+f*mapinc;
		char map[axisy][axisx];
		int pos[2] = {RDD(axisy, 2), RDD(axisx, 2)};
		int opps[ob][2];
		char dir = 'x';
		int i = 0;
		f++;
		printf("\nStage %d",f);
		speed += (df < 2&&f>1) ? 0 : spdinc[f-1];
		do {
			r[1] = rand() % (axisx-1);
			r[0] = rand() % (axisy-1);
		} while(r[0]==pos[0]||r[1]==pos[1]);
		for (i = 0; i < ea; i++) {
			do {
				opps[i][0] = rand() % (axisy-1);
				opps[i][1] = rand() % (axisx-1);
			} while ((opps[i][0] == pos[0] && opps[i][1] == pos[1])||(opps[i][0] == r[0] && opps[i][1] == r[1]));
		}

		for (i = ea; i < ob; i++) {
			opps[i][0] = -1;
			opps[i][1] = -1;
		}
		g = 1;
		while (g == 1) {
			printf("\nCurrent Position: (%d, %d)\n", pos[1]+1, pos[0]+1);
			printf("Reward Position: (%d, %d)\n", r[1]+1,r[0]+1);
			Map(axisy,axisx,map, pos, opps, r);
			printf("Enter direction (w/a/s/d) or 0 to quit: ");
			scanf(" %c", &dir);
			switch (dir) {
			case 'w':
				pos[0]--;
				break;
			case 'a':
				pos[1]--;
				break;
			case 's':
				pos[0]++;
				break;
			case 'd':
				pos[1]++;
				break;
			default:
				break;
			}
			pos[1] = pos[1] < 0 ? axisx-1 : pos[1];
			pos[0] = pos[0] < 0 ? axisy-1 : pos[0];
			pos[1] = pos[1] >= axisx ? 0 : pos[1];
			pos[0] = pos[0] >= axisy ? 0 : pos[0];
			//enemy move
			for(i = 0; i < ea; i++)
			{
				int it = 0;
				if(df==0)
				{
					// printf("\n enemy %d position:(%d, %d)",i+1,opps[i][1],opps[i][0]);
					//right+1
					for(it = 0; (it<speed)&&(pos[1] - it > opps[i][1]); it++);
					// printf("\nr:%d",it);
					if (it == 0)
					{
						//left-1
						for(it; (it>-speed)&&(pos[1] + it < opps[i][1]); it--);
						// printf("\nl:%d",it);
					}
					opps[i][1] += it;
					if(it==0)
					{
						//up+1
						for(it; (it<speed)&&(pos[0] - it > opps[i][0]); it++);
						//   printf("\nu:%d",it);
						if(it == 0)
						{
							//down-1
							for(it; (it>-speed)&&(pos[0] + it < opps[i][0]); it--);
							//   printf("\nd:%d",it);
						}
						opps[i][0] += it;
					}
				}
				else
				{
					int dih = rand() % 2;
					it = pos[dih] - opps[i][dih];
					if(it==0)
					{
						dih = 1 - dih;
						it = pos[dih] - opps[i][dih];
					}
					it = it > speed ? speed : it;
					it = it < -speed ? -speed : it;
					opps[i][dih] += it;
				}
				opps[i][1] = opps[i][1] < 0 ? axisx-1 : opps[i][1];
				opps[i][0] = opps[i][0] < 0 ? axisy-1 : opps[i][0];
				opps[i][1] = opps[i][1] >= axisx ? 0 : opps[i][1];
				opps[i][0] = opps[i][0] >= axisy ? 0 : opps[i][0];
				// printf("\n enemy %d position:(%d, %d)",i+1,opps[i][1],opps[i][0]);
			}
			g = (dir == '0') ? 0 : CheckPos(pos, opps,r);
		}
		printf("\nCurrent Position: (%d, %d)\n", pos[1]+1, pos[0]+1);
		printf("Reward Position: (%d, %d)\n", r[1]+1,r[0]+1);
		Map(axisy, axisx,map, pos, opps, r);
		printf(g == 0 ? "\n" : "Stage Completed\n");
		ea+= xe;
	}
	printf(g == 2 ? "You Win" : "Game Over");
	return 0;
}
