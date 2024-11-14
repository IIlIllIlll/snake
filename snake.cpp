#include<random>
#include<iostream>
#include<unistd.h>
#include<conio.h>
using namespace std;

const int width=42;
const int height=16;

int x_offset=0, y_offset=0;

void setloc(int x, int y){
	for(int i=0; i<x; i++){
		cout<<"\033[C";
		x_offset++;
	}
	for(int i=0; i<y; i++){
		cout<<"\033[A";
		y_offset++;
	}
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
}


class snake
{
	int x_init=width/2, y_init=height/2;//head initial coordinate

public:
	int lenth=3;//initial lenth
	int x[20000]={x_init, x_init, x_init}, y[20000]={y_init, y_init-1, y_init-2};
	bool eat=0;
	char dir='w';

	
	snake(){
		setloc(x_init, y_init);
		cout<<"@";//draw head
		resetloc();

		setloc(x_init, y_init-1);
		cout<<"@";//draw initial body
		resetloc();
		
		setloc(x_init, y_init-2);
		cout<<"@";//draw butt
		resetloc();
	}

	
	void move(){
		if(!eat){
			setloc(x[lenth-1], y[lenth-1]);
			cout<<" ";
			resetloc();
		}
		else lenth++;

		switch(dir){
			case 'w': 
				setloc(x[0], y[0]+1);
				cout<<"@";
				resetloc();
				for(int i=lenth-1; i>0; i--){
					x[i]=x[i-1];
					y[i]=y[i-1];
				};
				y[0]++;
				break;

			case 'a': 
				setloc(x[0]-1, y[0]);
				cout<<"@";
				resetloc();
				for(int i=lenth-1; i>0; i--){
					x[i]=x[i-1];
					y[i]=y[i-1];
				};
				x[0]--;
				break;

			case 's':
				setloc(x[0], y[0]-1);
				cout<<"@";
				resetloc();
				for(int i=lenth-1; i>0; i--){
					x[i]=x[i-1];
					y[i]=y[i-1];
				}
				y[0]--;
				break;

			case 'd':
				setloc(x[0]+1, y[0]);
				cout<<"@";
				resetloc();
				for(int i=lenth-1; i>0; i--){
					x[i]=x[i-1];
					y[i]=y[i-1];
				}
				x[0]++;
				break;
		}
	}
};



class bonus
{
public:
	void spawn(int x, int y){
		setloc(x, y);
		cout<<"o";
		resetloc();
	}
};



int main(){
//game area
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
	mt19937 rand1;
	mt19937 rand2;
	uniform_int_distribution<int> x_pos(1, width-2);
	uniform_int_distribution<int> y_pos(1, height-2);
	int x=x_pos(rand1);
	int y=y_pos(rand2);
	bonus.spawn(x, y);

//snake move
	while(snake.dir!='0'){
		if(_kbhit()){
			snake.dir=getch();
		}
		snake.move();
		snake.eat=0;
		if(snake.x[0]==x && snake.y[0]==y){
			snake.eat=1;
			score++;
			setloc(7, height);
			cout<<score;
			int t=0, temp=score;
			while(temp){
				temp=temp/10;
				t++;
			}
			resetloc(t);
			x=x_pos(rand1);
			y=y_pos(rand2);
			bonus.spawn(x, y);
		}
		usleep(400000);
	}
/***************DEBUG*************************
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

	sleep(5);
******************DEBUG********************/
}
