// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <time.h>
#include "Structs.h"
#include "Maze.h"
#include "glut.h"


// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
Maze gMaze;
int mazeX = 5;
int mazeY = 4;



//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//globalMaze.Draw();
	gMaze.Draw();
	// Test lines that draw all three shapes and some text.
	// Delete these when you get your code working.
	//glColor3d(0,0,1);
	/*double X[3] = { 20, 50, 100 };
	double Y[3] = { 10, 100, 40 };
	double x = 50;
	double y = 80;

	glBegin(GL_POINTS);
	for(int i = 1; i < 10000000; i++){
		glVertex2d(x, y);
		int r = rand() % 3;
		x = (x + X[r]) / 2.0;
		y = (y + Y[r]) / 2.0;
	}*/
	//glEnd();
	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	CellWalls walls = gMaze.GetCellWalls(gMaze.rat.GetIndex());
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'w':
			gMaze.rat.HandleKey(Key::UP, walls);
			break;
		case 's':
			gMaze.rat.HandleKey(Key::DOWN, walls);
			break;
		case 'a':
			gMaze.rat.HandleKey(Key::LEFT, walls);
			break;
		case 'd':
			gMaze.rat.HandleKey(Key::RIGHT, walls);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	//glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-.5, mazeX + .5, -.5, mazeY + .5);
	//gluOrtho2D(0, mazeX, 0, mazeY);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	gMaze = Maze(mazeX, mazeY);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	srand(time(NULL));
	std::cout << "Enter maze size as 'x y':     ";
	std::cin >> mazeX >> mazeY;
	screen_x = 100 * mazeX;
	screen_y = 100 * mazeY;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
