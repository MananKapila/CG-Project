#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include<string.h>

#define PI 3.14159
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4			
#define MAX_STONES  100      
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 0.5
#define SPACESHIP_SPEED 20

int stoneTranslationSpeed=1;
GLint m_viewport[4];
int x,y;
int i;
int randomStoneIndices[100];
int index;
int Score=0;
int alienLife=100;
int GameLvl= 1;
float mouseX ,mouseY ;				//Cursor coordinates;
float LaserAngle=0 ,stoneAngle =0,lineWidth = 1;
float xOne=0,yOne=0;				//Spaceship coordinates
float xStone[MAX_STONES] ,yStone[MAX_STONES];//coordinates of stones
float xHealthBarStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[MAX_STONES];		//check to see if stone is killed

bool mButtonPressed= false,startGame=false,gameOver=false;		//boolean values to check state of the game
bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;

char highScore[100],ch;
void display();
void StoneGenerate();
void displayText(float x ,float y ,float z ,char *stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z); //pointer to sa
		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]); //Text Style
	}
}

void SetDisplayMode(int modeToDisplay) {
		switch(modeToDisplay){
		case GAME_SCREEN: glClearColor(0, 0, 0, 1);break;
		case MENU_SCREEN : glClearColor(1, 0 , 0, 1);break;
	}
}

void initializeStoneArray() {
	//random stones index
	for(int i = 0;i < MAX_STONES ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true;
	}
	xStone[0] = -(200*MAX_STONES)-600;             //START LINE for stone appearance
												
	for(int i = 0;i<MAX_STONES ;i++) {				//random appearance yIndex for each stone
		yStone[i]=rand()%600;
		if(int(yStone[i])%2)
			yStone[i]*=-1;
		xStone[i+1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}

void DrawBase()
{
	glColor3f(0.85,0.85,0.85);
	glBegin(GL_POLYGON);           //Lazer Base
		glVertex2f(-70 ,20);
		glVertex2f(5 ,20);
		glVertex2f(10, 0);
		glVertex2f(-100 ,0);
	glEnd(); 
	glBegin(GL_POLYGON);           //Ship tail
		glVertex2f(-10,20);
		glVertex2f(5 ,20);
		glVertex2f(5,40);
	glEnd(); 
	glBegin(GL_POLYGON);           //Ship tail
		glVertex2f(-50,0);
		glVertex2f(-18 ,-10);
		glVertex2f(-25,5);
	glEnd();
	
	glColor3f(0,1,1);
	glBegin(GL_POLYGON);           //Screen
		glVertex2f(-85.33 ,10);
		glVertex2f(-70 ,10);
		glVertex2f(-76, 3);
		glVertex2f(-96.66 ,3);
	glEnd(); 
	
	//Outlining
	glColor3f(0,0,0);			//BODY Outline
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);           //Lazer Base
		glVertex2f(5 ,20);
		glVertex2f(10, 0);
	glEnd(); 
	glBegin(GL_LINE_STRIP);           //Ship tail
		glVertex2f(-10,20);
		glVertex2f(5 ,20);
		glVertex2f(5,40);
	glEnd(); 
	glBegin(GL_LINE_STRIP);           //Ship tail
		glVertex2f(-50,0);
		glVertex2f(-18 ,-10);
		glVertex2f(-25,5);
	glEnd();
	glBegin(GL_LINE_STRIP);           //Screen
		glVertex2f(-85.33 ,10);
		glVertex2f(-70 ,10);
		glVertex2f(-76, 3);
		glVertex2f(-96.66 ,3);
	glEnd(); 
}
void DrawSpaceShipLazer() {
	
	glColor3f(0.85, 0.85, 0.85);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
		glVertex2f(-55 ,20);
		glVertex2f(-55 ,30);
		glVertex2f(-50, 30);
		glVertex2f(-50 ,20);
	glEnd();
	
	float xMid =0,yMid =0;
	//Mid point of the lazer horizontal 	
	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;
	
	//Rotating about the point ,20
	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	glTranslated(xMid , -yMid ,0);

	//find mid point of top of lazer stem
	float midPoint = -(55+50)/2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
		glVertex2f(midPoint + 10 ,25);
		glVertex2f(midPoint + 10 ,35);
		glVertex2f(midPoint - 10 ,35);
		glVertex2f(midPoint - 10 ,25);
	glEnd();

	glPopMatrix();
}
void DrawLazerBeam() {

	float xMid = -(55+50)/2.0;
	float yMid = (25+35)/2.0;
	
	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5);   //----Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(xMid ,yMid);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

    	glTranslated(xStone[index] , yStone[index] ,0);
    	glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35,35,1);
		glutSolidSphere(1,9,50);
		
		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,10,1);
		glutSolidSphere(1,9,50);
		
		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,60,1);
		glutSolidSphere(1,9,50);
		break;

	case 1:
		glColor3f(0.4f, 0.4f, 0.4f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15,20,1);
		glutSolidSphere(1,9,50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,25,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,60,1);
		glutSolidSphere(1,9,50);
		
		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35,10,1);
		glutSolidSphere(1,10,7);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50,20,1);
		glutSolidSphere(1,5,50);
		break;
	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,55,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20,10,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle+45 ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,10,1);
		glutSolidSphere(1,9,50);
		break;
	}
	glPopMatrix();
}
bool checkIfSpaceShipIsSafe() {
		for(int i =0 ;i<MAX_STONES ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -10 ) && yOne <= (yStone[i]/2 + 43)) || (yOne <= (yStone[i]/2 - 10) && yOne >= (yStone[i]/2 - 50) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{	
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate(){	
	glPushMatrix();
	glTranslated(xOne,yOne,0);
	if(!checkIfSpaceShipIsSafe() && alienLife ){
		alienLife-=10;
		xHealthBarStart -= 230;
	}
	glPushMatrix();
	glTranslated(4,19,0);
	glPopMatrix();
    DrawBase();
	DrawSpaceShipLazer();
	if(mButtonPressed) {
		DrawLazerBeam();
	}
	glEnd(); 
	glPopMatrix();
}
void DisplayHealthBar() {
	
	glColor3f(1 ,0 ,0);
	glBegin(GL_POLYGON);
		glVertex2f(-xHealthBarStart ,700);
		glVertex2f(1200 ,700);
		glVertex2f(1200 ,670);
		glVertex2f(-xHealthBarStart, 670);
	glEnd();
	char temp[40];
	glColor3f(0 ,0 ,1);
	sprintf(temp,"SCORE = %d",Score);
	displayText(-1100 ,600 ,0.4 ,temp);//<---display variable score ?
	sprintf(temp,"  LIFE = %d",alienLife);
	displayText(800 ,600 ,0.4 ,temp);
	sprintf(temp,"  LEVEL : %d",GameLvl);
	displayText(-100 ,600 ,0.4 ,temp);
	glColor3f(1 ,0 ,0);
}
void startScreenDisplay() // edit
{	
	glLineWidth(15);
	SetDisplayMode(MENU_SCREEN);
	
	displayText(-300 ,540 ,0.0 ,"ASTEROID ASSASSINATOR");
	displayText(-300 ,523 ,0.0 ,"________________________");
	
	//Team members names
	displayText(700 ,-100 ,0.0 ,"Group Members");
	displayText(700 ,-113 ,0.0 ,"_____________");
	displayText(700 ,-173 ,0.0 ,"101803460 Ashray");
	displayText(700 ,-223 ,0.0 ,"101803654 Manan");
    displayText(700 ,-273 ,0.0 ,"101803660 Jaskaran");
	
	glColor3f(0,0,0);
	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-600 ,-400 ,0.5);
		glVertex3f(-600 ,450 ,0.5);
		glVertex3f(600 ,450 ,0.5);
		glVertex3f(600 ,-400, 0.5);
	glEnd();
	
	glLineWidth(1);

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
		glVertex3f(-200 ,300 ,0.5);
		glVertex3f(-200 ,400 ,0.5);
		glVertex3f(200 ,400 ,0.5);
		glVertex3f(200 ,300, 0.5);
	glEnd();
	
	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();

	if(mouseX>=-100 && mouseX<=100 && mouseY>=150 && mouseY<=200){
		glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			startGame = true ;
			gameOver = false;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);

	displayText(-100 ,340 ,0.4 ,"Start Game");
	
	if(mouseX>=-100 && mouseX<=100 && mouseY>=30 && mouseY<=80) {
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			instructionsGame = true ;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);
	displayText(-120 ,80 ,0.4 ,"Instructions");
	
	if(mouseX>=-100 && mouseX<=100 && mouseY>=-90 && mouseY<=-40){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			gameQuit = true ;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayText(-100 ,-170 ,0.4 ,"    Quit");
	
}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2 ,0);
	if(alienLife){
		SpaceshipCreate();
	}
	else {
		gameOver=true;
		instructionsGame = false;
		startScreen = false;
	}								//<----------------------gameover screen

	StoneGenerate();
	
}
void readFromFile() {

	FILE *fp = fopen("HighScoreFile.txt" ,"r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch,sizeof(char),1 ,fp)){
			highScore[i++] = ch;
		}
		highScore[i] = '\0';
	} 
	fclose(fp);
}
void writeIntoFile() {						//To write high score on to file
	FILE *fp = fopen("HighScoreFile.txt" ,"w");
	int i=0;
	char temp[40];
	if(fp!= NULL){
		int n= Score;
		while(n){
			ch = (n%10)+ '0';
			n/=10;
			temp[i++] = ch;
		}
		temp[i] = '\0';
		strrev(temp);
		puts(temp);
		if(temp[0] == '\0')
			temp[i++] = '0' ,temp[i++] = '\0';
		fwrite(temp ,sizeof(char)*i ,i ,fp);
	}
 fclose(fp);
}
void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0,0,0);
	glLineWidth(50);
	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-650 ,-500 ,0.5);
		glVertex3f(-650 ,520 ,0.5);
		glVertex3f(650 ,520 ,0.5);
		glVertex3f(650 ,-500, 0.5);
	glEnd();
	
	glLineWidth(1);
	stoneTranslationSpeed=1;
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);				//GAME OVER
		glVertex3f(-550 ,810,0.5);
		glVertex3f(-550 ,610 ,0.5);
		glVertex3f(550 ,610 ,0.5);
		glVertex3f(550 ,810, 0.5);
	glEnd();
	
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();
		

	displayText(-300 ,640 ,0.4 ,"G A M E    O V E R ! ! !");
	glColor3f(0 , 0, 0);
	char temp[40];
	
	sprintf(temp,"Score : %d",Score);
	displayText(-100 ,340 ,0.4 ,temp);
	readFromFile();
	char temp2[40];
	if(atoi(highScore) < Score){
		writeIntoFile();
		sprintf(temp2 ,"Highest Score :%d" ,Score);
	} else 
		sprintf(temp2 ,"Highest Score :%s" ,highScore);

	displayText(-250 ,400 ,0.4 ,temp2);
		
	if(mouseX>=-100 && mouseX<=100 && mouseY>=25 && mouseY<=75){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){                                                       //Reset game default values
			startGame = true ;
			gameOver=false;
			mButtonPressed = false;
			initializeStoneArray();
			alienLife=100;	
			xHealthBarStart=1200;
			Score=0;
			GameLvl=1;
			GameScreenDisplay();
		}
	} else
		glColor3f(0 , 0, 0);
	displayText(-70 ,80 ,0.4 ,"Restart");
		
	if(mouseX>=-100 && mouseX<=100 && mouseY>=-100 && mouseY<=-50){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			exit(0);
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayText(-100 ,-170 ,0.4 ,"    Quit");
	
}
void StoneGenerate(){

		if(xStone[0]>=1200){      //If the last screen hits the end of screen then go to Nxt lvl
			GameLvl++;
			stoneTranslationSpeed+=2;
			Score+=50;
			initializeStoneArray();
			GameScreenDisplay();
		}

	for(int i=0; i<MAX_STONES ;i++){
		index = i;
		
		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mButtonPressed){
			if(stoneAlive[i]){   // IF ALIVE KILL STONE
				stoneAlive[i]=0;
				Score++;
				if(Score%1==0) {
					stoneTranslationSpeed+=0.5;			//<--------------Rate of increase of game speed
				}							
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )             //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle+=stoneRotationSpeed;
	if(stoneAngle > 360) stoneAngle = 0;
}
void backButton() {
	if(mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250){
			glColor3f(0, 0, 1);
			if(mButtonPressed){
				mButtonPressed = false;
				instructionsGame = false;
				startScreenDisplay();
			}
	}
	else glColor3f(0, 0, 0);
	displayText(-1000 ,-550 ,0, "Back");
}
void InstructionsScreenDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0, 0, 0);
	displayText(-900 ,400 ,0.4 ,"Key 'w' to move up.");
	displayText(-900 ,300 ,0.4 ,"Key 's' to move down.");
	displayText(-900 ,200 ,0.4 ,"Key 'd' to move right.");
	displayText(-900 ,100 ,0.4 ,"Key 'a' to move left.");
	displayText(-900 ,0.0 ,0.4 ,"Left mouse click to shoot laser");
	displayText(-900 ,-200 ,0.4 ,"You Get 1 point for shooting each objet and 50 points for completing each lvl ");
	displayText(-900, -270,0.4,"The Objective is to score maximum points");
	backButton();
	if(previousScreen)
		nextScreen = false ,previousScreen = false; //as set by backButton()
}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);   
	glViewport(0,0,1200,700);

	if(startGame && !gameOver)
		GameScreenDisplay();

	else if(instructionsGame)
		InstructionsScreenDisplay();

	else if(gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if(startScreen){

			startScreenDisplay();
			if(gameQuit || startGame || optionsGame || instructionsGame){
				//startScreen = false;
				
				if(startGame){
					SetDisplayMode(GAME_SCREEN);
					startScreen = false;
					
				} else if(gameQuit) 
					exit(0);

				} else if(instructionsGame) {
					SetDisplayMode(GAME_SCREEN);
					InstructionsScreenDisplay();
				} 
		}

	//Reset Scaling values
	glScalef(1/2 ,1/2 ,0);
	glFlush();  
	glLoadIdentity();
	glutSwapBuffers();
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan/mouseXForTan);
	LaserAngle = (180/PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{
	if(key == 'd') xOne+=SPACESHIP_SPEED; 
	if(key == 'a') xOne-=SPACESHIP_SPEED; 
	if(key == 'w') {yOne+=SPACESHIP_SPEED;}
	if(key == 's') {yOne-=SPACESHIP_SPEED;}
	if(key == 'd' || key == 'a' || key == 'w' || key == 's')
		somethingMovedRecalculateLaserAngle();
			
	display();	
}
void myinit()
{
	glClearColor(0.5,0.5,0.5,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(-1200,1200,-700,700);                   //<-----CHANGE THIS TO GET EXTRA SPACE
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);
	//Do calculations to find value of LaserAngle
	somethingMovedRecalculateLaserAngle();
	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {
	
	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else 
		mButtonPressed = false;
	display();
}
void idleCallBack() {			//when no mouse or keybord pressed
	 display();
 }
int main(int argc, char** argv) {
	
	 FILE *fp = fopen("HighScoreFile.txt" ,"r") ;      //check if HighScoreFile.txt exist if not create             
	 if(fp!=NULL)
		fclose(fp);
	 else
		 writeIntoFile(); 
		 
	glutInit(&argc, argv);    
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("THE SPACESHIP SHOOTING GAME");  
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);  
	glutPassiveMotionFunc(passiveMotionFunc); //mouse ki position update 
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA); //pixel airthematic
	glutIdleFunc(idleCallBack); 
	glutMouseFunc(mouseClick); //mouse movements
	glGetIntegerv(GL_VIEWPORT ,m_viewport); //normalize device coordinates to window coordinates
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
 }
