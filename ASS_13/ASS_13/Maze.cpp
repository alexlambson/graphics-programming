#include "Maze.h"
using namespace std;
Maze::Cell::Cell(){
	this->walls.left = true;
	this->walls.top = true;
	this->walls.bottom = true;
	this->walls.right = true;
	visited = false;
	this->r = rand() % 256;
	this->g = rand() % 256;
	this->b = rand() % 256;
}
Maze::Maze(){
	this->createMaze(4, 4);
	this->setMazeSize(4, 4);
}
Maze::Maze(int sizeX, int sizeY){
	this->createMaze(sizeX, sizeY);
	this->setMazeSize(sizeX, sizeY);
}
void Maze::setMazeSize(int sizeX, int sizeY){
	this->mazeSize.sizeX = sizeX;
	this->mazeSize.sizeY = sizeY;
}
MazeSize Maze::getMazeSize(){
	return this->mazeSize;
}
void Maze::createMaze(int x, int y){
	this->mCells.resize(x);
	for (int i = 0; i < x; i++){
		this->mCells[i].resize(y);
		for (int j = 0; j < y; j++){
			this->mCells[i][j] = Cell();
			this->mCells[i][j].setPosition(i, j);
			this->mCells[i][j].visited = false;
		}
	}
	this->setMazeSize(x, y);
	this->removeWalls(x/2, y/2);
	int end = rand() % x;
	this->mCells[0][0].walls.bottom = false;
	this->mCells[end][y - 1].walls.top = false;
	this->rat = Rat(0.5, 0.5);
}
void Maze::Draw(GLuint * texName){
	int xSize = this->mCells.size();

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2d(-.5, -.5);
	glTexCoord2f(10, 0); glVertex2d(this->getMazeSize().sizeX + .5, -.5);
	glTexCoord2f(10, 10); glVertex2d(this->getMazeSize().sizeX + .5, this->getMazeSize().sizeY + .5);
	glTexCoord2f(0, 10); glVertex2d(-.5, this->getMazeSize().sizeY + .5);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < xSize; i++){
		int ySize = this->mCells[i].size();
		for (int j = 0; j < ySize; j++){
			this->mCells[i][j].Draw(this->mazeSize.sizeX, this->mazeSize.sizeY, texName);
		}
	}
	this->rat.Draw();
}
void Maze::Cell::Draw(int mazeX, int mazeY, GLuint * texName){

	int x, y;
	x = this->position.xPos;
	y = this->position.yPos;
	if (Globals::current_view == top_view){
		glBegin(GL_LINES);

		if (this->walls.bottom){
			glVertex2i(x, y);
			glVertex2i(x + 1, y);
		}
		if (this->walls.right){
			glVertex2i(x + 1, y);
			glVertex2i(x + 1, y + 1);
		}
		if (this->walls.top){
			glVertex2i(x + 1, y + 1);
			glVertex2i(x, y + 1);
		}
		if (this->walls.left){
			glVertex2i(x, y + 1);
			glVertex2i(x, y);
		}
		glEnd();
	}
	if (Globals::current_view == perspective_view || Globals::current_view == rat_view){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[1]);
		glBegin(GL_QUADS);
		if (this->walls.top && this->position.yPos == mazeY - 1) {
			glTexCoord2d(0, 0); glVertex3f(x, y + 1, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y + 1, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y + 1, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y + 1, 1);
		}
		if (this->walls.bottom) {
			glTexCoord2d(0, 0); glVertex3f(x, y, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y, 1);
		}
		if (this->walls.right && this->position.xPos == mazeX - 1) {
			glTexCoord2d(0, 0); glVertex3f(x + 1, y + 1, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y, 1);
			glTexCoord2d(0, 1); glVertex3f(x + 1, y + 1, 1);
		}
		if (this->walls.left) {
			glTexCoord2d(0, 0); glVertex3f(x, y, 0);
			glTexCoord2d(1, 0); glVertex3f(x, y + 1, 0);
			glTexCoord2d(1, 1); glVertex3f(x, y + 1, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y, 1);
		}
		glEnd(); 
		glDisable(GL_TEXTURE_2D);
	}
}
void Maze::Cell::setPosition(int x, int y){
	this->position.xPos = x;
	this->position.yPos = y;
}
void Maze::removeWalls(int x, int y){
	Cell * me = &this->mCells[x][y];
	me->visited = true;
	Unvisited unvisited = this->unvisitedNeighbors(x, y);
	while (unvisited.unvisitedNeighbor){
		int randomPick = rand() % unvisited.uPos.size();
		Position nextCell = unvisited.uPos[randomPick];
		Cell * other = &this->mCells[nextCell.xPos][nextCell.yPos];

		if (x < nextCell.xPos){
			me->walls.right = false;
			other->walls.left = false;
		}
		if (x > nextCell.xPos){
			me->walls.left = false;
			other->walls.right = false;
		}
		if (y < nextCell.yPos){
			me->walls.top = false;
			other->walls.bottom = false;
		}
		if (y > nextCell.yPos){
			me->walls.bottom = false;
			other->walls.top = false;
		}
		this->removeWalls(nextCell.xPos, nextCell.yPos);
		unvisited = this->unvisitedNeighbors(x, y);
	}
	return;
}
Unvisited Maze::unvisitedNeighbors(int myX, int myY){
	bool beginX = false, beginY = false, endX = false, endY = false;
	int mazeSizeX, mazeSizeY;
	mazeSizeX = this->getMazeSize().sizeX;
	mazeSizeY = this->getMazeSize().sizeY;
	Unvisited unvisited;

	if (myX + 1 >= mazeSizeX){
		endX = true;
	}
	if (myX - 1 < 0){
		beginX = true;
	}
	if (myY + 1 >= mazeSizeY){
		endY = true;
	}
	if (myY - 1 < 0){
		beginY = true;
	}
	if (!beginX && !this->mCells[myX - 1][myY].visited){
		unvisited.Push(myX - 1, myY);
	}
	if (!endX && !this->mCells[myX + 1][myY].visited){
		unvisited.Push(myX + 1, myY);
	}
	if (!beginY && !this->mCells[myX][myY - 1].visited){
		unvisited.Push(myX, myY - 1);
	}
	if (!endY && !this->mCells[myX][myY + 1].visited){
		unvisited.Push(myX, myY + 1);
	}
	return unvisited;
}
CellWalls Maze::GetCellWalls(Position position){
	if (position.xPos >= this->mCells.size() || position.yPos >= this->mCells[0].size() || position.xPos < 0 || position.yPos < 0){
		CellWalls walls;
		walls.bottom = false; walls.left = false; walls.right = false; walls.top = false;
		return walls;
	}
	return this->mCells[position.xPos][position.yPos].walls;
}
