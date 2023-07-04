#include <graphics.h>
#include <math.h>
#include <iostream>
#include <windows.h>
using namespace std;

// �궨�峣��
#define BLOCK_SIZE 20  // ÿ��С���ӵĳ����С
#define WIDTH 40       // �����һ��40��С����
#define HEIGHT 30      // �߶���һ��30��С����

// ȫ�ֱ�������
int blocks[HEIGHT][WIDTH] = { 0 }; // ��ʼ��һ��ȫ0�ľ���������¼���������ɫ��������HSV��ɫ�ռ�
char move_direction; // ��w a s d����С���ƶ����� 
int food_i, food_j;  // ʳ���λ��
int is_failure = 0;  // �Ƿ���Ϸʧ��
int length=4;		// ��¼�ߵĳ���

// �����ʼ������ Ӧ�ð�����ʼʱ�ߵ�λ�ã���ɫ��ʳ���λ�õȵ�
void start_up() {
	// ��ʼ��������С
	initgraph(WIDTH*BLOCK_SIZE, HEIGHT*BLOCK_SIZE);
	randomize(); // ��ʼ�������������
	setrendermode(RENDER_MANUAL);  //����Ϊ���Զ�ˢ�� (ֻ��delay/delay_fps/getch ��ˢ����Ļ�� 
	blocks[HEIGHT / 2][WIDTH / 2] = 1; // �����м仭��ͷ
	for (int i = 1; i <= length; i++) { // ��������4�����飬��ֵ����Ϊ2��3��4��5 
		blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	}
	move_direction = 'd'; // ��ʼ�����ƶ�
	food_i = random(HEIGHT -5) ; // �����ʼ��ʳ��λ��
	food_j = random(WIDTH - 5);
}

//���ƺ��� �ߵĳ��ȣ��ߺ�ʳ���λ�ö���ʱ�̱仯�ģ�����Ҫ����ˢ�»����ϵ����ݣ����벻�ϵ����������
void show() { 
	color_t color;
	cleardevice();// ����

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (blocks[i][j]) {
				// ������ɫ����HSV��ɫ�ռ�ת��ΪRGB��ʽ
				color = HSVtoRGB(blocks[i][j] * 10, 0.8, 1);
			}
			else {
				//��ͨ������ɫ��Ҳ���Ǿ���������Ϊ0����ɫ
				color = EGERGB(150, 150, 150);
			}
			// ��ͬ��䲻ͬ����ɫ
			setfillcolor(color);
			//i����(y)��j����(x)��˫��forѭ���Ͷ������
			bar(j*BLOCK_SIZE, i*BLOCK_SIZE, (j + 1)*BLOCK_SIZE, (i + 1)*BLOCK_SIZE);
		}
	}
	// ʳ��Ĭ����ɫΪ��ɫGREEN
	setfillcolor(GREEN);
	// ʳ��λ��Ϊȫ�ֱ������ڸ����ߵĳ��Ⱥ�λ��ʱ��ʳ��λ��Ҳ��֮����
	bar(food_j*BLOCK_SIZE, food_i*BLOCK_SIZE, (food_j + 1)*BLOCK_SIZE, (food_i + 1)*BLOCK_SIZE);
	// ��ʾ�ߵĳ���
	setcolor(EGERGB(255, 0, 0));
	setfont(20, 0, "����");
	xyprintf(10, 580, "length: %d", length+1);
}

// �ƶ�С�߼���ش����� 
void move_snake() {
	// ����ͷ����β����,i=x���꣬j=y����
	int old_head_i, old_head_j, old_tail_i, old_tail_j;
	int max = 0;
	// �ƶ��ߣ������������з�0����λ�ö���1����ͷ�������ƶ�����ͷ��ǰһ��λ��Ԫ�ؾͱ��1�����һ��Ԫ�ر��0
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			// ��һ���ǽ����з�0λ�ã���������ֵ�� + 1
			if (blocks[i][j] > 0)
				blocks[i][j]++;
			// ����βλ�õ�ֵ��󣬽����ֵ��x,y ��ֵ����β����
			if (max < blocks[i][j]) {
				max = blocks[i][j];
				old_tail_i = i;
				old_tail_j = j;
			}
			// ����ͷλ�õ�ֵΪ2����Ϊһ��ʼ��ͷ����ֵӦ����1����0������1��ͳ���2
			if (2 == blocks[i][j]) {
				old_head_i = i;
				old_head_j = j;
			}
		}
	}

	// �Ƚ�����ͷ��λ�ø�ֵ������ͷ�������߲�ͬ������ƶ����ٸı���ͷ������λ��
	int new_head_i = old_head_i;
	int new_head_j = old_head_j;
	switch (move_direction){
	case 'w': // �����ƶ�,x�����1��y���겻�䣬��������
		new_head_i = old_head_i - 1;
		break;
	case 's': //�����ƶ� 
		new_head_i = old_head_i + 1;
		break;
	case 'a': //�����ƶ�
		new_head_j = old_head_j - 1;
		break;
	case 'd': //�����ƶ�
		new_head_j = old_head_j + 1;
		break;
	}

	// �����ͷ�����߽磬��������������Ϸʧ��
	if ((new_head_i >= HEIGHT) || (new_head_i < 0) || (new_head_j >= WIDTH) || (new_head_j < 0)
		|| (blocks[new_head_i][new_head_j] > 0)) {
		is_failure = 1; //��Ϸʧ��
		return;
	}
	// ����ͷλ����ֵΪ1
	blocks[new_head_i][new_head_j] = 1;
	// �������ͷ��������ʳ��,�������ʳ��λ�ã�-5��Ϊ�˲���ʳ�￿���߽磬���ﲻ�Ծ���β�����൱���ߵĳ���+1
	if ((new_head_i == food_i) && (new_head_j == food_j)) {
		food_i = random(HEIGHT - 5);
		food_j = random(WIDTH - 5);
		length++;
	}
	else {
		//û������ʳ��
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
	//�������޹صĸ��º���
	if (is_failure)
		return;
	static int wait_index = 1;
	wait_index++;

	if (wait_index == 10) { //ÿ10֡ ִ��һ�Σ����������С���ƶ��ٶ� 
		move_snake();
		wait_index = 1;
	}
}

//�����κ���  x0,y0,λ��  size:��С   COLORREF C����ɫ
void heart(int x0, int y0, int size){
	double  m, n, x, y; double i;
	for (i = 0; i <= 2 * size; i = i + 0.01)
	{
		//�����������
		m = i;
		n = -size * (((sin(i)*sqrt(fabs(cos(i)))) / (sin(i) + 1.4142)) - 2 * sin(i) + 2);
		//ת��Ϊ�ѿ�������
		x = n * cos(m) + x0;
		y = n * sin(m) + y0;
		putpixel(x, y, RED);
	}
}

int main2()
{
	//�������Ϊ640, 480�Ĵ���
	initgraph(WIDTH*BLOCK_SIZE, HEIGHT*BLOCK_SIZE);
	setbkcolor(EGERGB(255,245,238));

	// Բ
	setcolor(EGERGB(199,21,133));
	circle(320, 240, 200);

	// ��Բ
	setcolor(EGERGB(0x0, 0x0, 0xFF));
	setfillcolor(EGERGB(0xFF, 0xA5, 0x0));
	fillellipse(320, 280, 100, 60);

	// ����Բ
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
	setfont(35, 0, "����");
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
	music.Play(14000);//��������
	// ��ʼ������
	main2();
	getch();

	start_up();
	while (is_run()) {
		show();
		if (is_failure) {
			cleardevice();
			//�����Ϸʧ��
			setcolor(EGERGB(255, 0, 0));
			setfont(80, 0, "����");
			xyprintf(240, 220, "GAME OVER");
			xyprintf(240, 310, "length: %d", length+1);
			xyprintf(240, 390, "CONTINUE???");
			char inputs = getch();
			if (inputs == 'y' || inputs == 'Y') {
				cleardevice();
				blocks[HEIGHT][WIDTH] = { 0 };
				is_failure = 0; // �Ƿ���Ϸʧ��
				length = 4;
				getch();
				for (int i = 0; i < HEIGHT; i++) {
					for (int j = 0; j < WIDTH; j++) {
						if (blocks[i][j] > 0)  // ��С������
							blocks[i][j]=0;						
					}
				}
				start_up();
				is_run();
			}
			else {

				music.OpenFile("thankyou.mp3");
				music.Play(14000);//��������
				cleardevice();
				initgraph(600, 600);
				////����ͼƬ����
				PIMAGE pimg = newimage();
				//���ļ��л�ȡͼ�񣨺�ԭͼһ����С��
				getimage(pimg, "a5.jpg");
				putimage(0, 0, pimg);

				heart(250, 250, 60);
				heart(250, 250, 70);
				heart(250, 250, 80);
				heart(250, 250, 90);
				heart(250, 250, 100);
				getch();
				//�ر�����
				music.Close();
				// �رջ���
				closegraph();
			}
		}
		update_without_input();
		update_with_input();
		delay_fps(60);
	}
	return 0;

}
