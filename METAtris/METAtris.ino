//imports the Gamebuino library and the gb object


/*
** INFO
*/

/*
Save block 0 to 4 is for top 5 high scores

5 to 9 is for the name. 3 letters
*/


/*
** CONSTANT VARIABLES, DEFINITIONS AND INCLUDES
*/
#include <Gamebuino-Meta.h>

#define playFieldWidth 10
#define playFieldHeight 20

//playarea offset from top left
#define playFieldStartX 2
#define playFieldStartY 2

#define dropSoundPitch 200
#define moveSoundPitch 1000
#define rotateSoundPitch 500

extern const byte font5x7[];


const Color PROGMEM blockColors[] = {
LIGHTBLUE,
DARKBLUE,
ORANGE,
YELLOW,
LIGHTGREEN,
PINK,
RED,
};

//colors of the level selector. Used for rainbow effects
const Color PROGMEM levelColors[] = {
LIGHTBLUE,
BLUE,
DARKBLUE,
GREEN,
LIGHTGREEN,
YELLOW,
ORANGE,
RED,
PINK,
PURPLE,
};

//all rotations for all blocks. First letter is the block and second is rotation. displayedBlock points to one of these
bool I0[] = {
0,0,0,0,0,
0,0,0,0,0,
0,1,1,1,1,
0,0,0,0,0,
0,0,0,0,0,
};
bool IR[] = {
0,0,0,0,0,
0,0,1,0,0,
0,0,1,0,0,
0,0,1,0,0,
0,0,1,0,0,
};
bool I2[] = {
0,0,0,0,0,
0,0,0,0,0,
1,1,1,1,0,
0,0,0,0,0,
0,0,0,0,0,
};
bool IL[] = {
0,0,1,0,0,
0,0,1,0,0,
0,0,1,0,0,
0,0,1,0,0,
0,0,0,0,0,
};

bool J0[] = {
1,0,0,
1,1,1,
0,0,0,
};
bool JR[] = {
0,1,1,
0,1,0,
0,1,0,
};
bool J2[] = {
0,0,0,
1,1,1,
0,0,1,
};
bool JL[] = {
0,1,0,
0,1,0,
1,1,0,
};

bool L0[] = {
0,0,1,
1,1,1,
0,0,0,
};
bool LR[] = {
0,1,0,
0,1,0,
0,1,1,
};
bool L2[] = {
0,0,0,
1,1,1,
1,0,0,
};
bool LL[] = {
1,1,0,
0,1,0,
0,1,0,
};

bool O0[] = {
0,1,1,
0,1,1,
0,0,0,
};
bool OR[] = {
0,0,0,
0,1,1,
0,1,1,
};
bool O2[] = {
0,0,0,
1,1,0,
1,1,0,
};
bool OL[] = {
1,1,0,
1,1,0,
0,0,0,
};

bool S0[] = {
0,1,1,
1,1,0,
0,0,0,
};
bool SR[] = {
0,1,0,
0,1,1,
0,0,1,
};
bool S2[] = {
0,0,0,
0,1,1,
1,1,0,
};
bool SL[] = {
1,0,0,
1,1,0,
0,1,0,
};

bool T0[] = {
0,1,0,
1,1,1,
0,0,0,
};
bool TR[] = {
0,1,0,
0,1,1,
0,1,0,
};
bool T2[] = {
0,0,0,
1,1,1,
0,1,0,
};
bool TL[] = {
0,1,0,
1,1,0,
0,1,0,
};

bool Z0[] = {
1,1,0,
0,1,1,
0,0,0,
};
bool ZR[] = {
0,0,1,
0,1,1,
0,1,0,
};
bool Z2[] = {
0,0,0,
1,1,0,
0,1,1,
};
bool ZL[] = {
0,1,0,
1,1,0,
1,0,0,
};

//array of all above blocks and rotation states
bool *blockLayouts[] = {
I0,
IR,
I2,
IL,
J0,
JR,
J2,
JL,
L0,
LR,
L2,
LL,
O0,
OR,
O2,
OL,
S0,
SR,
S2,
SL,
T0,
TR,
T2,
TL,
Z0,
ZR,
Z2,
ZL,
};

//used to set speed in levels 0...18
const unsigned char PROGMEM speedArray[19] = {
20,
18,
16,
14,
12,
10,
9,
8,
7,
6,
5,
5,
5,
4,
4,
4,
3,
3,
3,
};

//array of tests to make before rotating block to figure out if position needs to change also
//odd numbers are X values and even are Y
//there are 5 tests for every rotation so up to 10 values need to be checked
const int PROGMEM wallKickCW[] = {
0,0, -1,0, -1,+1, 0,-2, -1,-2,	//0 -> 1 or 0 -> R
0,0, +1,0, +1,-1, 0,+2, +1,+2,	//1 -> 2 or R -> 2
0,0, +1,0, +1,+1, 0,-2, +1,-2,	//2 -> 3 or 2 -> L
0,0, -1,0, -1,-1, 0,+2, -1,+2,	//3 -> 0 or L -> 0
};

//for I block
const int PROGMEM wallKickICW[] = {
0,0, -2,0, +1,0, -2,-1, +1, +2,	//0 -> 1 or 0 -> R
0,0, -1,0, +2,0, -1,+2, +2,-1,	//1 -> 2 or R -> 2
0,0, +2,0, -1,0, +2,+1, -1,-2,	//2 -> 3 or 2 -> L
0,0, +1,0, -2,0, +1,-2, -2,+1,	//3 -> 0 or L -> 0
};




//Images converted with Image to code converter by gouz. Set reminder to give credit

//these blocks fill screen after game over
const uint8_t gameoverBlockData[] = {4, 3, 1, 0, 1, 0x00, 1, 0x76, 0x50,0x66, 0x50,0x55, 0x50};
Image gameoverBlock = Image(gameoverBlockData);

//menu difficulty selector panel
const uint8_t difficulty_barData[] = {54, 6, 1, 0, 1, 0xFF, 1, 0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x77, 0x77,0x70, 0x70,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x60,0x70, 0xcc,0xcc, 0x0d,0xdd, 0xd0,0x11, 0x11,0x03, 0x33,0x30, 0xbb,0xbb, 0x0a,0xaa, 0xa0,0x99, 0x99,0x08, 0x88,0x80, 0xee,0xee, 0x02,0x22, 0x20,0x60, 0x70,0xcc, 0xcc,0x0d, 0xdd,0xd0, 0x11,0x11, 0x03,0x33, 0x30,0xbb, 0xbb,0x0a, 0xaa,0xa0, 0x99,0x99, 0x08,0x88, 0x80,0xee, 0xee,0x02, 0x22,0x20, 0x60,0x70, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x60, 0x76,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x66,0x66, 0x60};
Image difficultyBar = Image(difficulty_barData);

//graphic for bottom of menu
const uint8_t bottomGraphicData[] = {80, 12, 1, 0, 1, 0xFF, 1, 
0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x88, 0x80,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x88,0x80, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x80, 0x80,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x80,0x80, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x88, 0x80,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x88,0x80, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x11, 0x10,0x00, 0xee,0xe0, 0x00,0x99, 0x99,0x99, 0x99,0x98, 0x88,0x88, 0x80,0x00, 0x11,0x10, 0x00,0xee, 0xe0,0x00, 0x99,0x99, 0x99,0x99, 0x98,0x88, 0x88,0x80, 0x00,0x11, 0x10,0x00, 0xee,0xe0, 0x00,0x99, 0x99,0x99, 0x99,0x10, 0x10,0x00, 0xe0,0xe0, 0x00,0x90, 0x99,0x09, 0x90,0x98, 0x08,0x80, 0x80,0x00, 0x10,0x10, 0x00,0xe0, 0xe0,0x00, 0x90,0x99, 0x09,0x90, 0x98,0x08, 0x80,0x80, 0x00,0x10, 0x10,0x00, 0xe0,0xe0, 0x00,0x90, 0x99,0x09, 0x90,0x11, 0x10,0x00, 0xee,0xe0, 0x00,0x99, 0x99,0x99, 0x99,0x98, 0x88,0x88, 0x80,0x00, 0x11,0x10, 0x00,0xee, 0xe0,0x00, 0x99,0x99, 0x99,0x99, 0x98,0x88, 0x88,0x80, 0x00,0x11, 0x10,0x00, 0xee,0xe0, 0x00,0x99, 0x99,0x99, 0x99,0x11, 0x1e,0xee, 0xee,0xee, 0xee,0x99, 0x9a,0xaa, 0xaa,0xa8, 0x88,0xbb, 0xbb,0xbb, 0x11,0x1e, 0xee,0xee, 0xee,0xee, 0x99,0x9a, 0xaa,0xaa, 0xa8,0x88, 0xbb,0xbb, 0xbb,0x11, 0x1e,0xee, 0xee,0xee, 0xee,0x99, 0x9a,0xaa, 0xaa,0x10, 0x1e,0x0e, 0xe0,0xee, 0x0e,0x90, 0x9a,0x0a, 0xa0,0xa8, 0x08,0xb0, 0xbb,0x0b, 0x10,0x1e, 0x0e,0xe0, 0xee,0x0e, 0x90,0x9a, 0x0a,0xa0, 0xa8,0x08, 0xb0,0xbb, 0x0b,0x10, 0x1e,0x0e, 0xe0,0xee, 0x0e,0x90, 0x9a,0x0a, 0xa0,0x11, 0x1e,0xee, 0xee,0xee, 0xee,0x99, 0x9a,0xaa, 0xaa,0xa8, 0x88,0xbb, 0xbb,0xbb, 0x11,0x1e, 0xee,0xee, 0xee,0xee, 0x99,0x9a, 0xaa,0xaa, 0xa8,0x88, 0xbb,0xbb, 0xbb,0x11, 0x1e,0xee, 0xee,0xee, 0xee,0x99, 0x9a,0xaa, 0xaa,0x11, 0x1c,0xcc, 0xcc,0xcc, 0xcc,0xcc, 0xca,0xaa, 0xaa,0xab, 0xbb,0xbb, 0xb1,0x11, 0x11,0x1c, 0xcc,0xcc, 0xcc,0xcc, 0xcc,0xca, 0xaa,0xaa, 0xab,0xbb, 0xbb,0xb1, 0x11,0x11, 0x1c,0xcc, 0xcc,0xcc, 0xcc,0xcc, 0xca,0xaa, 0xaa,0x10, 0x1c,0x0c, 0xc0,0xcc, 0x0c,0xc0, 0xca,0x0a, 0xa0,0xab, 0x0b,0xb0, 0xb1,0x01, 0x10,0x1c, 0x0c,0xc0, 0xcc,0x0c, 0xc0,0xca, 0x0a,0xa0, 0xab,0x0b, 0xb0,0xb1, 0x01,0x10, 0x1c,0x0c, 0xc0,0xcc, 0x0c,0xc0, 0xca,0x0a, 0xa0,0x11, 0x1c,0xcc, 0xcc,0xcc, 0xcc,0xcc, 0xca,0xaa, 0xaa,0xab, 0xbb,0xbb, 0xb1,0x11, 0x11,0x1c, 0xcc,0xcc, 0xcc,0xcc, 0xcc,0xca, 0xaa,0xaa, 0xab,0xbb, 0xbb,0xb1, 0x11,0x11, 0x1c,0xcc, 0xcc,0xcc, 0xcc,0xcc, 0xca,0xaa, 0xaa};
Image bottomGraphic = Image(bottomGraphicData);

//selector 
const uint8_t selectorData[] = {6, 4, 1, 0, 1, 0x00, 1, 0x44, 0x44,0x44, 0x40,0x00, 0x07,0x40, 0x00,0x07, 0x77,0x77, 0x77};
Image selector = Image(selectorData);

//bg tile
const uint8_t bgTileData[] = {2, 2, 1, 0, 1, 0xFF, 1, 0x65, 0x50};
Image bgTile = Image(bgTileData);

//text "Game Over"
const uint8_t gameoverTextData[] = {18, 14, 1, 0, 1, 0x0a, 1, 0xaa, 0x00,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xa0,0x77, 0x0a,0x00, 0xa0,0xa0, 0xaa,0x0a, 0xaa,0x07, 0x00,0xa0, 0x77,0x07, 0x07,0x00, 0x70,0xaa, 0x07,0x07, 0x07,0x07, 0x07,0x77, 0x07,0x07, 0x0a,0x07, 0x07,0x07, 0x07,0x07, 0x07,0x07, 0x70,0xaa, 0xa0,0x77, 0x00,0x77, 0x07,0x07, 0x00,0x77, 0x0a,0xaa, 0x00,0xaa, 0x00,0xa0, 0xa0,0xaa, 0x00,0xaa, 0xa0,0x70, 0xa0,0xa0, 0xaa,0x0a, 0xa0,0xa0, 0xaa,0x07, 0x07,0x07, 0x07,0x00, 0x70,0x07, 0x07,0x0a, 0x07,0x07, 0x07,0x07, 0x07,0x07, 0x07,0x70, 0xaa,0x07, 0x07,0x07, 0x07,0x07, 0x70,0x07, 0x0a,0xaa, 0xa0,0x70, 0xa0,0x70, 0xa0,0x77, 0x07,0x0a, 0xaa,0xaa, 0x0a,0xaa, 0x0a,0xaa, 0x00,0xa0, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa};
Image gameoverText = Image(gameoverTextData);

//text "Press A"
const uint8_t pressAData[] = {30, 8, 1, 0, 1, 0x0a, 1, 0xa0, 0x0a,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0x0a,0xaa, 0x07,0x70, 0xa0,0xa0, 0xaa,0x0a, 0xaa,0x00, 0xaa,0x00, 0xaa,0xaa, 0xa0,0x70, 0xaa,0x07, 0x07,0x07, 0x07,0x00, 0x70,0xa0, 0x77,0x00, 0x77,0x0a, 0xaa,0x07, 0x07,0x0a, 0x07,0x07, 0x07,0x70, 0x07,0x07, 0x07,0x70, 0x07,0x70, 0xaa,0xaa, 0x07,0x77, 0x0a,0x07, 0x70,0x07, 0x0a,0x07, 0x70,0xa0, 0x07,0x00, 0x07,0x0a, 0xaa,0x07, 0x07,0x0a, 0x07,0x0a, 0x07,0x0a, 0xa0,0x77, 0x07,0x70, 0x07,0x70, 0xaa,0xaa, 0x07,0x07, 0x0a,0xa0, 0xaa,0xa0, 0xaa,0xaa, 0x00,0xa0, 0x0a,0xa0, 0x0a,0xaa, 0xaa,0xa0, 0xa0,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa,0xaa, 0xaa};
Image pressA = Image(pressAData);


/*
** VARIABLE VARIABLES
*/

//contains all placed blocks. 10x20

unsigned char playField[] = {
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
};

unsigned char speed = 20;	//frames until block moves down. Value is updated when level goes up

//variables for currently falling block
//falling block position on playfield. Set by newBlock function
int blockX;	//0...9
int blockY;	//0...19

unsigned char block;	//block type. Value from 0...6 in order I J L O S T Z
unsigned char rotationState;	//0...3 for all possible rotations

//position of blocks ghost. Set by updateGhost function
int ghostBlockX;
int ghostBlockY;

bool *displayedBlock;	//point to the falling block shape. 
bool *nextDisplayedBlock;	//checkPosition uses this pointer when checking for collision. Set it before calling the function

unsigned char nextBlock;	//this value is moved to the block variable when the currently falling block hits bottom

bool *pointNextBlock;	//points to the next blocks shape like displayedBlock 

//stats kept while playing
unsigned char rowsCleared;	//how many rows were cleared at the same time. 0...4
unsigned int allRowsCleared;	//all rows cleared during the whole game
unsigned char internalRows;	//hidden row counter that resets back to 0 after passing 10. Used during fast level progression

unsigned long score;

//High scores loaded here on boot
unsigned long highScores[]{
0,
0,
0,
0,
0,
};
//names loaded here on boot
char names[][4] = {
"AAA",
"AAA",
"AAA",
"AAA",
"AAA",
};

//if 0, no high score broken. If larger than 0, that number high score was broken. 1 best. 5 last
unsigned char newHighScore = 0;

bool gameover;	//switches to game over state

//set by showMenu() function
unsigned char level = 0;
unsigned char startLevel = 0;


/*
set to 1 if down is held after block is placed. Don't move block down manually
unless BUTTON_DOWN has been released and held again.
*/
bool downReleasedAfterPlace = 1;


/*
** FUNCTIONS
*/

//moves value of nextBlock to block and randomly picks next block, sets block coordinates, sets pointer pointNextBlock
void newBlock(){
	//check if new block can spawn
	if(playField[3] > 0 || playField[4] > 0 || playField[5] > 0 || playField[6] > 0){
		gameover = 1;
		return;
	}
	//make sure there are never three same blocks back to back
	if(nextBlock == block){
		block = nextBlock;
		nextBlock = (nextBlock + random(1,7)) % 7;
	}
	else{
		block = nextBlock;
		nextBlock = random(0,7);
	}
	
	//position starting from top left. Moves 3 pixels per step
	blockX = 3;
	blockY = 0;

	if(block == 0){
		blockX = 2;
		blockY = -2;
	}
	switch (nextBlock){
		case 0:
			pointNextBlock = I2;
			break;
		case 1:
			pointNextBlock = J0;
			break;
		case 2:
			pointNextBlock = L0;
			break;
		case 3:
			pointNextBlock = O0;
			break;
		case 4:
			pointNextBlock = S0;
			break;
		case 5:
			pointNextBlock = T0;
			break;
		case 6:
			pointNextBlock = Z0;
			break;
	}	

	rotationState = 0;
	displayedBlock = blockLayouts[(block * 4) + rotationState];
	updateGhost();

	
	if(gb.buttons.repeat(BUTTON_DOWN, 0)){
		downReleasedAfterPlace = 0;
	}
	else{
		downReleasedAfterPlace = 1;
	}
}


//places block in playField[] array, always runs checkRow() and newBlock() functions
void placeBlock(){
	if(block == 0){
		for(int x = 0; x < 5; x++){
			for(int y = 0; y < 5; y++){
				if(displayedBlock[x + (y * 5)] != 0){
					playField[blockX + x + (y * playFieldWidth) + (blockY * playFieldWidth)] = block + 1;
				}
			}
		}
	}
	else{
		for(int x = 0; x < 3; x++){
			for(int y = 0; y < 3; y++){
				if(displayedBlock[x + (y * 3)] != 0){
					playField[blockX + x + (y * playFieldWidth) + (blockY * playFieldWidth)] = block + 1;
				}
			}
		}
	}
	checkRow();
	newBlock();
	gb.sound.tone(dropSoundPitch,50);
}


/*
checks if block can move and moves it to the specified direction
left l, right r, down d
usage:
move('l');
move('r');
move('d');
returns 0 if block is placed by placeBlock() function
else returns 1
*/
bool move(char direction){
	switch (direction){
		case 'd':
			nextDisplayedBlock = displayedBlock;
			if(checkPosition(blockX, blockY + 1) == 1){
				blockY++;
			}
			else{
				placeBlock();
				return 0;
			}
			break;
			
		case 'l':
			nextDisplayedBlock = displayedBlock;
			if(checkPosition(blockX - 1, blockY) == 1){
				blockX--;
				gb.sound.tone(moveSoundPitch, 50);
			}
			break;
			
		case 'r':
			nextDisplayedBlock = displayedBlock;
			if(checkPosition(blockX + 1, blockY) == 1){
				blockX++;
				gb.sound.tone(moveSoundPitch, 50);
			}
			break;
	}
	return 1;
}



//check rows and if it finds full rows, run clearRow on that row
void checkRow(){
	for(int y = 0; y < playFieldHeight; y++){
		for(int x = 0; x < playFieldWidth; x++){
			if(playField[x + (y * playFieldWidth)] == 0){
				break;
			}
			//if didn't break; , clear the row. This method should probably be changed even though it works
			if(x == 9){
				clearRow(y);
				addRows();
			}
		}
	}

}

//moves all rows above given row down, effectively clearing the given row
void clearRow(unsigned char row){
	
	for(int j = row; j > 0; j--){
	
		for(int i = 0; i < playFieldWidth; i++){
			playField[i + (j * playFieldWidth)] = playField[i + ((j - 1) * playFieldWidth)];
		}
	}
	
}

//check if nextDisplayedBlock can exist in the position and returns 1 when possible
bool checkPosition(int checkBlockX, int checkBlockY){

	//checks every piece of block if it interferes with a piece on the playfield
	if(block == 0){
		for(int x = 0; x < 5; x++){
			for(int y = 0; y < 5; y++){
				if(nextDisplayedBlock[x + (y * 5)] != 0){
					if(checkBlockX + x < 0 || checkBlockX + x > 9 || checkBlockY + y > 19){
						return 0;
					}
					if(playField[checkBlockX + x + (checkBlockY * playFieldWidth) + (y * playFieldWidth)] > 0 || x + checkBlockX > 9){
						return 0;
					}
				}
			}
		}
	}
	else{
		for(int x = 0; x < 3; x++){
			for(int y = 0; y < 3; y++){
				if(nextDisplayedBlock[x + (y * 3)] != 0){
					if(checkBlockX + x < 0 || checkBlockX + x > 9 || checkBlockY + y > 19){
						return 0;
					}
				
					if(playField[checkBlockX + x + (checkBlockY * playFieldWidth) + (y * playFieldWidth)] > 0 || x + checkBlockX > 9){
						return 0;
					}
				}
			}
		}
	}
	
	return 1;
	
}

void rotateCW(){
	nextDisplayedBlock = blockLayouts[(block * 4) + ((rotationState + 1) % 4)];
	int x;
	int y;
	
	//check all five possible wall kicks
	for(int i = 0; i < 5; i++){
		if(block == 0){
			x = wallKickICW[(rotationState * 10) + (i * 2)];
			y = wallKickICW[((rotationState * 10) + (i * 2)) + 1];
		}
		else{
			x = wallKickCW[(rotationState * 10) + (i * 2)];
			y = wallKickCW[((rotationState * 10) + (i * 2)) + 1];
		}

		if(checkPosition(blockX + x, blockY + y) == 1){
			displayedBlock = nextDisplayedBlock;
			blockX = blockX + x;
			blockY = blockY + y;
			gb.sound.tone(rotateSoundPitch, 50);
			rotationState = (rotationState + 1) % 4;
			updateGhost();
			return;
		}
		
	}
	
}


//run move('d') until block is down and add 2 score for every space the block traveled.
void instaDrop(){
	while(1){
		if(move('d') != 0){
			score = score + 2;
		}
		else{
			return;
		}
	}
}

//updates position and rotation of ghost block
void updateGhost(){

	ghostBlockX = blockX;
	ghostBlockY = blockY;
	
	while(1){
		if(block == 0){
			for(int x = 0; x < 5; x++){
				for(int y = 0; y < 5; y++){
					if(displayedBlock[x + (y * 5)] != 0){
						if(playField[ghostBlockX + x + playFieldWidth + (ghostBlockY * playFieldWidth) + (y * playFieldWidth)] > 0 || y + ghostBlockY >= 19){
							return;
						}
					}
				}
			}

		}
		else{
			for(int x = 0; x < 3; x++){
				for(int y = 0; y < 3; y++){
					if(displayedBlock[x + (y * 3)] != 0){
						if(playField[ghostBlockX + x + playFieldWidth + (ghostBlockY * playFieldWidth) + (y * playFieldWidth)] > 0 || y + ghostBlockY >= 19){
							return;
						}
					}
				}
			}

		}
		ghostBlockY++;
	}

}

//add values to counters
void addRows(){
	rowsCleared++;
	allRowsCleared++;
	internalRows++;
}


//set speed variable based on level
void setSpeed(){

	if(level < 19){
		speed = speedArray[level];
	}
	else if(level > 18 && level < 29){
		speed = 2;
	}
	else{
		speed = 1;
	}

}

void lightTest(unsigned char number, Color pixelColour){
	switch(number){
		case 0:
			gb.lights.drawPixel(1,0,pixelColour);
			break;
		case 1:
			gb.lights.drawPixel(1,1,pixelColour);
			break;
		case 2:
			gb.lights.drawPixel(1,2,pixelColour);
			break;
		case 3:
			gb.lights.drawPixel(1,3,pixelColour);
			break;
		case 4:
			gb.lights.drawPixel(0,3,pixelColour);
			break;
		case 5:
			gb.lights.drawPixel(0,2,pixelColour);
			break;
		case 6:
			gb.lights.drawPixel(0,1,pixelColour);
			break;
		case 7:
			gb.lights.drawPixel(0,0,pixelColour);
			break;
	}

}


//calculates new position from position and destination
int smoothMovement(int position, int destination){
	if (position > destination){
		position = position - max((position - destination) / 2, 1);
	}
	else if (position < destination){
		position = position + max((destination - position) / 2, 1);
	}
	return position;
}


//The main menu. Sets startLevel to selected level
void showMenu(){

	unsigned char selectorPosition = 7;
	unsigned char arrowPosition = 2;
	unsigned char arrowDestination = 0;

	unsigned char selection = 0;
	bool levelDouble = 0;
	
	while(1){
		while(!gb.update())
		// clear the previous screen
		gb.display.clear();


		//check buttons
		if(gb.buttons.pressed(BUTTON_UP)){
			selection = (selection + 2) % 3;
			gb.sound.playTick();
		}


		//move falling block down
		if(gb.buttons.pressed(BUTTON_DOWN)){
			selection = (selection + 1) % 3;
			gb.sound.playTick();
		}
		
		if(gb.buttons.repeat(BUTTON_RIGHT, 3) && selection == 1){
			startLevel = (startLevel + 1) % 10;
			gb.sound.playTick();
		}
		
		if(gb.buttons.repeat(BUTTON_LEFT, 3) && selection == 1){
			startLevel = (startLevel + 9) % 10;
			gb.sound.playTick();
		}
		
		if(gb.buttons.repeat(BUTTON_MENU, 0)){
			levelDouble = 1;
		}
		else{
			levelDouble = 0;
		}
		
		if(gb.buttons.pressed(BUTTON_A)){
			switch(selection){
				case 0:
					startLevel = startLevel + (levelDouble * 10);
					gb.sound.playOK();
					return;
					break;
				case 1:
					break;
					
				case 2:
					gb.sound.playOK();
					highScoreScreen();
					break;
			}
		}
		
		
		switch (selection){
			case 0:
				arrowDestination = 2;
				break;
			case 1:
				arrowDestination = 10;
				break;
			case 2:
				arrowDestination = 26;
				break;
		}
		

		arrowPosition = smoothMovement(arrowPosition, arrowDestination);
		selectorPosition = smoothMovement(selectorPosition, 7 + (5 * startLevel));
		
		//draw arrow on the left side
		gb.display.cursorX = 2;
		gb.display.cursorY = arrowPosition;
		gb.display.print("\20");
		
		
		//draw menu text
		gb.display.setColor(WHITE);
		
		gb.display.cursorX = 6;
		gb.display.cursorY = 2;
		gb.display.print("Play game");
		
		gb.display.cursorX = 6;
		gb.display.cursorY = 10;
		gb.display.print("Set difficulty:");
		gb.display.print(startLevel + (levelDouble * 10));
		
		gb.display.cursorX = 6;
		gb.display.cursorY = 26;
		gb.display.print("Check high scores");


		//difficulty selection box
		gb.display.drawImage(6, 18, difficultyBar);
		
		//draw selector
		gb.display.drawImage(selectorPosition, 19, selector);
		
		//show graphic on bottom of screen
		gb.display.drawImage(0, 52, bottomGraphic);
	}
}

//this is the game
void gameLoop(){
	//initialize and set variables etc. in gameLoop

	unsigned char delayToDrop = 20;	//how many frames until the block goes down automatically
	gameover = 0;
	score = 0;

	rowsCleared = 0;
	allRowsCleared = 0;
	internalRows = 0;

	level = startLevel;

	//empty playfield one square at a time
	for(int i = 0; i < 200; i++){
		playField[i] = 0;
	}

	//run twice to reset next block and current block
	newBlock();
	newBlock();
	setSpeed();

	unsigned char lightTimer;	//used to time light effects
	unsigned char lightEffect = 0;	//if not 0, do the corresponding light effect

while(1){

	while(!gb.update());
	
	// clear the previous screen
	gb.display.clear();
	gb.lights.clear();

	//check buttons
	if(gb.buttons.pressed(BUTTON_A)){
		rotateCW();
	}
	
	//do once and after 3 frames do every 2 frames
	if(gb.buttons.pressed(BUTTON_RIGHT) || gb.buttons.timeHeld(BUTTON_RIGHT) > 3 && gb.buttons.repeat(BUTTON_RIGHT, 2)){
		move('r');
		updateGhost();
	}
	
	//do once and after 3 frames do every 2 frames
	if(gb.buttons.pressed(BUTTON_LEFT) || gb.buttons.timeHeld(BUTTON_LEFT) > 3 && gb.buttons.repeat(BUTTON_LEFT, 2)){
		move('l');
		updateGhost();		
	}
	
	//instantly drop block down
	if(gb.buttons.pressed(BUTTON_UP)){
		instaDrop();
		delayToDrop = speed;
	}

	if(gb.buttons.repeat(BUTTON_DOWN, 0) == 0){
		downReleasedAfterPlace = 1;
	}
	
	//move falling block down. DelayToDrop makes sure the block doesnt move down right after manually moving down
	if(gb.buttons.repeat(BUTTON_DOWN, 0) && downReleasedAfterPlace == 1){
		if(move('d') == 1){
			score = score + 1;
		}
		delayToDrop = speed;
	}
	
		
	else if(delayToDrop == 0) {
		move('d');
		delayToDrop = speed;
	}
	else{
		delayToDrop--;
	}

	/*
	//debugging lights
	if(gb.buttons.pressed(BUTTON_MENU)){
		lightEffect = random(1,5);
	}
	*/

	if(rowsCleared > 0){
		switch (rowsCleared){
			case 1:
				lightEffect = 1;
				score = score + (40 * (level + 1));
				break;
			case 2:
				lightEffect = 2;
				score = score + (100 * (level + 1));
				break;
			case 3:
				lightEffect = 3;
				score = score + (300 * (level + 1));
				break;
			case 4:
				lightEffect = 4;
				score = score + (1200 * (level + 1));
				break;
		}
		rowsCleared = 0;
		
		if(allRowsCleared >= ((startLevel * 10) + 10) || allRowsCleared >= max(100, (startLevel * 10) - 50)){
			//if it is the first time the level goes up, set internalRows to < 10
			if(level == startLevel){
				level++;
				internalRows = internalRows % 10;
			}
			else{
				if(internalRows >= 10){
					internalRows = internalRows - 10;
					level++;
				}
			}
			setSpeed();
		}
		
		
	}
	
	if(lightEffect != 0){

		switch(lightEffect){
			case 1:
				
				gb.lights.drawPixel(0, map(lightTimer, 0, 3, 3, 0), RED);
				gb.lights.drawPixel(1, map(lightTimer, 0, 3, 3, 0), RED);
				if(lightTimer < 3){
					lightTimer++;
				}
				else{
					lightEffect = 0;
				}
				break;
			case 2:
				if(lightTimer < 4){
					gb.lights.drawPixel(0, map(lightTimer, 0, 3, 3, 0), GREEN);
					gb.lights.drawPixel(1, map(lightTimer, 0, 3, 3, 0), GREEN);
				}
				else{
					gb.lights.drawPixel(0, lightTimer % 4 + 1, GREEN);
					gb.lights.drawPixel(1, lightTimer % 4 + 1, GREEN);
				}				
				if(lightTimer < 6){
					lightTimer++;
				}
				else{
					lightEffect = 0;
				}
				break;
			case 3:
				lightTest(lightTimer % 8, DARKBLUE);
				lightTest((lightTimer + 4) % 8, DARKBLUE);
				
				if(lightTimer < 10){
					lightTimer++;
				}
				else{
					lightEffect = 0;
				}
				break;
			case 4:
				lightTest(lightTimer % 8, levelColors[lightTimer % 10]);
				lightTest((lightTimer + 4) % 8, levelColors[(lightTimer + 5) % 10]);
				
				if(lightTimer < 10){
					lightTimer++;
				}
				else{
					lightEffect = 0;
				}

				break;

		}

	}
	if(lightEffect == 0){
		lightTimer = 0;
	}

	//draw playarea borders
	gb.display.setColor(WHITE);
	gb.display.drawFastVLine(1,1,62);
	gb.display.drawFastVLine(32,1,62);
	gb.display.drawFastHLine(2,1,30);
	gb.display.drawFastHLine(2,62,30);
	
	//draw play area background
	
	//draw text on side
	gb.display.setColor(WHITE);
	gb.display.cursorX = 34;
	gb.display.cursorY = 1;
	gb.display.print("SCORE");
	
	gb.display.cursorX = 34;
	gb.display.cursorY = 7;
	gb.display.print((score / 100000000) % 10);
	gb.display.print((score / 10000000) % 10);
	gb.display.print((score / 1000000) % 10);
	gb.display.print((score / 100000) % 10);
	gb.display.print((score / 10000) % 10);
	gb.display.print((score / 1000) % 10);
	gb.display.print((score / 100) % 10);
	gb.display.print((score / 10) % 10);
	gb.display.print((score / 1) % 10);
	
	gb.display.cursorX = 34;
	gb.display.cursorY = 14;
	gb.display.print("LEVEL");
		
	gb.display.cursorX = 34;
	gb.display.cursorY = 20;
	gb.display.print(level);

	gb.display.cursorX = 34;
	gb.display.cursorY = 27;
	gb.display.print("LINES");

	gb.display.cursorX = 34;
	gb.display.cursorY = 33;
	gb.display.print(allRowsCleared);
	
	gb.display.cursorX = 34;
	gb.display.cursorY = 54;
	gb.display.print("NEXT");
	
	
	//draw play area border
	gb.display.drawRect(50,53, 11, 8);
	
	//draw next block
	gb.display.setColor(blockColors[nextBlock]);
	if(nextBlock == 0){	//Fixed drawing for I-block
		gb.display.drawRect(52, 55, 8, 2);
	}
	
	else{
		for(int x = 0; x < 3; x++){
			for(int y = 0; y < 2; y++){
				if(pointNextBlock[x + (y * 3)] > 0 ){
					gb.display.drawRect(52 + (x * 2), 55 + (y * 2), 2, 2);
				}
			}
		}
	}
	
	

	//draw stationary blocks (and background?)
	for(int x = 0; x < playFieldWidth; x++){
		for(int y = 0; y < playFieldHeight; y++){
			//if there is block draw it, else draw background tile
			if( playField[x + (y * playFieldWidth)] > 0 ){
				gb.display.setColor(blockColors[playField[x + (y * playFieldWidth)] - 1]);
				gb.display.drawRect((x * 3) + playFieldStartX, (y * 3) + playFieldStartY, 3, 3);
			}
			/*
			else{
				gb.display.drawImage((x * 3) + playFieldStartX, (y * 3) + playFieldStartY, bgTile);
			}
			*/
		}

	}
	
	
	//draw falling block and ghost block
	//different drawing for I-block
	if(block == 0){
		for(int x = 0; x < 5; x++){
			for(int y = 0; y < 5; y++){
				if(displayedBlock[x + (y*5)] == 1 && y + blockY >= 0){
					//ghost block first so falling block can override it if necessary
					gb.display.setColor(DARKGRAY);
					gb.display.drawRect(playFieldStartX + ((ghostBlockX + x) * 3), playFieldStartY + ((ghostBlockY + y) * 3), 3, 3);
					//falling block
					gb.display.setColor(blockColors[block]);
					gb.display.drawRect(playFieldStartX + ((blockX + x) * 3), playFieldStartY + ((blockY + y) * 3), 3, 3);
				}
			}
		}
	
	}
	else{
		for(int x = 0; x < 3; x++){
			for(int y = 0; y < 3; y++){
				if(displayedBlock[x + (y*3)] == 1 && blockY + y >= 0){
					//ghost block first so the falling block can override it if necessary
					gb.display.setColor(DARKGRAY);
					gb.display.drawRect(playFieldStartX + ((ghostBlockX + x) * 3), playFieldStartY + ((ghostBlockY + y)) * 3, 3, 3);
					//falling block
					gb.display.setColor(blockColors[block]);
					gb.display.drawRect(playFieldStartX + ((blockX + x) * 3), playFieldStartY + ((blockY + y)) * 3, 3, 3);
				}
			}
		}
	}
	
	if(gameover == 1){
		gb.lights.clear();
		gameoverState();
		return;
	}

}
}

//go here when game is over
void gameoverState(){
	unsigned char gameoverRows = 0;
	int countFrames = 0;
	bool blink = 0;	//blinking text on or off
	
	for(int j = 19; j >= 0; j--){
		while(!gb.update());
		for(int i = 0; i < 10; i++){
			gb.display.drawImage(2 + (3 * i), 2 + (3 * j), gameoverBlock);
	
		}
	}
	
	//wait for user input
	while(countFrames < 40){
		while(!gb.update());
		
		if(countFrames > 19){
			gb.display.drawImage(9, 24, gameoverText);
		}
		countFrames++;
	}

	gb.display.drawImage(3, 39, pressA);
	while(!gb.buttons.pressed(BUTTON_A)){
		while(!gb.update());
	}
	gb.sound.playOK();
}

void highScoreScreen(){

while(1){

	while(!gb.update());
	
	// clear the previous screen
	gb.display.clear();
	gb.lights.clear();
	
	
	
	gb.display.println("HIGH SCORES");
	gb.display.drawFastHLine(0, 6, 80);
	
	gb.display.cursorY = 8;
	for(int i = 0; i < 5; i++){
		if(newHighScore == i + 1){
			gb.display.setColor(levelColors[(gb.frameCount / 2) % 10]);
		}
		gb.display.print(highScores[i]);
		gb.display.cursorX = 68;
		gb.display.println(names[i]);
		gb.display.setColor(WHITE);
	}
	

	gb.display.cursorY = 56;
	gb.display.print("Press A to return");
	
	if(gb.buttons.pressed(BUTTON_A)){
		newHighScore = 0;
		return;
	}
	

}
}

void newHighScoreScreen(int i){


unsigned char selectedLetter = 0;	//point to letter

//move all previous records in ram down by one to make sure they won't get overridden
for(int j = 4; j > i; j--){
	highScores[j] = highScores[j-1];
	for(int k = 0; k < 3; k++){
		names[j][k] = names[j-1][k];
	}
}

//set name to AAA
for(int j = 0; j < 3; j++){
	names[i][j] = 65; 
}

while(1){
	while(!gb.update());
	gb.display.clear();
	
	if(gb.buttons.pressed(BUTTON_UP) || gb.buttons.timeHeld(BUTTON_UP) > 10 && gb.buttons.repeat(BUTTON_UP, 2)){
		names[i][selectedLetter]++;
		if(names[i][selectedLetter] > 90){
			names[i][selectedLetter] = 65;
		}
		gb.sound.playTick();
	}
	if(gb.buttons.pressed(BUTTON_DOWN) || gb.buttons.timeHeld(BUTTON_DOWN) > 10 && gb.buttons.repeat(BUTTON_DOWN, 2)){
		names[i][selectedLetter]--;
		if(names[i][selectedLetter] < 65){
			names[i][selectedLetter] = 90;
		}
		gb.sound.playTick();
	}
	if(gb.buttons.pressed(BUTTON_RIGHT)){
		selectedLetter = (selectedLetter + 1) % 3;
		gb.sound.playTick();
	}
	if(gb.buttons.pressed(BUTTON_LEFT)){
		selectedLetter = (selectedLetter + 2) % 3;
		gb.sound.playTick();
	}
	

	if(gb.buttons.pressed(BUTTON_A)){
		gb.sound.playTick();
		if(selectedLetter == 2){
			highScores[i] = score;
			
			//save updated scores to sd
			for(int i = 0; i < 5; i++){
				gb.save.set(i, highScores[i]);
				gb.save.set(i + 5, names[i]);
			}
			score = 0;
			newHighScore = i + 1;
			return;
		}
		
		else{
			selectedLetter++;
		}
	}	
	
	gb.display.setFont(font5x7);
	
	gb.display.cursorX = 8;
	gb.display.setColor(levelColors[(gb.frameCount / 2) % 10]);
	gb.display.print("High score!");
	
	gb.display.setColor(WHITE);

	gb.display.cursorX = 8;
	gb.display.cursorY = 16;
	gb.display.print(score);
	
	gb.display.cursorX = 31;
	gb.display.cursorY = 32;
	gb.display.print(names[i]);
	
	if(gb.frameCount % 16 < 8){
		gb.display.drawFastHLine(31 + (selectedLetter * 6), 40, 5);
	}
	
}
}

// the setup routine runs once when Gamebuino starts up
void setup(){
	// initialize the Gamebuino object
	gb.begin();
	SerialUSB.begin(9600);
	SerialUSB.println("READY");
	//load scores
	for(int i = 0; i < 5; i++){
		highScores[i] = gb.save.get(i);
		gb.save.get(i + 5, names[i]);
	}
}

// the loop routine runs over and over again forever
//loop switches main functions of the game
void loop(){

	showMenu();
	gameLoop();
	for(int i = 0; i < 5; i++){
		if(score > highScores[i]){
			newHighScoreScreen(i);
			break;
		}
	}
	highScoreScreen();
}
