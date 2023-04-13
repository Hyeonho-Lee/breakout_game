#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define	width 			400
#define	height			600
#define	PI				3.1415
#define	polygon_num		50

int	left = 0;
int	bottom = 0;
int	collision_count = 0;
int ball_speed = 30;
int player_speed = 20;
int stage_value = 1;
int dead[27] = {0};
int dead_count = 0;

float	radius1, moving_ball_radius;

typedef struct _Point {
	float	x;
	float	y;
} Point;

Point	player, moving_ball, velocity, enemy;


void init(void) {
	player.x = 150;
	player.y = 50;

	moving_ball_radius = 10.0;
	moving_ball.x = width / 2;
	moving_ball.y = height / 4;

	velocity.x = 0.05 * ball_speed;
	velocity.y = 0.05 * ball_speed;

	collision_count = 1;

	for (int i = 0; i < 28; i++) {
		dead[i] = 1;
	}

	printf("스테이지 1입니다.\n");
}


void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height);
}


void	Modeling_Circle(float radius, Point CC) {
	float	delta;

	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));
	glEnd();
}

void Collision_Detection_Between_Balls() {
	float d_1, d_2, d_3, d_4, d_5;

	float delta_x1 = player.x - moving_ball.x;
	float delta_y1 = player.y - moving_ball.y;

	float delta_x2 = player.x + 25 - moving_ball.x;
	float delta_y2 = player.y - moving_ball.y;

	float delta_x3 = player.x + 50 - moving_ball.x;
	float delta_y3 = player.y - moving_ball.y;

	float delta_x4 = player.x + 75 - moving_ball.x;
	float delta_y4 = player.y - moving_ball.y;

	float delta_x5 = player.x + 100 - moving_ball.x;
	float delta_y5 = player.y - moving_ball.y;

	d_1 = sqrt(delta_x1 * delta_x1 + delta_y1 * delta_y1);
	d_2 = sqrt(delta_x2 * delta_x2 + delta_y2 * delta_y2);
	d_3 = sqrt(delta_x3 * delta_x3 + delta_y3 * delta_y3);
	d_4 = sqrt(delta_x4 * delta_x4 + delta_y4 * delta_y4);
	d_5 = sqrt(delta_x5 * delta_x5 + delta_y5 * delta_y5);

	if(d_1 <= moving_ball_radius + 15 || d_2 <= moving_ball_radius + 15 || d_3 <= moving_ball_radius + 15 || d_4 <= moving_ball_radius + 15 || d_5 <= moving_ball_radius + 15){
		velocity.x *= 1;
		velocity.y *= -1;
	}else {
		velocity.x *= 1;
		velocity.y *= 1;
	}
}

void reset() {
	moving_ball.x = width / 2;
	moving_ball.y = height / 4;
	velocity.x = 0.05 * ball_speed;
	velocity.y = 0.05 * ball_speed;
	printf("GAME OVER!\n");
	stage_value = 1;
	dead_count = 0;
	for (int i = 0; i < 28; i++) {
		dead[i] = 1;
	}
	printf("스테이지 %d입니다.\n", stage_value);
}

void Collision_Detection_to_Walls(void) {

	// 아래쪽 충돌
	if (moving_ball.y - moving_ball_radius < 0 && velocity.y < 0){
		velocity.y *= -1;
		reset();
	}

	// 위쪽 충돌
	if (moving_ball.y + moving_ball_radius > height && velocity.y > 0)
		velocity.y *= -1;

	// 오른쪽 충돌
	if (moving_ball.x + moving_ball_radius > width && velocity.x > 0)
		velocity.x *= -1;

	// 왼쪽 충돌
	if (moving_ball.x + moving_ball_radius < 0 && velocity.x < 0)
		velocity.x *= -1;
}

void Player(int weights, int heights) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(weights, heights);
	glVertex2f(weights + 100, heights);
	glVertex2f(weights + 100, heights + 10);
	glVertex2f(weights, heights + 10);
	glEnd();
}

void Enemy(int x, int y, int name) {
	int range = 20;
	enemy.x = x;
	enemy.y = y;

	if (dead[name] > 0) {
		if(dead[name] == 1)
			glColor3f(1.0, 0.0, 0.0);
		else if(dead[name] == 2)
			glColor3f(1.0, 1.0, 0.0);
		else if(dead[name] == 3)
			glColor3f(1.0, 0.0, 1.0);
		else if(dead[name] == 4)
			glColor3f(0.0, 1.0, 1.0);
		else if(dead[name] >= 5)
			glColor3f(1.0, 1.0, 1.0);
		Modeling_Circle(18, enemy);
	}else if (dead[name] == 0){
		range = 0;
		glColor3f(0.0, 0.0, 0.0);
		Modeling_Circle(0, enemy);
		return;
	}

	float distance;
	float delta_x = x - moving_ball.x;
	float delta_y = y - moving_ball.y;

	distance = sqrt(delta_x * delta_x + delta_y * delta_y);

	if(distance <= moving_ball_radius + range && dead[name] >= 0){
		velocity.x *= 1;
		velocity.y *= -1;
		dead[name] = dead[name] - 1;

		if(dead[name] == 0) {
			dead_count++;
		}
	}
}

void SpawnEnemy() {
	int spawn = 0;
	for (int i = 1; i < 10; i++) {
		for(int j = 1; j < 4; j++) {
			Enemy(i*40, height - (j*40), spawn);
			spawn++;
		}
	}
}

void Stage() {
	SpawnEnemy();

	if (dead_count == 27) {
		dead_count = 0;
		stage_value++;
		for (int i = 0; i < 28; i++) {
			dead[i] = stage_value;
		}
		velocity.x = velocity.x * 1.1;
		velocity.y = velocity.y * 1.1;
		printf("스테이지 %d입니다.\n", stage_value);
	}
}


void RenderScene(void) {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Stage();

	Player(player.x, player.y);

	Collision_Detection_Between_Balls();
	Collision_Detection_to_Walls();

	moving_ball.x += velocity.x;
	moving_ball.y += velocity.y;

	glColor3f(0.0, 0.0, 1.0);
	Modeling_Circle(moving_ball_radius, moving_ball);

	glutSwapBuffers();
	glFlush();
}

void processSpecialKeys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT:
			if (player.x < 0)
				player.x -= 0;
			else
				player.x -= player_speed;
			break;
		case GLUT_KEY_RIGHT:
			if (player.x > width - 90)
				player.x += 0;
			else
				player.x += player_speed;
			break;
	}
}

void main(int argc, char** argv) {
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("TEST");
	init();
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(processSpecialKeys);
	glutIdleFunc(RenderScene);
	glutMainLoop();
}
