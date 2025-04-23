#define FPS 200000
#include<random>
#include<iostream>

#ifdef _WIN32
#include<conio.h>
#include<windows.h>

#else
#include<termios.h>
#include<sys/ioctl.h>
#include<unistd.h>
int _kbhit(){
	//get the fd of stdin
	static bool inited = false;
	int STDIN = fileno(stdin);
	termios attr;
	//set new tty and save original attribution
	if(!inited){
		tcgetattr(STDIN, &attr);
		attr.c_lflag &= ~ICANON;
		attr.c_lflag &= ~ECHO;
		tcsetattr(STDIN, TCSANOW, &attr);
	}
	//get bytes in waiting
	int bytes=0;
	ioctl(0, FIONREAD, &bytes);
	return bytes;
}

char getch(){
	int STDIN= 0;//fileno(stdin);
	termios attr;
	tcgetattr(STDIN, &attr);
	attr.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN, TCSANOW, &attr);
	attr.c_lflag &= (ICANON | ECHO);
	char a=getchar();
	tcflush(STDIN, TCIOFLUSH);
	tcsetattr(STDIN, TCSANOW, &attr);
	return a;
}

#endif

using namespace std;

const int width=42;
const int height=16;

int x_offset=0, y_offset=0;

void setloc(int x, int y){
	for(int i=0; i<x; i++){
		cout<<"\033[C";
	}
	for(int i=0; i<y; i++){
		cout<<"\033[A";
	}
	x_offset=x, y_offset=y;
	cout.flush();
}

void resetloc(int char_num = 1){
	while(char_num){
		cout<<"\033[D";
		char_num--;
	}
	while(x_offset){
		cout<<"\033[D";
		x_offset--;
	}
	while(y_offset){
		cout<<"\033[B";
		y_offset--;
	}
	cout.flush();
}

class snake
{
	int tail_remain[2];

public:
	int x[100], y[100];
	int length=2;
	char dir='w';
	snake()
	{
		x[0]=width/2, y[0]=height/2;
		x[1]=width/2, y[1]=height/2-1;
		tail_remain[0]=x[length-1], tail_remain[1]=y[length-1];
		setloc(x[0], y[0]);
		cout<<"@";
		resetloc();
		setloc(x[1], y[1]);
		cout<<"@";
		resetloc();
	}

	void move(){
		tail_remain[0]=x[length-1], tail_remain[1]=y[length-1];	
		for(int i=length-1; i>0; i--){
			x[i]=x[i-1];
			y[i]=y[i-1];
		}
		switch(dir){
			case 'w':
				y[0]++;
				break;
			case 's':
				y[0]--;
				break;
			case 'a':
				x[0]--;
				break;
			case 'd':
				x[0]++;
				break;
		}
		draw();
	}


	void eat(){
		length++;
		x[length-1]=tail_remain[0], y[length-1]=tail_remain[1];
	}
private:
	void draw(){	
		setloc(tail_remain[0], tail_remain[1]);
		cout<<" ";
		resetloc();
		for(int i=0; i<length; i++){
			setloc(x[0], y[0]);
			cout<<"@";
			resetloc();
		}
	}
};

class bonus
{
public:
	int x, y;

	bonus(): x(1), y(height - 1){;} 

	void spawn(){
		static mt19937 rand1;
		static mt19937 rand2;
		static uniform_int_distribution<int> x_pos(1, width-2);
		static uniform_int_distribution<int> y_pos(1, height-2);
		x=x_pos(rand1);
		y=y_pos(rand2);
		setloc(x, y);
		cout<<"O";
		resetloc();
	}
};



int main(){
//game area
	printf("\033[?25l");
	int score=0;
	cout<<"Press 0 (zero) to exit"<<endl<<"score: "<<score<<endl;
	for(int i=0; i<width; i++){
		cout<<"#";
	}
	cout<<endl;
	for(int j=0; j<height-2; j++){
		cout<<"#";
		for(int i=0; i<width-2; i++){
			cout<<" ";
		}
		cout<<"#";
		cout<<endl;
	}
	for(int i=0; i<width; i++){
		cout<<"#";
	}
	cout<<"\r";


//snake spawn
	snake snake;

//bonus spawn
	bonus bonus;
	bonus.spawn();

//snake move
	while(snake.dir!='0'){
		if(_kbhit()){
			snake.dir=getch();
		}
		snake.move();
		if(snake.x[0]==bonus.x && snake.y[0]==bonus.y){
			score++;
			snake.eat();
			setloc(7, height);
			cout<<score;
			int t=0, temp=score;
			while(temp){
				temp=temp/10;
				t++;
			}
			resetloc(t);
			bonus.spawn();
		}
#ifdef _WIN32
		Sleep(FPS/10000);
#else
		usleep(FPS);
#endif
	}
	printf("\033[?25h");
/***************LOCATE*************************
	setloc(0, 0);
	cout<<"$";
	resetloc();

	setloc(0, height-1);
	cout<<"$";
	resetloc();

	setloc(width-1, 0);
	cout<<"$";
	resetloc();

	setloc(width-1, height-1);
	cout<<"$";
	resetloc();
	
	cout.flush();
	usleep(5000000);
******************DEBUG***********************/
}
