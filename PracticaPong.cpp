#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PI 3.1415926535898 

double xpos_ball, ypos_ball, ydir_ball, xdir_ball;
float xvel_ball = 1.5;
float yvel_ball = 1.5;
double xpos_j1, ypos_j1, xpos_j2, ypos_j2; //posiciones de las paletas
double vel_paddle = 10.0;
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation 
int SPEED = 20;        // speed of timer call back in msecs
bool inicio = true;
int score_j1 = 0;
int score_j2 = 0;
int score = 0;
int WINDOW_HEIGHT = 720;
int WINDOW_WIDTH = 800;
double ball_up, ball_down, ball_left, ball_right; //
float j1_up, j1_down, j1_right, j2_up, j2_down, j2_left;

GLfloat T1[16] = { 1.,0.,0.,0.,\
				  0.,1.,0.,0.,\
				  0.,0.,1.,0.,\
				  0.,0.,0.,1. };
GLfloat S[16] = { 1.,0.,0.,0.,\
				 0.,1.,0.,0.,\
				 0.,0.,1.,0.,\
				 0.,0.,0.,1. };
GLfloat T[16] = { 1.,0.,0.,0.,\
				 0., 1., 0., 0.,\
				 0.,0.,1.,0.,\
				 0.,0.,0.,1. };



#define PI 3.1415926535898 
GLint circle_points = 100;
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius) {
	GLint i;
	GLdouble angle;
	glBegin(GL_POLYGON);
	for (i = 0; i < circle_points; i++) {
		angle = 2 * PI * i / circle_points;
		glVertex2f(centerx + radius * cos(angle), centery + radius * sin(angle));
	}
	glEnd();
}

GLfloat RadiusOfBall = 5.;
// Draw the ball, centered at the origin
void draw_ball() {
	glColor3f(0.6, 0.3, 0.);
	MyCircle2f(0., 0., RadiusOfBall);
}

void initial_position_paddle() {
	xpos_j1 = 5;
	ypos_j1 = 60;
	xpos_j2 = 155;
	ypos_j2 = 60;
}

void initial_position_ball() {
	xpos_ball = 80.;
	ypos_ball = 50.;
	int diry = rand() % 2;
	if (diry == 0) {
		diry = -1;
	}
	if (!inicio) {
		if (score == 1) {
			
			yvel_ball *= diry;
			xvel_ball *= -1;
		}
		else {
			
			yvel_ball *= diry;
			xvel_ball *= -1;
		}
	}
	else {
		ydir_ball = diry;
		xdir_ball = 1;
	}
}

void update_ball_hitbox() {
	/*	Función para actualizar la hitbox cuadrada de la pelota
	*/
	ball_up = ypos_ball + RadiusOfBall;
	ball_down = ypos_ball - RadiusOfBall;
	ball_left = xpos_ball - RadiusOfBall;
	ball_right = xpos_ball + RadiusOfBall;
}

void update_paddle_hitbox() {
	/*	Función para actualizar la hitbox rectangular de las paletas
	*/
	j1_down = ypos_j1 - 15;
	j1_up = ypos_j1 + 15;
	j1_right = xpos_j1 + 2.5f;
	j2_down = ypos_j2 - 15;
	j2_up = ypos_j2 + 15;
	j2_left = xpos_j2 - 2.5f;
}

void draw_paddle(float x, float y) {
	/* Función para dibujar una paleta en las coordenadas dadas
	* @param x, y coordenadas donde dibujar la paleta
	*/
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2f(x - 2.5f, y - 15);
	glVertex2f(x + 2.5f, y - 15);
	glVertex2f(x + 2.5f, y + 15);
	glVertex2f(x - 2.5f, y + 15);
	glEnd();
}

void reset_positions() {
	/*	Función para resetear las posiciones de las paletas y la pelota a su punto de inicio en el juego.
	*/
	printf("Jugador 1: %d | Jugador 2:  %d \n", score_j1, score_j2);
	initial_position_ball();
	initial_position_paddle();
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(SPEED, timer, 0);
}

void Display(void)
{
	draw_paddle(xpos_j1, ypos_j1);
	draw_paddle(xpos_j2, ypos_j2);
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);

	ypos_ball += yvel_ball;
	xpos_ball += xvel_ball;
	update_ball_hitbox();
	update_paddle_hitbox();

	if (ypos_ball + RadiusOfBall >= 120 || ypos_ball - RadiusOfBall <= 0) {
		yvel_ball *= -1;
		ydir_ball *= -1;
	}

	if (xpos_ball + RadiusOfBall >= 160) {
		score = 1;
		score_j1 += 1;
		reset_positions();
	}

	if (xpos_ball - RadiusOfBall <= 0){
		score = 2;
		score_j2 += 1;
		reset_positions();
	}

	if (ball_left <= j1_right &&
		ball_up >= j1_down &&
		ball_down <= j1_up) {
		xvel_ball *= -1;
	}

	if (ball_right >= j2_left &&
		ball_up >= j2_down &&
		ball_down <= j2_up) {
		xvel_ball *= -1;
	}

	glPushMatrix();

	//Translate the bouncing ball to its new position
	T[12] = xpos_ball;
	T[13] = ypos_ball;
	glLoadMatrixf(T);

	T1[13] = -RadiusOfBall;
	// Translate ball back to center
	glMultMatrixf(T1);
	S[0] = sx;
	S[5] = sy;
	// Scale the ball about its bottom
	glMultMatrixf(S);
	T1[13] = RadiusOfBall;
	// Translate ball up so bottom is at the origin
	glMultMatrixf(T1);

	draw_ball();
	glPopMatrix();
	
}

void keyboard_input(unsigned char key, int x, int y) {
	/* Función para detectar y procesar la entrada del teclado
	*/
	if (key == 'w' && ypos_j1 + 15 + vel_paddle <= 120) {
		ypos_j1 += vel_paddle;
	}

	if (key == 's' && ypos_j1 - 15 - vel_paddle >= 0) {
		ypos_j1 -= vel_paddle;
	}

	if (key == 'i' && ypos_j2 + 15 + vel_paddle <= 120) {
		ypos_j2 += vel_paddle;
	}

	if (key == 'k' && ypos_j2 - 15 - vel_paddle >= 0) {
		ypos_j2 -= vel_paddle;
	}
}

void reshape(int w, int h)
{
	// on reshape and on startup, keep the viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// keep our logical coordinate system constant
	gluOrtho2D(0.0, 160.0, 0.0, 120.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void init(void) {
	//set the clear color to be white
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// initial position set to 0,0
	xpos_ball = 60; ypos_ball = RadiusOfBall; xdir_ball = 1; ydir_ball = 1;
	sx = 1.; sy = 1.; squash = 0.9;
	rot = 0;
	initial_position_paddle();
	initial_position_ball();
	inicio = false;
}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Pong");
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_input);
	glutTimerFunc(SPEED, timer, 0);
	glutMainLoop();


	return 1;
}