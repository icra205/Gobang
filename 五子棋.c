#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRUE 1
#define FALSE 0

#define player 0
#define player_player 1
#define player_machine 2
#define player_tui 3


/*初始设置*/
#define CHESS_N 19
#define CHESS_position_MAX_NUM 100

/*函数区*/ 
	
	/*主函数区*/
	
		/*初始化*/ 
			/*棋盘初始化，player为交战区初始化，player_machine为记忆空间初始化，player_tui为推演空间初始化*/ 
				void init_board(int flag);

			/*评分初始化，player为全局评分空间初始化，player_machine或player_tui为学习空间评分初始化*/ 
				void init_positions(int flag);
		
			
		/*输出交互区*/ 
		
			/*欢迎界面*/
				void welcome(); 
	
			/*棋盘更新*/ 
				void print_board();
	
			/*落子函数*/ 
				/*落子函数——人*/ 
					void scan(void);
				
				/*落子函数——机*/ 
					/*求出评分最高的，落子*/ 
					void think_act(void);
				
		/*裁判判定区*/ 
	
			/*是否已满——平局*/ 
				int is_full(void);

			/* cp 方是否获胜*/ 
				void is_win(int x, int y, char cp);
				
		/*评分统计函数*/ 
			void sense(); 
			/*辅助函数，检测函数*/ 
			int jiance(int y,int x);
			
			void jiance_mac();
			
	
	/*机器判别区（评分详解）*/ 
	
		/*评分系统*/
			
			/*单次评分系统*/
			int score(char row[], int x);/*辅助函数，单方向评分函数*/
			void reverse(char row[],int len);/*辅助函数，将单向评分简化*/ 
	
				/*交战区评分*/ 
				
				/*求分函数*/ 
					
					/*求和函数*/ 
					int sense_sum(int x,int y);
			
						int sense_row(int x, int y);/*横向*/ 
						int sense_col(int x, int y);/*纵向*/ 
						int sense_right_bias(int x, int y);/*右斜*/ 
						int sense_left_bias(int x, int y);/*左斜*/  
				
				
				/*推演区单次评分*/ 
				
				/*求分函数*/ 
					
					/*求和函数*/ 
					int sense_sum_0(int x,int y);
			
						int sense_row_0(int x, int y);/*横向*/
						int sense_col_0(int x, int y);/*纵向*/ 
						int sense_right_bias_0(int x, int y);/*右斜*/ 
						int sense_left_bias_0(int x, int y);/*左斜*/  
			
				
				
			/*推演函数*/ 
			
				/*Q学习，推演出最高评分*/ 
				double sense_get(int scan_current_0_y,int scan_current_0_x);
				
					/*子函数*/  
					 
						/*初始化推演空间与学习评分区*/	
						void init_learn();
						/*辅助函数，检测函数*/ 
						int check_all_0();
						/*推演函数（下一状态）*/ 
						void chess_tui_update(int x,int y);
						/*更新函数（更新为下一状态）*/ 
						void chess_update();
						/*模拟对手落子*/ 
						void chess_tui_scan();
		
	


/*变量区*/ 
	/*棋盘*/ 
		/*交战区*/
		char chess_board[CHESS_N][CHESS_N];
		
		/*推演空间*/ 
		char chess_board_think_mac[CHESS_N][CHESS_N];/*机器记忆空间*/ 
		char chess_board_think_tui[CHESS_N][CHESS_N];/*机器推演空间*/ 
		
	/*辅助变量*/
		int chess_x,chess_y;
		int chess_x0,chess_y0;




/*评分区*/ 

	/*全局评分结构*/
		struct position 
		{
			int x;
			int y;
			double score;
		}; 

	/*全局评分区*/
		struct position positions[CHESS_position_MAX_NUM]; 
		
		/*评分区控制变量*/ 
			int position_order;
	
	/*Q学习评分区*/ 
	double q_learn_value_mac[CHESS_N][CHESS_N];



int flag=0;

/*初始化*/ 
void init_board(int flag)
{
	int i, j;
	
	if(flag==0)
	{
		for (i = 0;i < CHESS_N;i++) 
		{
			for (j = 0;j < CHESS_N;j++) 
			{
				chess_board[i][j] = ' ';
			}
		}
	}
	else
	{
			if(flag==2)
			{
				for (i = 0;i < CHESS_N;i++) 
				{
					for (j = 0;j < CHESS_N;j++) 
					{
						chess_board_think_mac[i][j] = ' ';
			
					}
				}
			}
			else
			{
				for (i = 0;i < CHESS_N;i++) 
				{
					for (j = 0;j < CHESS_N;j++) 
					{
						chess_board_think_tui[i][j] = ' ';
			
					}
				}
			}
	}
}

void init_positions(int flag)
{
	int i,j;
	
	if(flag==0)
	{
		positions[0].x = positions[0].y = positions[0].score = 0;
		for (position_order = 1;position_order < CHESS_position_MAX_NUM;position_order++) 
		{
			positions[position_order] = positions[0];
		}
		position_order = 0;
	}
	else
	{
		
			for (i = 0;i < CHESS_N;i++) 
			{
				for (j = 0;j < CHESS_N;j++) 
				{
					q_learn_value_mac[i][j] = 0;
				}
			}
		
	}
	
}

/*输出交互区*/

void welcome()
{
	printf("**************************************************************************\n");
	printf("                  傻瓜五子棋\n");
	printf("            Welcome!Have a good time！\n");
	printf("**************************************************************************\n");
	printf("\n\n\n人机模式：\n输入AI\n");
	printf("对抗模式：\n输入WAR\n");
	printf("\n\n\n\n\nTIP：\n如果想要终止比赛，输入EXIT\n\n");
	printf("**************************************************************************\n");
	printf("            Have a good time！\n");
} 

void print_board()
{
	int i, j;
	/*print letters*/
	printf("  ");
	for (j = 0;j < CHESS_N;j++) 
	{
		printf(" %c", 'A' + j);
	}
	putchar('\n');
	/*print chess board*/
	for (i = 0;i < CHESS_N;i++)  
	{
		
		printf("%2d", i+1);
		
		
		
		for (j = 0;j < CHESS_N;j++)  
		{
			/*The first row*/
			if(chess_board[i][j]!=' ')
				{
					if (chess_board[i][j]=='*') 
						printf("●");/*Unicode编码号：0x25cf*/
						
					if (chess_board[i][j]=='O') 
						printf("○");/*Unicode编码号：0x25cb*/
				} 
			
			else
			{
				if (i == 0 && j == 0) 
				{
					printf("┌ ");
				}
				else if (i == 0 && j==CHESS_N-1) 
				{
						printf("┐");
				}
				else if (i == 0 ) 
				{
					printf("┬ ");
				}
				/*The last row*/
				if (i == CHESS_N-1 && j == 0) 
				{
					printf("└ ");
				}
				else if (i == CHESS_N-1 && j == CHESS_N-1) 
				{
					printf("┘");
				}
				else if (i == CHESS_N-1 ) 
				{
					printf("┴ ");
				}
				/*The other rows*/
				if (i != 0 && i != CHESS_N-1) 
				{
					if (j == 0) 
					{
						printf("├ ");
					}
					else if (j == CHESS_N-1) 
					{
						printf("┤");
					}
					else  
						printf("┼ ");
				}
				
			}
				
		}
		
		putchar('\n');
	}
		
}

/*落子函数——人*/
void scan(void)
{
	char c;
	int i;
	
	printf("输入落子行列：\n");
	
	do 
	{
		scanf_s("%d %c", &i, &c);
			
		if (i - 1 > CHESS_N -1 ||c - 'A' > CHESS_N -1||i - 1 < 0||c - 'A' < 0)
			{
				printf("超出边界！！！\n请重新输入落子行列：\n");
				continue;
			}
			
			
		if ((chess_board[i - 1][c - 'A'] != ' '))
			{
				printf("已有落子！！！\n请重新输入落子行列：\n");
				continue;
			}
		
		chess_board[i - 1][c - 'A'] = '*';
		break;
	} while (TRUE);
	is_win(c - 'A', i - 1, '*');
}

void scan_player_A(void)
{
	char c;
	int i;
	
	printf("黑方：\n");
	printf("输入落子行列：\n");
	
	do 
	{
		scanf_s("%d %c", &i, &c);
			
		if (i - 1 > CHESS_N -1 ||c - 'A' > CHESS_N -1||i - 1 < 0||c - 'A' < 0)
			{
				printf("超出边界！！！\n请重新输入落子行列：\n");
				continue;
			}
			
			
		if ((chess_board[i - 1][c - 'A'] != ' '))
			{
				printf("已有落子！！！\n请重新输入落子行列：\n");
				continue;
			}
		
		chess_board[i - 1][c - 'A'] = '*';
		break;
	} while (TRUE);
	is_win(c - 'A', i - 1, '*');
}

void scan_player_B(void)
{
	char c;
	int i;
	
	printf("白方：\n");
	printf("输入落子行列：\n");
	
	do 
	{
		scanf_s("%d %c", &i, &c);
			
		if (i - 1 > CHESS_N -1 ||c - 'A' > CHESS_N -1||i - 1 < 0||c - 'A' < 0)
			{
				printf("超出边界！！！\n请重新输入落子行列：\n");
				continue;
			}
			
			
		if ((chess_board[i - 1][c - 'A'] != ' '))
			{
				printf("已有落子！！！\n请重新输入落子行列：\n");
				continue;
			}
		
		chess_board[i - 1][c - 'A'] = 'O';
		break;
	} while (TRUE);
	is_win(c - 'A', i - 1, 'O');
}

/*落子函数——机*/
void think_act(void)
{
	int  max_order, i;
	double max = 0;
	for (i = 0;i < position_order;i++)
	{
		if (positions[i].score > max) 
		{
			max = positions[i].score;
			max_order = i;
		}
	}
	chess_board[positions[max_order].y][positions[max_order].x] = 'O';
	is_win(positions[max_order].x, positions[max_order].y, 'O');
}



/*裁判判定区*/

int is_full(void) 
{
	int i, j;
	for (i = 0;i < CHESS_N;i++) 
	{
		for (j = 0;j < CHESS_N;j++) 
		{
			if (chess_board[i][j] == ' ')
				return(FALSE);
		}
	}
	return(TRUE);
}

void is_win(int x, int y, char cp)
{
	int i, num=0;
	
	for (i = 0;chess_board[y][x + i] == cp;i++, num++);
	for (i = -1;chess_board[y][x + i] == cp;i--, num++);
	if (num >= 5) {
		system("cls");
		print_board();
		
		if(cp=='*')
		printf("● win!\n");
		else
		printf("○ win!\n");
		
		flag=1;
		system("pause");
		init_board(player);
		init_positions(player);
	}
	else 
	{
		num = 0;
	}
	
	for (i = 0;chess_board[y + i][x] == cp;i++, num++);
	for (i = -1;chess_board[y + i][x] == cp;i--, num++);
	
	if (num >= 5)
	{
		system("cls");
		print_board();
		
		if(cp=='*')
		printf("● win!\n");
		else
		printf("○ win!\n");
		
		flag=1;
		system("pause");
		init_board(player);
		init_positions(player);
	}
	else 
	{
		num = 0;
	}
	
	for (i = 0;chess_board[y + i][x + i] == cp;i++, num++);
	for (i = -1;chess_board[y + i][x + i] == cp;i--, num++);
	
	if (num >= 5) 
	{
		system("cls");
		print_board();
		
		if(cp=='*')
		printf("● win!\n");
		else
		printf("○ win!\n");
		
		flag=1;
		system("pause");
		init_board(player);
		init_positions(player);
	}
	else 
	{
		num = 0;
	}
	
	for (i = 0;chess_board[y + i][x - i] == cp;i++, num++);
	for (i = -1;chess_board[y + i][x - i] == cp;i--, num++);
	
	if (num >= 5) 
	{
		system("cls");
		print_board();
		
		if(cp=='*')
		printf("● win!\n");
		else
		printf("○ win!\n");
		
		flag=1;
		system("pause");
		init_board(player);
		init_positions(player);
	}
	else 
	{
		num = 0;
	}
	
}


/*机器判别区*/


/*评分辅助函数*/ 

void reverse(char row[],int len)            
{
	char temp;
	int i,j;
	for (i = 0, j = len - 1;i <= j;i++, j--) 
	{
		temp = row[i];
		row[i] = row[j];
		row[j] = temp;
	}
}

int score(char row[], int x)                
{                                           
	if (x > 2) 
	{                        
		reverse(row,5);             
		x = 4 - x;                 
	}
	switch (x) 
	{
	case 0: 
	{
		if (row[1] == 'O' && (row[2] == 'O' || row[2] == ' ') && (row[3] == 'O' || row[3] == ' ') && (row[4] == 'O' || row[4] == ' ')) {
			if (row[x + 2] == ' ') 
			{		
				return(5);
			}
			else if (row[x + 3] == ' ') 
			{	
				return(30);
			}
			else if (row[x + 4] == ' ') 
			{	
				return(70);
			}
			else 
			{							
				return(500);
			}
		}
		else if (row[1] == '*' && (row[2] == '*' || row[2] == ' ') && (row[3] == '*' || row[3] == ' ') && (row[4] == '*' || row[4] == ' ')) {
			if (row[x + 2] == ' ') 
			{	
				return(15);
			}
			else if (row[x + 3] == ' ')
			{	
				return(50);
			}
			else if (row[x + 4] == ' ') 
			{	
				return(90);
			}
			else {						
				return(1000);
			}
		}
	};
	break;
	
	case 1:
	{
		if ((row[0] == 'O' || row[0] == ' ') && (row[2] == 'O' || row[2] == ' ') && (row[3] == 'O' || row[3] == ' ') && (row[4] == 'O' || row[4] == ' ')) {
			if (row[0] == 'O') {
				if (row[2] == ' ')
				{		
					return(5);
				}
				else if (row[3] == ' ')
				{	
					return(30);
				}
				else if (row[4] == ' ') 
				{	
					return(70);
				}
				else {						
					return(500);
				}
			}
			else if (row[2] == ' ') 
			{		
				return(0);
			}
			else if (row[3] == ' ') 
			{		
				return(5);
			}
			else if (row[4] == ' ') 
			{		
				return(30);
			}
			else {							
				return(60);
			}
		}
		else if ((row[0] == '*' || row[0] == ' ') && (row[2] == '*' || row[2] == ' ') && (row[3] == '*' || row[3] == ' ') && (row[4] == '*' || row[4] == ' ')) {
			if (row[0] == '*') {
				if (row[2] == ' ') 
				{		
					return(15);
				}
				else if (row[3] == ' ') 
				{	
					return(50);
				}
				else if (row[4] == ' ') 
				{	
					return(90);
				}
				else 
				{						
					return(1000);
				}
			}
			else if (row[2] == ' ') 
			{		
				return(0);
			}
			else if (row[3] == ' ') 
			{		
				return(15);
			}
			else if (row[4] == ' ') 
			{		
				return(50);
			}
			else 
			{							
				return(80);
			}
		}
	}
	break;
	
	case 2: 
	{
		if ((row[0] == 'O' || row[0] == ' ') && (row[1] == 'O' || row[2] == ' ') && (row[3] == 'O' || row[3] == ' ') && (row[4] == 'O' || row[4] == ' ')) {
			if (row[1] == 'O') 
			{
				if (row[3] == 'O') 
				{
					if (row[0] == 'O')
					{
						if (row[4] == 'O') 
						{
							return(500);
						}
						else 
						{
							return(70);		
						}
					}
					else 
					{
						if (row[4] == 'O') 
						{
							return(30);
						}
						else 
						{				
							return(20);
						}
					}
				}
				else {
					if (row[0] == 'O') 
					{	
						return(50);
					}
					else 
					{					
						return(5);
					}
				}
			}
			else 
			{
				if (row[3] == 'O') 
				{
					if (row[4] == 'O') 
					{	
						return(20);
					}
					else
					{					
						return(5);
					}
				}
			}
		}
		else if ((row[0] == '*' || row[0] == ' ') && (row[1] == '*' || row[2] == ' ') && (row[3] == '*' || row[3] == ' ') && (row[4] == '*' || row[4] == ' ')) {
			if (row[1] == '*') 
			{
				if (row[3] == '*') 
				{
					if (row[0] == '*') 
					{
						if (row[4] == '*') 
						{
							return(1000);
						}
						else 
						{				
							return(90);
						}
					}
					else 
					{
						if (row[4] == '*') 
						{
							return(90);
						}
					}
				}
				else 
				{
					if (row[0] == '*') 
					{	
						return(40);
					}
					else 
					{					
						return(15);
					}
				}
			}
			else {
				if (row[3] == '*') 
				{		
					if (row[4] == '*') 
					{
						return(40);
					}
					else 
					{					
						return(15);
					}
				}
			}
		}
	}
	break;
	
	}
	
	return(3);
}

/*推演区单次评分*/ 
int sense_sum(int x,int y)
{
	int sum;
	
	sum += sense_col(x, y);			
	sum += sense_row(x, y);			
	sum += sense_left_bias(x, y);			
	sum += sense_right_bias(x, y);	
	
	return sum;
}


int sense_row(int x, int y)        
{                                  	
	int sum = 0, i, j;
	char row[5];	
	for (i = x - 4;i <= x;i++) 
	{		
		if (!(i >= 0 && i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_mac[y][i + j];
			}			
			sum += score(row, x - i);		
			
		}	
		
	}	
	
	return(sum);
	
}

int sense_col(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	for (i = y - 4;i <= y;i++) 
	{		
		if (!(i >= 0 && i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_mac[i + j][x];	
			}			
			
			sum += score(row, y - i);		
			
		}	
		
	}	
	
	return(sum);
	
}

int sense_right_bias(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	
	for (i = -4;i <= 0;i++) 
	{		
		if (!(y + i >= 0 && x + i >= 0 && y + i + 4 <= CHESS_N-1 && x + i + 4 <= CHESS_N-1 )) 
		{			
			continue;
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_mac[y + i + j][x + i + j];			
			}			
			sum += score(row, -i);		
			
		}
		
	}	
	return (sum);
}

int sense_left_bias(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	for (i = -4;i <= 0;i++) 
	{		
		if (!(y - i <= CHESS_N-1 && x + i >= 0 && y - i - 4 >= 0 && x + i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_mac[y - i - j][x + i + j] ;		
			}			
			sum += score(row, -i);		
		}	
	}	
	return(sum);
}



/*交战区评分*/ 

int sense_sum_0(int x,int y)
{
	int sum;
	
	sum += sense_col_0(x, y);			
	sum += sense_row_0(x, y);			
	sum += sense_left_bias_0(x, y);			
	sum += sense_right_bias_0(x, y);	
	
	return sum;
}

int sense_row_0(int x, int y)        
{                                  	
	int sum = 0, i, j;
	char row[5];	
	for (i = x - 4;i <= x;i++) 
	{		
		if (!(i >= 0 && i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_tui[y][i + j];
			}			
			sum += score(row, x - i);		
			
		}	
		
	}	
	
	return(sum);
	
}

int sense_col_0(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	for (i = y - 4;i <= y;i++) 
	{		
		if (!(i >= 0 && i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_tui[i + j][x];	
			}			
			
			sum += score(row, y - i);		
			
		}	
		
	}	
	
	return(sum);
	
}

int sense_right_bias_0(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	
	for (i = -4;i <= 0;i++) 
	{		
		if (!(y + i >= 0 && x + i >= 0 && y + i + 4 <= CHESS_N-1 && x + i + 4 <= CHESS_N-1 )) 
		{			
			continue;
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_tui[y + i + j][x + i + j];			
			}			
			sum += score(row, -i);		
			
		}
		
	}	
	return (sum);
}

int sense_left_bias_0(int x, int y)
{	
	int sum = 0, i, j;	
	char row[5];	
	for (i = -4;i <= 0;i++) 
	{		
		if (!(y - i <= CHESS_N-1 && x + i >= 0 && y - i - 4 >= 0 && x + i + 4 <= CHESS_N-1 )) 
		{			
			continue;		
		}		
		else 
		{			
			for (j = 0;j < 5;j++) 
			{				
				row[j] = chess_board_think_tui[y - i - j][x + i + j] ;		
			}			
			sum += score(row, -i);		
		}	
	}	
	return(sum);
}




/*推演函数*/

double sense_get(int scan_current_0_y,int scan_current_0_x)            //Q学习 
{	
	int x, y, sum = 0;
	double num;
	
	double epsilon = 0.9 ;  // 贪婪度 greedy
	double alpha = 0.1    ; // 学习率
	double gamma = 0.8    ; // 奖励递减值	
	int num_update=8;   // 更新次数 
	int num_step;      // 推演步数 （控制） 
	int total_steps = 0; //  推演步数（交互） 
	
	int i,t,num_i_0;
	double a,num_i;
	int scan_learn_x,scan_learn_y;
	int scan_current_y,scan_current_x;
	
	init_learn();
	
	
	for (i=0;i<num_update;i++)
	{
		for (y = 0;y < CHESS_N;y++) 
		{		
			for (x = 0;x < CHESS_N;x++) 
			{				
				chess_board_think_mac[y][x]=chess_board[y][x];
			}	
		}
		scan_current_y=scan_current_0_y;
		scan_current_x=scan_current_0_x;
		
		total_steps = 0;
		num_step=5;
		
		while(num_step--)
		{
			
			
			
			chess_board_think_mac[scan_current_y][scan_current_x]= 'O';
			chess_tui_scan();
			
			do
			{
				srand((unsigned)time(NULL));
    			a = rand() % 1000 ;
    			num_i = epsilon*1000;
    			
    			jiance_mac();
    		
				if ( (a  > num_i ) || check_all_0() )  // 探索
			{
				scan_learn_x=(rand()%(chess_x0-chess_x) )+ chess_x+2;
				scan_learn_y=(rand()%(chess_y0-chess_y) )+ chess_y+2;
			}	
			else
			{
				
				for (y = 0;y < CHESS_N;y++) 
				{		
					for (x = 0;x < CHESS_N;x++) 
					{				
						num=q_learn_value_mac[0][0];
						scan_learn_x=0;
						scan_learn_y=0;
						if(num<q_learn_value_mac[y][x])
						{
							num=q_learn_value_mac[y][x];
							scan_learn_x=x;
							scan_learn_y=y;
						}
					}	
				}
			}
			
				if(chess_board_think_mac[scan_learn_y][scan_learn_x] != ' ')
				{
					scan_learn_x=rand()%(chess_x0-chess_x) + chess_x;
					scan_learn_y=rand()%(chess_y0-chess_y) + chess_y;
				}
				
			}while(chess_board_think_mac[scan_learn_y][scan_learn_x] != ' ');
			
			chess_tui_update(scan_learn_x,scan_learn_y);
			
			for (y = 0;y < CHESS_N;y++) 
			{		
				for (x = 0;x < CHESS_N;x++) 
				{				
					num_i_0=q_learn_value_mac[0][0];
					if(num_i_0<q_learn_value_mac[y][x])
					{
						num_i_0=q_learn_value_mac[y][x];
					}		
				}	
			}
			t=sense_sum(scan_learn_x,scan_learn_y)/4;
				
			q_learn_value_mac[scan_current_y][scan_current_x] += alpha*(t + gamma * num_i_0 - q_learn_value_mac[scan_current_y][scan_current_x]);
			
			chess_update();
			
			scan_current_y=scan_learn_y;
			scan_current_x=scan_learn_x;
			
			total_steps++;
		} 
		
	} 
	

	
		for (y = 0;y < CHESS_N;y++) 
		{		
			for (x = 0;x < CHESS_N;x++) 
			{				
				num=q_learn_value_mac[0][0];
				if(num<q_learn_value_mac[y][x])
				{
					num=q_learn_value_mac[y][x];
				}
			}	
		}
	
	return num;
}


/*学习辅助函数*/ 

void init_learn()
{
	
	
	init_positions(player_machine);	
	
	init_board(player_machine);
	
} 

int check_all_0()
{
	int flag_value;
	
	flag_value=1;
	
	int x,y;
	
		for (y = 0;y < CHESS_N;y++) 
		{		
			for (x = 0;x < CHESS_N;x++) 
			{				
				if(q_learn_value_mac[y][x]!=0)
				flag_value=0;
			}
		}
	
	return flag_value;
}



void chess_tui_update(int x,int y)
{
	
	
		for (y = 0;y < CHESS_N;y++) 
		{		
			for (x = 0;x < CHESS_N;x++) 
			{				
				chess_board_think_tui[y][x] = chess_board_think_mac[y][x];
			}	
		}
	
	

}
void chess_update()
{
	int x,y;
		for (y = 0;y < CHESS_N;y++) 
		{		
			for (x = 0;x < CHESS_N;x++) 
			{				
				chess_board_think_mac[y][x] = chess_board_think_tui[y][x];
			}	
		}
	

}



void chess_tui_scan()
{
	int x,y; 
	int scan_learn_0_x, scan_learn_0_y;
	int num;
	
	for (y = 0;y < CHESS_N;y++) 
	{		
		for (x = 0;x < CHESS_N;x++) 
		{				
			num=0;
			if(chess_board_think_mac[y][x]==' ')
			{
				if(num<sense_sum(x,y))
				{
					num=sense_sum(x,y);
					scan_learn_0_x=x;
					scan_learn_0_y=y;
				}
			}
		}	
	}
			
	chess_board_think_mac[scan_learn_0_y][scan_learn_0_x]= '*';
	
}



/*评分统计函数*/ 

/*辅助函数，检测函数*/ 
int jiance(int y,int x)
{
	int i,j;
	int num_a,num_b;
	int num_0a,num_0b;
	
	int flag=0;
	
	if(y-2<0)
	num_a=0;
	else
	num_a=y-2;
	
	if(x-2<0)
	num_b=0;
	else
	num_b=x-2;
	
	if(y+2>CHESS_N-1)
	num_0a=CHESS_N-1;
	else
	num_0a=y+2;
	
	if(x+2>CHESS_N-1)
	num_0b=CHESS_N-1;
	else
	num_0b=x+2;
	
	for (i = num_a;i < num_0a;i++) 
	{		
		for (j = num_b;j < num_0b;j++) 
		{
			if(chess_board[i][j]!=' ')
			flag=1;
		}
	}
	
	return flag;
}

void jiance_mac()
{
	int i,j;
	int num_a=0,num_b=0;
	int num_0a=CHESS_N,num_0b=CHESS_N;
	
	
	
	for (i = 0;i < CHESS_N;i++) 
	{		
		for (j = 0;j < CHESS_N;j++) 
		{
			if(chess_board_think_mac[i][j]!=' ')
			{
				if(i<num_a)
				num_a=i;
				
				if(i>num_0a)
				num_0a=i;
				
				if(j<num_b)
				num_b=j;
				
				if(j>num_0b)
				num_0b=j;
				
			}
		}
	}
	
	
	
	if(num_a-2<0)
	num_a=0;
	else
	num_a=num_a-2;
	
	if(num_b-2<0)
	num_b=0;
	else
	num_b=num_b-2;
	
	if(num_0a+2>CHESS_N-1)
	num_0a=CHESS_N-1;
	else
	num_0a=num_0a+2;
	
	if(num_0b+2>CHESS_N-1)
	num_0b=CHESS_N-1;
	else
	num_0b=num_0b+2;
	
	chess_x=num_a;
	chess_x0=num_0a;
	chess_y=num_b;
	chess_y0=num_0b;
	

}
/*评分统计函数*/ 
void sense()
{
	int x,y;
	double sum=0;
	
	init_positions(player);	
	for (y = 0;y < CHESS_N;y++) 
	{		
		for (x = 0;x < CHESS_N;x++) 
		{			
			if (chess_board[y][x] != ' '||(!jiance(y,x))) 
			{				
				continue;		
			}			
			sum=sense_get(y, x);			
			if (sum != 0) 
			{				
				positions[position_order].score = sum;				
				positions[position_order].x = x;				
				positions[position_order].y = y;				
				position_order++;				
				sum = 0;			
			}		
		}	
	}

}



/*主函数*/  
int main(void)
{
	char wel[5];
	
	while(1)
	{
		welcome();
		scanf("%s",&wel);
		flag=0;
		if(wel[0]=='E'&&wel[1]=='X'&&wel[2]=='I'&&wel[3]=='T')
		{
			break;
		} 
		else if(wel[0]=='A'&&wel[1]=='I')
		{
			system("CLS");
			init_board(player);
			init_positions(player);
			print_board();
			while (!is_full()&&flag==0) 
			{
				scan();
				sense();
				think_act();
				system("CLS");
				print_board();
			}
			if(is_full())
			{
				printf("你们难分高下，平局\n");
				system("pause");
				system("CLS");
			}
		}
		else if(wel[0]=='W'&&wel[1]=='A'&&wel[2]=='R')
		{
			system("CLS");
			init_board(player);
			init_positions(player);
			print_board();
			while (!is_full()&&flag==0) 
			{
				scan_player_A();
				system("CLS");
				print_board();
				scan_player_B();
				system("CLS");
				print_board();
			}
			if(is_full())
			{
				printf("你们难分高下，平局\n");
				system("pause");
				system("CLS");
			}
		}
		else
		{
			system("CLS");
			printf("输入不对哦，亲<~.~>\n");
		}
	} 
	
	return 0;
}


