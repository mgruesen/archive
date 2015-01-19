#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <random>

struct vec3 { GLdouble x,y,z; };

const GLfloat ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat diffuse_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_position[] = {0.0f, 0.0f, 0.0f};

GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.0f, 0.5f };
GLfloat mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat high_shininess[] = { 0.0f };

GLfloat turn_rotation = 0.0;
GLfloat wave_rotation = 0.0;
GLfloat walk_rotation = 0.0;

struct vec3 transform = {0.0,0.0,0.0};
struct vec3 eyeColor = {255,255,255};

GLdouble red = 1.0, grn = 1.0, blu = 1.0;
const GLfloat deg2rad = 3.1415926 / 180.0;

enum Type { SPEC, DIFF, SHINE };

//from Andre Stein - http://steinsoft.net/index.php?site=Programming/Code%20Snippets/OpenGL/no6
void drawEllipse(float x, float y){
	glBegin(GL_POLYGON);
	for(int i = 0; i < 360; i++)
		glVertex2f(cos(deg2rad * i) * x, sin(deg2rad * i) * y);
	glEnd();
}

void drawCircle(float rad){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	const float twoPi = 3.1415926 * (float)2;
	for(int i = 0; i <= 360; i++){
		float x = rad * cosf((float)i * twoPi / 360);
		float y = rad * sinf((float)i * twoPi / 360);
		glVertex2f(x, y);
	}
	glEnd();
}

void idle(){
	glutPostRedisplay();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//outer
	glPushMatrix();
	
	//ball
	glPushMatrix();
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0 + transform.x, 0.72 + transform.y, 0 + transform.z);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	//antenna
	glPushMatrix();
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0 + transform.x, 0.7 + transform.y, 0 + transform.z);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* antquad = gluNewQuadric();
	gluCylinder(antquad, 0.01, 0.02, 0.2, 25,25);
	glPopMatrix();

	//head
	glPushMatrix();
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0 + transform.x, 0.5 + transform.y, 0 + transform.z);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* hquad = gluNewQuadric();	
	gluCylinder(hquad, 0.08, 0.08, 0.3, 15, 15);
	glPopMatrix();

	//eye socket
	glPushMatrix();
	glColor3d(0,0,0);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0 + transform.x, 0.4 + transform.y, -0.075 + transform.z);	
	drawEllipse(0.075, 0.05);
	glPopMatrix();

	//left eye
	glPushMatrix();
	glColor3d(eyeColor.x, eyeColor.y, eyeColor.z);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(-0.03 + transform.x, 0.4 + transform.y, -0.08 + transform.z);
	drawCircle(0.02);
	glPopMatrix();

	//right eye
	glPushMatrix();
	glColor3d(eyeColor.x, eyeColor.y, eyeColor.z);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0.03 + transform.x, 0.4 + transform.y, -0.08 + transform.z);
	drawCircle(0.02);
	glPopMatrix();

	//body
	glPushMatrix();	
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0 + transform.x, 0.2 + transform.y, 0 + transform.z);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* bquad = gluNewQuadric();
	gluCylinder(bquad, 0.15, 0.15, 0.75, 32, 32);	
	glPopMatrix();

	//left shoulder
	glPushMatrix();
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(-0.175 + transform.x, 0.021 + transform.y, 0.0 + transform.z);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glColor3d(red, grn, blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(-0.175 + transform.x, 0.0 + transform.y, 0.001 + transform.z);
	//wave rotation
	glRotatef(-wave_rotation, 0.0, 0.0, 1.0);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* laquad = gluNewQuadric();	
	gluCylinder(laquad, 0.03, 0.03, 0.5, 32, 32);
	glPopMatrix();

	//right shoulder
	glPushMatrix();
	glColor3d(red,grn,blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0.175 + transform.x, 0.021 + transform.y, 0.0 + transform.z);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glColor3d(red, grn, blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0.175 + transform.x, 0.0 + transform.y, 0.0 + transform.z);
	//wave rotation
	glRotatef(wave_rotation, 0.0, 0.0, 1.0);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* raquad = gluNewQuadric();
	gluCylinder(raquad, 0.03, 0.03, 0.5, 32, 32);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glColor3d(red, grn, blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(-0.05 + transform.x, -0.55 + transform.y, 0.0 + transform.z);
	//walk rotation
	glRotatef(walk_rotation, 1.0, 0.0, 0.0);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* llquad = gluNewQuadric();
	gluCylinder(llquad, 0.03, 0.03, 0.4, 32, 32);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glColor3d(red, grn, blu);
	//turn rotation
	glRotatef(turn_rotation, 0.0, 1.0, 0.0);
	//translation
	glTranslated(0.05 + transform.x, -0.55 + transform.y, 0.0 + transform.z);
	//walk rotation
	glRotatef(-walk_rotation, 1.0, 0.0, 0.0);
	//orientation
	glRotatef(90, 1, 0, 0);
	GLUquadric* rlquad = gluNewQuadric();
	gluCylinder(rlquad, 0.03, 0.03, 0.4, 32, 32);
	glPopMatrix();

	//end outer
	glPopMatrix();
	glutSwapBuffers();
}

void increase(Type t){	
	bool redisplay = true;
	
	switch(t){
	case SPEC:
		{
			for(int i = 0; i < 3; i++){
				GLfloat val = mat_specular[i];
				mat_specular[i] = val + 0.1 > 1.0f ? 0.0 : val + 0.1;
			}
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			break;
		}
	case DIFF:
		{
			for(int i = 0; i < 3; i++){
				GLfloat val = mat_diffuse[i];
				mat_diffuse[i] = val + 0.1 > 1.0f ? 0.0 : val + 0.1;
			}
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			break;
		}
	case SHINE:
		{
			GLfloat val = high_shininess[0];
			high_shininess[0] = val + 10.0 > 100.0f ? 0.0 : val + 10.0;
			glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
			break;
		}
	default:
		redisplay = false;
		break;		
	}
	
	if (redisplay) glutPostRedisplay();
}

void doJump(int val){
	glutPostRedisplay();
	GLdouble v = val;
	if (val){//jump up
		if (transform.y < v){
			transform.y += 0.1;
			glutTimerFunc(50,doJump,1);
		}
		else {
			glutTimerFunc(50,doJump,0);
		}
	}
	else{//jump down
		if (transform.y > v){
			transform.y -= 0.1;
			glutTimerFunc(50,doJump,0);
		}
		else
			transform.x = transform.y = transform.z = 0.0;
	}
}

void doTurnAround(int angle){
	if ((turn_rotation += angle) < 360.0){		
		glutPostRedisplay();
		glutTimerFunc(80, doTurnAround, 5);
	}
	else{
		turn_rotation = 0.0;
	}
}

void doColorChange(){	
	switch (rand() % 3){
	case 0:
		if (red > 0) red -= 0.1;
		else red = 1.0;
		break;
	case 1:
		if (grn > 0) grn -= 0.1;
		else grn = 1.0;
		break;
	case 2:
		if (blu > 0) blu -= 0.1;
		else blu = 1.0;
		break;
	default:break;
	}
	glutPostRedisplay();
}

void doWave(int n){
	if (n){
		if (wave_rotation < 180.0){
			wave_rotation += 5.0;
			glutTimerFunc(50, doWave, 1);
		}
		else
			glutTimerFunc(50, doWave, 0);
	}
	else {
		if (wave_rotation > 0){
			wave_rotation -= 5.0;
			glutTimerFunc(50, doWave, 0);
		}
		else{
			wave_rotation = 0;
		}
	}
	glutPostRedisplay();
}

void doWalk(int n){
	if (n == 1){
		if(walk_rotation < 45.0){
			walk_rotation += 5.0;
			glutTimerFunc(50, doWalk, 1);
		}
		else {
			glutTimerFunc(50, doWalk, 0);
		}
	}
	else if (n == 0){
		if (walk_rotation > -45.0){
			walk_rotation -= 5.0;
			glutTimerFunc(50, doWalk, 0);
		}
		else
			glutTimerFunc(50, doWalk, -1);
	}
	else {
		if(walk_rotation < 0.0){
			walk_rotation += 5.0;
			glutTimerFunc(50, doWalk, -1);
		}
		else{
			walk_rotation = 0.0;
		}
	}
	glutPostRedisplay();
}

void doBlink(int n){
	if (n){
		eyeColor.x = eyeColor.y = eyeColor.z = 0;
		glutPostRedisplay();
		glutTimerFunc(2000, doBlink, 0);
	}
	else {
		eyeColor.x = eyeColor.y = eyeColor.z = 255;
		glutPostRedisplay();
	}
	
}

void keyPress(unsigned char key, int x, int y){
	switch (key){
	case 'b'://blink
		doBlink(1);
		break;
	case 'c'://change color
		doColorChange();
		break;
	case 'd'://inc diffusive refl (+0.1 reset > 1)
		increase(DIFF);		
		break;
	case 'h'://inc shininess (+10 reset > 100)
		increase(SHINE);
		break;
	case 'j'://jump
		doJump(1);
		break;
	case 'q'://quit
		glutDestroyWindow(glutGetWindow());
		exit(0);
		break;
	case 'r'://run		
		doWalk(1);
		break;
	case 's'://inc specular refl (+0.1 reset > 1)
		increase(SPEC);
		break;
	case 't'://turn around
		glutTimerFunc(50, doTurnAround, 10);
		break;
	case 'w'://wave arms
		doWave(1);
		break;
	default://idle
		break;
	}
}

int main(int argc, char** argv){
	//init window
	glutInit(&argc, argv);
	glutInitWindowSize(800,500);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("myWindow");
	
	//setup callbacks
	glutKeyboardFunc(keyPress);
	glutIdleFunc(idle);
	glutDisplayFunc(display);

	//setup lighting and such
	glClearColor(1,1,1,1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	/*glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	//start loop
	glutMainLoop();
	return 0;	
}