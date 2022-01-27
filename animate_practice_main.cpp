#include <GL\glut.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <cmath>
#define PI 3.1415926

//variable declare
double fps = 200;
double play_rate = 0.01;
int n = 20;
double g = -9.8;
const int ball_num = 2;
double radius = 3.0;
int walls[] = { -50,50,-50,50 };


//function declare
void display(void);
void reshape(GLsizei, GLsizei);
void mytime(int);
double* arr_minus(double*, double*);

using namespace std;



class Ball
{
public:
	double position[2];
	double speed[2];
	double acceleration[2] = { 0.0,0.0 };
	double r = radius;
	void move() {
		position[0] += speed[0];
		position[1] += speed[1];
	}
	
	Ball(double* pos, double* spd, double rd) {
		Ball(pos, spd);
		r = rd;
	}
	Ball(double* pos, double* spd) {
		position[0] = pos[0];
		position[1] = pos[1];
		speed[0] = spd[0];
		speed[1] = spd[1];
	}

	Ball() {
		position[0] = 0.0;
		position[1] = 0.0;
		speed[0] = 0.0;
		speed[1] = 0.0;
	}


	void speed_up() {
		speed[0] += acceleration[0];
		speed[1] += acceleration[1];
	}
	void next_state();
	
};



Ball* balls[ball_num];


void Ball::next_state() {
	
	for (int i = 0; i < ball_num; i++) {
		double dist_sq = pow(balls[i]->position[0] - position[0], 2) + pow(balls[i]->position[1] - position[1], 2);
		if (dist_sq <= 36 && (this!=balls[i])) {
			double tmp[] = { speed[0],speed[1] };
			speed[0] = balls[i]->speed[0];
			speed[1] = balls[i]->speed[1];
			balls[i]->speed[0] = tmp[0];
			balls[i]->speed[0] = tmp[1];
		}
	}



	bool is_change = false;
	if (position[0] <= walls[0]+radius) {
		speed[0] = abs(speed[0]);
		is_change = true;
	}
	else if (position[0] >= walls[1]-radius) {
		speed[0] = -abs(speed[0]);
		is_change = true;
	}

	if (position[1] <= walls[2]+radius) {
		speed[1] = abs(speed[1]);
		is_change = true;
	}
	else if (position[1] >= walls[3]-radius) {
		speed[1] = -abs(speed[1]);
		is_change = true;
	}
	if (!is_change) {
		speed_up();
	}
	move();
}











int main(int argc, char* argv[]) {
	srand(time(NULL));
	int arg1 = 1;
	char* arg2[] = { "hello","" };
	glutInit(&arg1, arg2);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("animate test");

	for (int i = 0; i < ball_num; i++) {
		double* tmp1 = new double[2];
		double* tmp2 = new double[2];
		tmp1[0] = rand() % (walls[1] - walls[0] - 10) + walls[0];
		tmp1[1] = rand() % (walls[4] - walls[3] - 10) + walls[3];
		tmp2[0] = rand() % 200 / 10.0 - 10.0;
		tmp2[1] = rand() % 200 / 10.0 - 10.0;
		balls[i] = new Ball(tmp1, tmp2);
	}





	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1/fps*1000/play_rate, mytime, 1);
	glutMainLoop();
	

	return 0;

}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	for (int j = 0; j < ball_num; j++) {
		//¦bposition¦ì¸m µe¶ê
		glBegin(GL_POLYGON);
		for (int i = 0; i < n; i++) {
			glVertex2f(balls[j]->r * cos(2 * i * PI / n)+balls[j]->position[0], balls[j]->r * sin(2 * i * PI / n)+balls[j]->position[1]);
		}
		glEnd();
	}
	

	glutSwapBuffers();
}



void mytime(int t) {
	for (int i = 0; i < ball_num; i++) {
		balls[i]->next_state();
	}

	glutTimerFunc(1/fps*1000/play_rate, mytime, 0);
	glutPostRedisplay();
}

void reshape(GLsizei w, GLsizei h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(walls[0],walls[1],walls[2],walls[3]);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}


double* arr_minus(double* arr1, double* arr2) {
	double* ans = new double[2];
	ans[0] = arr1[0] - arr2[0];
	ans[1] = arr1[1] - arr2[1];
	return ans;

}