#include <graphics.h>
#include <math.h>
#include <iostream>
#include <windows.h>
using namespace std;

// 宏定义常量
#define BLOCK_SIZE 20  // 每个小格子的长宽大小
#define WIDTH 40       // 宽度上一共40个小格子
#define HEIGHT 30      // 高度上一共30个小格子

// 全局变量定义
int blocks[HEIGHT][WIDTH] = { 0 }; // 初始化一个全0的矩阵，用来记录蛇身体的颜色，这里用HSV颜色空间
char move_direction; // 用w a s d控制小蛇移动方向 
int food_i, food_j;  // 食物的位置
int is_failure = 0;  // 是否游戏失败
int length=4;		// 记录蛇的长度

// 程序初始化函数 应该包括初始时蛇的位置，颜色，食物的位置等等
void start_up() {
	// 初始化画布大小
	initgraph(WIDTH*BLOCK_SIZE, HEIGHT*BLOCK_SIZE);
	randomize(); // 初始化随机数发生器
	setrendermode(RENDER_MANUAL);  //设置为非自动刷新 (只有delay/delay_fps/getch 才刷新屏幕） 
	blocks[HEIGHT / 2][WIDTH / 2] = 1; // 画面中间画蛇头
	for (int i = 1; i <= length; i++) { // 向左依次4个方块，数值依次为2，3，4，5 
		blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	}
	move_direction = 'd'; // 初始向右移动
	food_i = random(HEIGHT -5) ; // 随机初始化食物位置
	food_j = random(WIDTH - 5);
}

//绘制函数 蛇的长度，蛇和食物的位置都是时刻变化的，所以要不断刷新画布上的内容，必须不断调用这个函数
void show() { 
	color_t color;
	cleardevice();// 清屏

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (blocks[i][j]) {
				// 蛇身颜色，将HSV颜色空间转化为RGB形式
				color = HSVtoRGB(blocks[i][j] * 10, 0.8, 1);
			}
			else {
				//普通格子颜色，也就是矩阵中数字为0的颜色
				color = EGERGB(150, 150, 150);
			}
			// 不同填充不同的颜色
			setfillcolor(color);
			//i是行(y)，j是列(x)，双重for循环就都填充了
			bar(j*BLOCK_SIZE, i*BLOCK_SIZE, (j + 1)*BLOCK_SIZE, (i + 1)*BLOCK_SIZE);
		}
	}
	// 食物默认颜色为绿色GREEN
	setfillcolor(GREEN);
	// 食物位置为全局变量，在更新蛇的长度和位置时，食物位置也随之更新
	bar(food_j*BLOCK_SIZE, food_i*BLOCK_SIZE, (food_j + 1)*BLOCK_SIZE, (food_i + 1)*BLOCK_SIZE);
	// 显示蛇的长度
	setcolor(EGERGB(255, 0, 0));
	setfont(20, 0, "宋体");
	xyprintf(10, 580, "length: %d", length+1);
}

// 移动小蛇及相关处理函数 
void move_snake() {
	// 旧蛇头、蛇尾坐标,i=x坐标，j=y坐标
	int old_head_i, old_head_j, old_tail_i, old_tail_j;
	int max = 0;
	// 移动蛇，将矩阵中所有非0数的位置都加1，蛇头的向哪移动，蛇头的前一个位置元素就变成1，最后一个元素变成0
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			// 这一步是将所有非0位置（蛇身）的数值都 + 1
			if (blocks[i][j] > 0)
				blocks[i][j]++;
			// 旧蛇尾位置的值最大，将最大值的x,y 赋值给蛇尾坐标
			if (max < blocks[i][j]) {
				max = blocks[i][j];
				old_tail_i = i;
				old_tail_j = j;
			}
			// 旧蛇头位置的值为2，因为一开始蛇头的数值应该是1，非0数都加1后就成了2
			if (2 == blocks[i][j]) {
				old_head_i = i;
				old_head_j = j;
			}
		}
	}

	// 先将旧蛇头的位置赋值给新蛇头，根据蛇不同方向的移动，再改变蛇头的坐标位置
	int new_head_i = old_head_i;
	int new_head_j = old_head_j;
	switch (move_direction){
	case 'w': // 向上移动,x坐标减1，y坐标不变，就是向上
		new_head_i = old_head_i - 1;
		break;
	case 's': //向下移动 
		new_head_i = old_head_i + 1;
		break;
	case 'a': //向左移动
		new_head_j = old_head_j - 1;
		break;
	case 'd': //向右移动
		new_head_j = old_head_j + 1;
		break;
	}

	// 如果蛇头超出边界，或者碰到蛇身，游戏失败
	if ((new_head_i >= HEIGHT) || (new_head_i < 0) || (new_head_j >= WIDTH) || (new_head_j < 0)
		|| (blocks[new_head_i][new_head_j] > 0)) {
		is_failure = 1; //游戏失败
		return;
	}
	// 新蛇头位置数值为1
	blocks[new_head_i][new_head_j] = 1;
	// 如果新蛇头正好碰到食物,重新随机食物位置，-5是为了不让食物靠近边界，这里不对旧蛇尾处理，相当于蛇的长度+1
	if ((new_head_i == food_i) && (new_head_j == food_j)) {
		food_i = random(HEIGHT - 5);
		food_j = random(WIDTH - 5);
		length++;
	}
	else {
		//没有碰到食物
		blocks[old_tail_i][old_tail_j] = 0;
	}
}

void update_with_input() {
	if (kbhit()) {
		char input = getch();
		switch (input) {
		case 'w':
		case 'd':
		case 's':
		case 'a':
			move_direction = input;
		}
	}
}
void update_without_input() {
	//与输入无关的更新函数
	if (is_failure)
		return;
	static int wait_index = 1;
	wait_index++;

	if (wait_index == 10) { //每10帧 执行一次，用这个控制小蛇移动速度 
		move_snake();
		wait_index = 1;
	}
}

//绘心形函数  x0,y0,位置  size:大小   COLORREF C：颜色
void heart(int x0, int y0, int size){
	double  m, n, x, y; double i;
	for (i = 0; i <= 2 * size; i = i + 0.01)
	{
		//产生极坐标点
		m = i;
		n = -size * (((sin(i)*sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
		//转换为笛卡尔坐标
		x = n * cos(m) + x0;
		y = n * sin(m) + y0;
		putpixel(x, y, RED);
	}
}

int main2()
{
	//创建宽高为640, 480的窗口
	initgraph(WIDTH*BLOCK_SIZE, HEIGHT*BLOCK_SIZE);
	setbkcolor(EGERGB(255,245,238));

	// 圆
	setcolor(EGERGB(199,21,133));
	circle(320, 240, 200);

	// 椭圆
	setcolor(EGERGB(0x0, 0x0, 0xFF));
	setfillcolor(EGERGB(0xFF, 0xA5, 0x0));
	fillellipse(320, 280, 100, 60);

	// 两个圆
	setfillcolor(EGERGB(0,0,0));
	circle(290, 280, 10);
	circle(350, 280, 10);

	setfillcolor(EGERGB(189, 183, 107));
	bar(220, 100, 270, 140);
	bar(380, 100, 430, 140);

	setlinewidth(5);
	setcolor(EGERGB(0xFF, 0x0, 0x0));
	arc(320, 240, 200, 280, 150);

	setcolor(EGERGB(255, 0, 0));
	setfont(35, 0, "宋体");
	xyprintf(540, 150, "HOW ARE YOU?");
	xyprintf(540, 200, "ARE YOU OK?");
	xyprintf(540, 250, "ARE YOU YEADY?");
	xyprintf(240,450, "Press enter to start!");

	return 0;
}

void getZoomImage(PIMAGE pimg, const char* fileName, int width, int height)
{

	PIMAGE temp = newimage();
	getimage(temp, fileName);

	if (getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);

	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
}
int main(){
	MUSIC music;
	music.OpenFile("aa.mp3");
	music.Play(14000);//播放音乐
	// 初始化函数
	main2();
	getch();

	start_up();
	while (is_run()) {
		show();
		if (is_failure) {
			cleardevice();
			//如果游戏失败
			setcolor(EGERGB(255, 0, 0));
			setfont(80, 0, "宋体");
			xyprintf(240, 220, "GAME OVER");
			xyprintf(240, 310, "length: %d", length+1);
			xyprintf(240, 390, "CONTINUE???");
			char inputs = getch();
			if (inputs == 'y' || inputs == 'Y') {
				cleardevice();
				blocks[HEIGHT][WIDTH] = { 0 };
				is_failure = 0; // 是否游戏失败
				length = 4;
				getch();
				for (int i = 0; i < HEIGHT; i++) {
					for (int j = 0; j < WIDTH; j++) {
						if (blocks[i][j] > 0)  // 是小蛇身体
							blocks[i][j]=0;						
					}
				}
				start_up();
				is_run();
			}
			else {

				music.OpenFile("thankyou.mp3");
				music.Play(14000);//播放音乐
				cleardevice();
				initgraph(600, 600);
				////创建图片对象
				PIMAGE pimg = newimage();
				//从文件中获取图像（和原图一样大小）
				getimage(pimg, "a5.jpg");
				putimage(0, 0, pimg);

				heart(250, 250, 60);
				heart(250, 250, 70);
				heart(250, 250, 80);
				heart(250, 250, 90);
				heart(250, 250, 100);
				getch();
				//关闭音乐
				music.Close();
				// 关闭画布
				closegraph();
			}
		}
		update_without_input();
		update_with_input();
		delay_fps(60);
	}
	return 0;

}
