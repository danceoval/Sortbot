//Sortbot Code by Daniel Sohval


//Are you a fan of cheap beer and/or a neat freak?
//Fear not, for Sortbot is here for you!
// After a raging keg party your housemate threw, you may scrape vomit out of your carpeting with peace of mind,
// knowing Sortbot will clean up all your plastic cups from the floor AND sort them by color so
// as to not trigger any additional anxieties in your fragile, hung-over state!


// This was for a Behavior-Based Artificial Intelligene project for Vassar College's COGS211 "Perception and Action" class.
//Sortbot is designed to navigate an environment of overturned red and blue solo cups. 
//Sortbot searches through the cups and carries the red cups to one side of the arena and the blue cups to the parallel side.


//Morphological features of note include a Lego claw for grabbing cups, an IR beacon, 
//a camera calibrated for sensitivity towards red and blue, wheels and a front bumper.


#include <stdio.h>

//Initialize seeking cups, carrying cups and escaping servos

void Move(int command);
int seek_cups();
int see_cup();
int carry_red();
int carry_blue();
int EscapeFrontBump();

//Initialize movements

const int FRONT_BUMP = 0;
const int BACK_LEFT = 0;
const int BACK_RIGHT = 1;
const int ARC_LEFT = 2;
const int ARC_RIGHT = 3;
const int ESCAPE_FORWARD = 4;
const int FORWARD = 5;
const int STOP = 6;
const int DETECT_BALL_RIGHT = 7;
const int DETECT_BALL_LEFT = 8;
const int DROP_CUP1 = 9;
const int WANDER_ARC_LEFT = 10;
const int GRAB_CUP = 11;
const int DROP_CUP2 = 12;
const int DROP_CUP3 = 13;

const int RED = 0;
const int BLUE = 1;

const int SEEK_CUPS = 0;//state 0
const int SEE_CUP = 1;//state1
const int CARRY_RED = 2;//state 2
const int CARRY_BLUE = 3;//state 3

int east; //north is the orange wire
int south;//green wire
int west;//yellow wire
int north;//blue wire

int state;
int Escape_Command;

int true = 1;
int false = 0;

//Behavior hierarchy for Sortbot
//Finite-State Schema Architecture

int main()
{
	state = SEEK_CUPS;
	while(1) {
		if(state == SEEK_CUPS){
			seek_cups();
		}
		else if(state == SEE_CUP){
			see_cup();
		}
		else if(state == CARRY_RED){
			carry_red();
		}
		else if(state == CARRY_BLUE){
			carry_blue();
		}
	}
	return 0;
}



int seek_cups()
{ 	
	track_update();
	if((track_count(RED) > 0) || (track_count(BLUE) > 0)){
		state = SEE_CUP;
		printf("In state: See Cup\n"); 	//Print state debugging
	}
	else if(EscapeFrontBump()){
		printf("escaping...\n");
		Move(Escape_Command);
	}
	else{
		Move(WANDER_ARC_LEFT); //Move in arc to cover maximum floorspace of environment
	}
}

//Divide environment into 6 sectors based on location of IR beacon in corner

int see_cup() 
{
	int RED_X, RED_Y, BLUE_X, BLUE_Y, xMax = 160, yMax = 120;
	track_update(); // get most recent camera image and process it
 
		RED_X = track_x(RED,0);  // get image x data R
		RED_Y = track_y(RED,0);  // get image y data R
		BLUE_X = track_x(BLUE,0);  // get image x data B
		BLUE_Y = track_y(BLUE,0);  // get image y data B
		if(track_count(RED) > 0) {  // if there is a red blob 
			if(RED_X >= 0 && RED_X < (xMax/3)
			&& RED_Y >= 0 && RED_Y < (yMax/2))
{   // AREA 1
				Move(DETECT_BALL_LEFT);
			}
			else if(RED_X >= 0 && RED_X < (xMax/3)
				&& RED_Y >= (yMax/2) && RED_Y <= yMax)
{ // AREA 2
	Move(GRAB_CUP);//Move Forward to secure grasp on cup
	printf("Now in state: Carry Red\n"); 	//Print state debugging
		state = CARRY_RED;//Bring red cup to depository	
			}
			else if(RED_X >= (xMax/3) && RED_X < ((2*xMax)/3)
				&& RED_Y >= 0 && RED_Y < (yMax/2))
{ // AREA 3
	Move(FORWARD);
			}
	else if(RED_X >= (xMax/3) && RED_X < ((2*xMax)/3)
	&& RED_Y >= (yMax/2) && RED_Y <= yMax)
{ // AREA 4
Move(GRAB_CUP);//Move Forward to secure grasp on cup
	printf("Now in state: Carry Red\n");
	state = CARRY_RED;//Bring red cup to depository	
			}
	else if(RED_X >= ((2*xMax)/3) && RED_X <= xMax
		&& RED_Y >= 0 && RED_Y < (yMax/2))
{ // AREA 5
	Move(DETECT_BALL_RIGHT);
			}
	else if(RED_X >= ((2*xMax)/3) && RED_X <= xMax
	&& RED_Y >= (yMax/2) && RED_Y <= yMax)
{ // AREA 6
	Move(GRAB_CUP);//Move Forward to secure grasp on cup
	printf("Now in state: Carry Red\n");
	state = CARRY_RED;//Bring red cup to depository	
			}
		}
else if(track_count(BLUE) > 0) {  // if there is a blue blob 
			if(BLUE_X >= 0 && BLUE_X < (xMax/3)
			&& BLUE_Y >= 0 && BLUE_Y < (yMax/2)){   // AREA 1
				Move(DETECT_BALL_LEFT);
			}
			else if(BLUE_X >= 0 && BLUE_X < (xMax/3)
				&& BLUE_Y >= (yMax/2) && BLUE_Y <= yMax)
{ // AREA 2
	Move(GRAB_CUP);//Move Forward to secure grasp on cup
	printf("Now in state: Carry Blue\n");
	state = CARRY_BLUE;//Bring red cup to depository	
			}
	else if(BLUE_X >= (xMax/3) && BLUE_X < ((2*xMax)/3)
			&& BLUE_Y >= 0 && BLUE_Y < (yMax/2))
{ // AREA 3
	Move(FORWARD);
			}
	else if(BLUE_X >= (xMax/3) && BLUE_X < ((2*xMax)/3) && BLUE_Y >= (yMax/2) && BLUE_Y <= yMax){ // AREA 4
		Move(GRAB_CUP); // Move forward to secure grasp on cup
		printf("Now in state: Carry Blue\n");
		state = CARRY_BLUE; //Carry blue cup to depository	
			}
	else if(BLUE_X >= ((2*xMax)/3) && BLUE_X <= xMax
		&& BLUE_Y >= 0 && BLUE_Y < (yMax/2))
{ // AREA 5
	Move(DETECT_BALL_RIGHT);
		}
	else if(BLUE_X >= ((2*xMax)/3) && BLUE_X <= xMax
	&& BLUE_Y >= (yMax/2) && BLUE_Y <= yMax)
{ // AREA 6
	Move(GRAB_CUP);//Move Forward to secure grasp on cup
	printf("Now in state: Carry Blue\n");
	state = CARRY_BLUE;//Bring red cup to depository	
			}
}
	else{
		state = SEEK_CUPS;
		}
		sleep(0.1); // don't rush print statement update
		track_update();  // get new image data before repeating
}

//Carry red cup and deposit it 
int carry_red() 
{	
	if (analog10(FRONT_BUMP) < 371){
		Move(DROP_CUP1);
		printf("Drop Cup 1/3 successful\n");
		Move(DROP_CUP2);
		printf("Drop Cup 2/3 succesful\n");
		Move(DROP_CUP3);
		printf("Drop Cup 3/3 succesful\n");
		state = SEEK_CUPS;}
	else {
	east=digital(10); //north is the orange wire
    south=digital(11);//green wire
    west=digital(12);//yellow wire
    north=digital(13);//blue wire
		
		//1 is North
		if (north){ 
		printf("North :-D\n");
		Move(FORWARD);	}//Red deposit at north end
		//2 is South
		if (south){
		printf("South :-P\n");
			Move(ARC_LEFT);}//turn to face east
		//3 is East
		if (east){
		printf("East :-)\n");
			Move(DETECT_BALL_RIGHT);}//Turn to face North
		//4 is West
		if (west){ // switched from -30 (neg old amplitude) to -60 (neg new amplitude)
		printf("West :-O\n");
			Move(DETECT_BALL_LEFT);}//Turn to face west
	}
}
//Carry blue cup and deposit it
int carry_blue() 
{
	east=digital(10); //north is the orange wire
    south=digital(11);//green wire
    west=digital(12);//yellow wire
    north=digital(13);//blue wire
	
	if (analog10(FRONT_BUMP) < 371){
		Move(DROP_CUP1);
		printf("Drop Cup 1/3 successful\n");
		Move(DROP_CUP2);
		printf("Drop Cup 2/3 succesful\n");
		Move(DROP_CUP3);
		printf("Drop Cup 3/3 succesful\n");
		state = SEEK_CUPS;}
	else {	
		//1 is North
		if (north){ 
		printf("North :-D\n");
		Move(ARC_LEFT);	}//Turn to face West
		//2 is South
		if (south){
		printf("South :-P\n");
			Move(FORWARD);}//Move towards opposite corner
		//3 is East
		if (east){
		printf("East :-)\n");
			Move(DETECT_BALL_LEFT);}//Turn to face South
		//4 is West
		if (west){ // switched from -30 (neg old amplitude) to -60 (neg new amplitude)
		printf("West :-O\n");
			Move(DETECT_BALL_RIGHT);}//Turn to face South
	}
}
//Escape behvior, in case Sortbot hits a wall or obstacle
int EscapeFrontBump(){
	if(analog10(FRONT_BUMP) < 231){ //If bumped on left side
			Escape_Command = BACK_LEFT;
			return true;
	}
	else if(analog10(FRONT_BUMP) > 230 && analog10(FRONT_BUMP) < 371){ //If bumped on right side
			Escape_Command = BACK_RIGHT;
			return true;
	}
	else{
			return false;
	}
}

void Move(int command){
	if(command == BACK_LEFT){
		motor(0, -50);
		motor(3, -70);
		sleep(1.0);
	}
	else if(command == BACK_RIGHT){
		motor(0, -70);
		motor(3, -50);
		sleep(1.0);
	}
	
	else if(command == ARC_RIGHT){
		motor(0, 70);
		motor(3, 50);
		sleep(1.0);
	}
	
	else if(command == ARC_LEFT){
		motor(0, 50);
		motor(3, 70);
		sleep(1.0);
	}
	else if(command == WANDER_ARC_LEFT){
		motor(0, 60);
		motor(3, 65);
		sleep(0.2);
	}
	else if(command == DETECT_BALL_RIGHT){
		motor(0, 50);
		motor(3, 30);
		sleep(0.1);
	}
	
	else if(command == DETECT_BALL_LEFT){
		motor(0, 30);
		motor(3, 50);
		sleep(0.1);
	}
	
	else if(command == ESCAPE_FORWARD){
		motor(0, 100);
		motor(3, 100);
		sleep(0.7);
		motor(0, 70);
		motor(3, 50);
		sleep(2.0);
	}	
	
	else if (command == DROP_CUP1){
		motor(0, -50);
		motor(3, -50);
		printf("Drop Cup 1/3 happened\n");
		sleep(1.0);
	}
	
	else if (command == DROP_CUP2){
		motor(0, -50);
		motor(3, 50);
		printf("Drop Cup 2/3 happened\n");
		sleep (1.5);
	}
	
	else if (command == DROP_CUP3){
		motor(0, 65);
		motor(3, 65);
		printf("Drop Cup 3/3 happened\n");
		sleep(1.9);
	}
	
	else if(command == FORWARD){
		motor(0, 65);
		motor(3, 65);
	}
	else if(command == GRAB_CUP){
		motor(0, 65);
		motor(3, 65);
		sleep(1.0);
	}
	
	else if(command == STOP){
		motor(0, 0);
		motor(3, 0);
	}
}

