// This program creates a yahtzee game.
// Created by Emily Obaditch and Lauren Ferrara

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gfx4.h"
#include <math.h>

struct Player{
        int ones;
        int twos;
        int threes;
        int fours;
        int fives;
        int sixes;
        int topTotal;
        int threeKind;
        int fourKind;
        int fullHouse;
        int smallStraight;
        int largeStraight;
        int yahtzee;
        int chance;
        int bottomTotal;
        int turns;
	int rounds;
	int total; 
};

void game_setup( int, int[5], struct Player[4], int );
void print_categories( int, int );
void print_players( int, int, int );
void create_die( struct Player[4], int);
char *num2str(int);
int check_event(int[5], int[5], int, struct Player[4], int);
void print_roll(void); 
void roll_dice( int[5], int[5]);
void print_die( int[5] );
void refresh_saved( int[5] );
void initialize_scores( int, struct Player[4] );
int end_early( int, int, struct Player[4], int[5]);
int choose_score( int, int, struct Player[4], int[5] );
int total_dice( int[5] );
int check_threeKind( int[5] );
int check_fourKind( int[5] );
int check_fullHouse( int[5] );
int check_smallStraight( int[5] );
int check_largeStraight( int[5] );
int check_yahtzee( int[5] );
void bonus_yahtzee( int, int, struct Player[4], int[5], int);
void end_game( int, struct Player[4] );
void bottom_total( int, int, struct Player[4] );
void top_total( int, int, struct Player[4] );
void display_total( int, int, struct Player[4]);

int main( void ){
	int dice[5]; 
	int saved[5];
	int wd = 800;
	int ht = 700;
	char c, d;
	int numPlayers, x, y;
	int whoPlayer = 0;
	srand(time(NULL));

	// Asks for number of players in terminal window	
	printf( "How many players would you like (1, 2, 3, or 4)?\n" );
	scanf( "%d", &numPlayers);

	// Array of struct players based on number of users entered
	struct Player players[numPlayers];

	gfx_open( wd, ht, "Yahtzee Game" );
	gfx_clear(); 

        while( 1 ){
		// Welcome window before game starts
		gfx_changefont("lucidasans-24"); 
		gfx_text(250, 250, "Welcome to Yahtzee!");
		gfx_changefont("lucidasans-12"); 
		gfx_text(325, 350, "Press 's' to start." );

		// Waits for an input
                if( gfx_event_waiting() ){
                        c = gfx_wait();
                        switch( c ){
				case 's': // to start the game
					gfx_clear(); // clears the welcome screen
					game_setup( numPlayers, saved, players, whoPlayer ); 
					gfx_color(0,0,0);
                			gfx_fill_rectangle(140, 75, 150, 100 );
					// checks if it should quit
					if( check_event( dice, saved, numPlayers, players, whoPlayer ) == 0)
						return 0;
                                case 'q': // to quit
                                        return 0;
                                        break;
                        }
		}
	}
}

// function to convert a number to a string
char *num2str(int n){
	static char a[10], *p = a;
	snprintf(p, 10, "%d", n);
	return p;
}
 
void game_setup( int numPlayers, int saved[5], struct Player players[numPlayers], int whoPlayer ){

	int i, j;
	int h = 5;
	int height = 650;
	int width = 350;
	int w = 60;

	// fills rectangular score card
	gfx_color( 255, 255, 255 );
	gfx_fill_rectangle( 400, 5, width, height );

	// draws horizontal lines on score card
	for( i = 0; i < 16; i++ ){
		gfx_color( 0, 0, 255 );
		h = h + height/17;
		gfx_line( 400, h, 750, h );
	}

	// draws vertical lines on scord card (depends on number of players)
	for( j = 0; j <= numPlayers; j++ ){
		gfx_color( 0, 0, 255 );
		w = w + (width-60)/(numPlayers+1);
		gfx_line( 400+w, 5, 400+w, 650);
	}

	// prints option to end before 3 turns are taken
	gfx_color(0,255,0);
	gfx_changefont("lucidasans-12");
	gfx_text( 50, 450, "Press space to end round early and choose score.");

	print_categories( 5, height );
	print_players( 80, width, numPlayers );
	print_roll(); 
	create_die(players, whoPlayer);
	refresh_saved( saved );
	initialize_scores( numPlayers, players );
}

void initialize_scores( int numPlayers, struct Player players[numPlayers] ){

	int i;

	// sets all scores to -1 and other categories to 0
	for( i = 0; i < numPlayers; i++ ){
		players[i].ones = -1;
        	players[i].twos = -1;
        	players[i].threes = -1;
        	players[i].fours = -1;
       		players[i].fives = -1; 
        	players[i].sixes = -1;
        	players[i].topTotal = 0;
        	players[i].threeKind = -1;
        	players[i].fourKind = -1;
        	players[i].fullHouse = -1;
        	players[i].smallStraight = -1;
        	players[i].largeStraight = -1;
        	players[i].yahtzee = -1;
        	players[i].chance = -1;
        	players[i].bottomTotal = 0;
        	players[i].turns = 0;
		players[i].rounds = 0;
		players[i].total = 0; 
	}

}

// function to put category names on the score card
void print_categories( int h, int height ){


	int i; 	

	char categories[17][20] = { "Categories", "Ones (1)", "Twos (2)", "Threes (3)", "Fours (4)", "Fives(5)", "Sixes(6)", "Top Total", "Three of a Kind (t)", 
		"Four of a Kind (f)", "Full House (h)", "Small Straight (s)", "Large Straight (l)", "Yahtzee!!! (y)", "Chance (c)", "Bottom Total", "Total" };
	// the char in ( ) above is what the user enters to choose a category
	
	for( i = 0; i < 17; i++ ){
		gfx_color( 0, 0, 0 );
		h = h + height/17;
		gfx_text( 401, h-5, categories[i] );
	}
}

// function to print the player numbers on the score card
void print_players( int w, int width, int numPlayers ){

	int i;

	for( i = 0; i <= numPlayers; i++ ){
		gfx_color( 0, 0, 0 );
		w = w + (width-60)/(numPlayers+1);
		gfx_text( 401+w, 30, num2str(i+1) );
	}
}

// function to set all dice to not saved (should be rolled)
void refresh_saved( int saved[5] ){
	int i;

	for( i = 0; i < 5; i++ ){
        	saved[i] = 1; // 1 means not saved
	}
}

// function to draw 5 rectangular die
void create_die( struct Player players[4], int whoPlayer ){

	gfx_color(0,0,0);
	gfx_fill_rectangle(215, 50, 50, 50 );

	int i, j;
	int value;

	gfx_changefont("lucidasanstypewriter-24");

	// gives the current player and roll number at top of the screen
	gfx_color(255,255,0); 
	gfx_text(125, 50, "Player: ");
	gfx_text(225, 50, num2str(whoPlayer+1));  
	gfx_text(140, 100, "Roll: "); 
	gfx_text(215, 100, num2str(players[whoPlayer].turns) ); 

	// draws the five die
	gfx_color( 255, 255, 255 );
	gfx_fill_rectangle( 5, 580, 70, 70 );
	gfx_fill_rectangle( 85, 580, 70, 70 );
	gfx_fill_rectangle( 165, 580, 70, 70 );
	gfx_fill_rectangle( 245, 580, 70, 70 );
	gfx_fill_rectangle( 325, 580, 70, 70 );

	gfx_changefont("lucidasanstypewriter-10");
}

// function to print the black circles on the die
void print_die( int dice[5] ){

	int i;

	gfx_color(0,0,0);
	
	for( i = 0; i < 5; i++ ){
		if( dice[i]%2 == 1 )
			gfx_fill_circle( 40 + 80*i, 615, 8 );
		if( dice[i] > 1 ){
			gfx_fill_circle( 65 + 80*i, 590, 8 );
			gfx_fill_circle( 15 + 80*i, 640, 8 );
		}
		if( dice[i] > 3 ){
			gfx_fill_circle( 15 + 80*i, 590, 8 );
			gfx_fill_circle( 65 + 80*i, 640, 8 );
		}
		if( dice[i] > 5 ){
			gfx_fill_circle( 15 + 80*i, 615, 8 );
			gfx_fill_circle( 65 + 80*i, 615, 8 );
		}
	}
}

// function to create the Roll button
void print_roll(void){

	gfx_color(255,0,0); 
	gfx_fill_rectangle(110, 250, 150, 150); 
	gfx_color(255,255,255);
	gfx_changefont("lucidasans-24");
	gfx_text(160,335, "Roll!"); 

}

// function to roll die and get random values
void roll_dice( int dice[5], int saved[5] ){


	int value, i;  
	
	for( i = 0; i < 5; i++ ){
		value = rand() % 6 + 1; // gives a value for each die between 1 and 6;
		if( saved[i] ){
			switch( value ){
				case 1:
					dice[i] = 1; 
					break;
				case 2:
					dice[i] = 2; 
					break;
				case 3:
					dice[i] = 3; 
					break;
				case 4:
					dice[i] = 4; 
					break;
				case 5:
					dice [i] = 5; 
					break;
				case 6:
					dice[i] = 6; 
					break;
			}	
		}
	}	

	print_die( dice );

}

// function to check what the user does next, if 3 rolls are completed, and if the game is over
int check_event( int dice[5], int saved[5], int numPlayers, struct Player players[numPlayers], int whoPlayer ){
	char d;
	int x, y, i;

	// the user has rolled 3 times
	if( players[whoPlayer].turns >= 3 ){
                gfx_color(0,0,0);
                gfx_fill_rectangle(125, 0, 150, 100 ); // to clear player on the screen
                gfx_color(255,255,0);
                gfx_text(125, 50, "Choose a category!");

                if( choose_score(numPlayers, whoPlayer, players, dice) == 0 )
			return 0; // if user presses quit while in choose_score

                gfx_color(0,0,0);
                gfx_fill_rectangle( 125, 0, 125, 100 ); // to clear "choose a category"
                players[whoPlayer].turns = 0; // sets rolls back to 0
                players[whoPlayer].rounds++; // increments player rounds

		// determines the next player
                if( whoPlayer + 1 < numPlayers )
                        whoPlayer++;
                else whoPlayer = 0;

		// checks if the game is over, the next player has completed 13 rounds
                if( players[whoPlayer].rounds == 13 ){
                        printf("Game over\n");
                        end_game( numPlayers, players );
                        return 0;
                }
                else
                        if( check_event( dice, saved, numPlayers, players, whoPlayer ) == 0)
				return 0;
			

	}
	else{
        	d = gfx_wait();
        	if( d==1 ){ // the mouse is clicked
        			x = gfx_xpos();
                		y = gfx_ypos();
				gfx_color(255,0,0); 
			if( x >= 110 && x <= 260 && y >= 250 && y <= 400 ){ // roll button is clicked
					players[whoPlayer].turns++;
					create_die(players, whoPlayer);
					roll_dice(dice, saved);
					refresh_saved( saved );
                        	if( check_event( dice, saved, numPlayers, players, whoPlayer ) == 0)
                                	return 0;


			}
			else{	
				gfx_color(255,0,0);
				if( x >= 5 && x <=75 && y >= 580 && y <= 650){ // first die is saved
                                                gfx_rectangle(5,580,69, 69);
                                                saved[0] = 0;
				}
				else if( x >= 85 && x <= 155 && y >= 580 && y <= 650){ // second die is saved
                                                gfx_rectangle(85,580,69, 69);
                                                saved[1] = 0;
				}
				else if( x >= 165 && x <= 235 && y >= 580 && y <= 650){ // third die is saved
                                                gfx_rectangle(165,580,69, 69);
                                                saved[2] = 0;
				}	
				else if( x >= 245 && x <= 315 && y >= 580 && y <= 650){ // fourth die is saved
						gfx_rectangle(245,580,69, 69);
                                                saved[3] = 0;
				}
				else if( x >= 325 && x <= 395 && y >= 580 && y <= 650){ // fifth die is saved
                                        	gfx_rectangle(325,580,69, 69);
						saved[4] = 0;
				}	
				check_event( dice, saved, numPlayers, players, whoPlayer );

			}
		}		
		else if( d== ' '){ // space is clicked to end round before 3 rolls completed
				
				whoPlayer = end_early( numPlayers, whoPlayer, players, dice);
			        if( whoPlayer == -1 )
					return 0; // checks if it should quit upon user request
	                        if( check_event( dice, saved, numPlayers, players, whoPlayer ) == 0)
                                	return 0; // checks if it shoudl quit upon user request

				return 1;
				
			}
		else if( d == 'q' ){ // to quit
				return 0;
		}
		else{ // to catch exceptions
				players[whoPlayer].turns--;
                        	gfx_color(0,255,0);
                        	gfx_changefont("lucidasans-12");
                        	gfx_text(25, 500, "You must enter a valid category. See the board for codes.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
					return 0;
		}
	

                                players[whoPlayer].turns++;
                                create_die(players, whoPlayer);
                                roll_dice(dice, saved);
                                refresh_saved( saved );
	                        if( check_event( dice, saved, numPlayers, players, whoPlayer ) == 0)
        	                        return 0;

	}	

	return 0;
}


int end_early( int numPlayers, int whoPlayer, struct Player players[4], int dice[4]){
        
	gfx_color(0,0,0);
        gfx_fill_rectangle(125, 0, 150, 100 ); // to clear player on the screen
        gfx_color(255,255,0);
        gfx_text(125, 50, "Choose a category!");
        
	if( choose_score(numPlayers, whoPlayer, players, dice) == 0)
		return -1;
        gfx_color(0,0,0);
        gfx_fill_rectangle(125, 0, 150, 100);

        players[whoPlayer].turns = 0; // sets turns back to 0
        players[whoPlayer].rounds++; // increments rounds played

	// finds next player
        if( whoPlayer + 1 < numPlayers )
        	whoPlayer++;
        else whoPlayer = 0;

	// checks if game is over ( 13 rounds completed )
        if( players[whoPlayer].rounds == 13 ){
                printf("Game over\n");
                end_game( numPlayers, players );
        }

	return whoPlayer;
}

// function for user to choose category for their score after 3 turns or if end_early called
int choose_score( int numPlayers, int whoPlayer, struct Player players[4], int dice[5] ){
	char c  = gfx_wait();
	int i;

        gfx_color(0,0,0);
        gfx_fill_rectangle(5, 475, 380, 100);

	// horizontal spacing for score card
	int x = 481 + (whoPlayer+1) * (290/(numPlayers+1));

	gfx_color(0,0,0);
        gfx_fill_rectangle(5, 475, 380, 100);

	gfx_changefont( "lucidasans-12" );

	switch( c ){
		case '1': 
			if( players[whoPlayer].ones == -1){ // score has not been chosen yet
				players[whoPlayer].ones = 0;
				// counts the ones and adds them
				for( i = 0; i < 5; i++ ){
					if( dice[i] == 1 )
						players[whoPlayer].ones++;
				}
				gfx_text( x, 70, num2str( players[whoPlayer].ones) );		
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for ones has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
	                        if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;		
			}
			break;
		case '2':
			if( players[whoPlayer].twos == -1){ // score has not been chosen yet
                                players[whoPlayer].twos = 0;
				// counts the twos and adds them
                        	for( i = 0; i < 5; i++ ){
                                	if( dice[i] == 2 )
                                        	players[whoPlayer].twos+=2;
                        	}
				gfx_text( x, 107, num2str(players[whoPlayer].twos));
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for twos has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;			}
			break;
		case '3':
			if( players[whoPlayer].threes == -1){
                                players[whoPlayer].threes = 0;
				// counts the threes and adds them
                        	for( i = 0; i < 5; i++ ){
                                	if( dice[i] == 3 )
                                        	players[whoPlayer].threes+=3;
                        	}	
                        gfx_text( x, 145,num2str(players[whoPlayer].threes) );
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for threes has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;
			}
			break;
		case '4':
                	if( players[whoPlayer].fours == -1){
                                players[whoPlayer].fours = 0;
		        	// counts the fours and adds them
				for( i = 0; i < 5; i++ ){
                                	if( dice[i] == 4 )
                                 	       players[whoPlayer].fours+=4;
                        	}
                        	gfx_text( x, 182,num2str(players[whoPlayer].fours) );
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for fours has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
	                        if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;
			}
			break;
		case '5':
                        if( players[whoPlayer].fives == -1){
                                players[whoPlayer].fives = 0;
                        	// counts the fives and adds them
				for( i = 0; i < 5; i++ ){
                                	if( dice[i] == 5 )
                                	        players[whoPlayer].fives+=5;
                        	}	
                        	gfx_text( x, 220, num2str(players[whoPlayer].fives));
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for small fives has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
	                        if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;

			}
			break;
		case '6':
                        if( players[whoPlayer].sixes == -1){
                                players[whoPlayer].sixes = 0;
                        	// counts the sixes and adds them
				for( i = 0; i < 5; i++ ){
                                	if( dice[i] == 6 )
                                        	players[whoPlayer].sixes+=6;
                        	}	
                        	gfx_text( x,  257, num2str(players[whoPlayer].sixes) );
			}
			else{ // error message
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for sixes has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                                if(choose_score( numPlayers, whoPlayer, players, dice ) == 0)
                                	return 0;
			}
			break;
		case 't': // for three of a kind
                        if( players[whoPlayer].threeKind == -1){
				if( check_threeKind(dice) )
					players[whoPlayer].threeKind = total_dice( dice ); // score is sum of all dice
				else players[whoPlayer].threeKind = 0;
                        	gfx_text( x, 333, num2str(players[whoPlayer].threeKind) );
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for three of a kind has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
					return 0;
			}
			break;
		case 'f': // for four of a kind
                        if( players[whoPlayer].fourKind == -1){
				if( check_fourKind(dice) )
					players[whoPlayer].fourKind = total_dice( dice ); // score is sum of all dice
				else players[whoPlayer].fourKind = 0;
                        	gfx_text( x,  371, num2str(players[whoPlayer].fourKind));
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for four of a kind has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;
			}
			break;
		case 'h': // for full house
                        if( players[whoPlayer].fullHouse == -1){
				if( check_fullHouse(dice) )
					players[whoPlayer].fullHouse = 25;
				else players[whoPlayer].fullHouse = 0;
                        	gfx_text( x, 408, num2str(players[whoPlayer].fullHouse) );
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for full house has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                                if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0; 
			}
			break;
		case 's': // for small straight
                        if( players[whoPlayer].smallStraight == -1){
				if( check_smallStraight(dice) )
					players[whoPlayer].smallStraight = 30;
				else players[whoPlayer].smallStraight = 0;
                        	gfx_text( x, 446, num2str(players[whoPlayer].smallStraight));
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for small straight has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");                                
	                        if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                        	        return 0;
			}
			break;
		case 'l': // for large straight
                        if( players[whoPlayer].largeStraight == -1){
				if( check_largeStraight(dice) )
					players[whoPlayer].largeStraight = 40;
				else players[whoPlayer].largeStraight = 0;
                        	gfx_text( x, 483,num2str(players[whoPlayer].largeStraight) );
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "A score for large straight has already been entered.");
                                gfx_text(70, 550, " Please choose another category.");
                                if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;        
			}
			break;
		case 'y': // for yahtzee
                        if( players[whoPlayer].yahtzee >= 50 ){ // if scored before, get bonus and another category
                                if( check_yahtzee(dice) )
					bonus_yahtzee( numPlayers, whoPlayer, players, dice, x);
				else{
                                	gfx_text(50, 500, "This is not a yahtzee.");
                                	gfx_text(70, 550, " Please choose another category.");
                                	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                        	return 0;
				}
			}
                        else if( players[whoPlayer].yahtzee == -1){
				if( check_yahtzee(dice) )
					players[whoPlayer].yahtzee = 50;
				else players[whoPlayer].yahtzee = 0;
                        	gfx_text( x, 521, num2str(players[whoPlayer].yahtzee));
			}
			else{
                                gfx_color(0,255,0);
                                gfx_changefont("lucidasans-12");
                                gfx_text(50, 500, "You have already zeroed your yahtzee score.");
                                gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;
			}
			break;
		case 'c': // for chance
                        if( players[whoPlayer].chance == -1){
				players[whoPlayer].chance = total_dice( dice ); // score is sum of dice
                        	gfx_text( x, 560, num2str(players[whoPlayer].chance));			
			}
			else{
				gfx_color(0,255,0);
                        	gfx_changefont("lucidasans-12");
                        	gfx_text(50, 500, "A score for chance has already been entered.");
				gfx_text(70, 550, " Please choose another category.");
                        	if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
                                	return 0;
			}
			break;
		case 'q': // to quit the program
			return 0;
		default: // to catch exceptions
			gfx_color(0,255,0);
			gfx_changefont("lucidasans-12");
			gfx_text(25, 500, "You must enter a valid category. See the board for codes.");
			if( choose_score( numPlayers, whoPlayer, players, dice ) == 0 )
				return 0;
	}

	// adds the top total, the bottom total, & the total, and prints to score card
	top_total(numPlayers, whoPlayer, players);
	bottom_total(numPlayers, whoPlayer, players);
	display_total(numPlayers, whoPlayer, players);
	return 1;
}

// function to add all five dice
int total_dice( int dice[5] ){

	int i;
	int total = 0;

	for( i = 0; i < 5; i++ ){
		total+= dice[i];
	}

	return total;
}

// function to check if it is a three of a kind
int check_threeKind( int dice[5] ){
	int i;
	int type1;
	int type2 = 0;
	int type3 = 0;
	int total1 = 1;
	int total2 = 0;
	int total3 = 0;

        type1 = dice[0];

        for( i = 1; i < 5; i++ ){
                if( dice[i] == type1 )
			total1++;
                else if( type2 == 0 && dice[i] != type1 ){
                        type2 = dice[i];
                        total2++;
                }
		else if( type3 == 0 && dice[i] != type1 && dice[i] != type2 ){
			type3 = dice[i];
			total3++;
		}
                else if( dice[i] == type2 )
                        total2++;
		else if( dice[i] == type3 )
			total3++;
		else
			return 0;
	}

        if( total1 >= 3 || total2 >= 3 || total3 >= 3 )
                return 1;
        return 0;
}

// function to check if it is a four of a kind
int check_fourKind( int dice[5] ){
        int i,j;
	int total = 1;

        i = dice[0];
	for( j = 1; j < 5; j++ ){
		if( i == dice[j] )
			total++;
	}
	
	if( total >= 4 ) return 1;

	else{
		total = 1;
		i = dice[1];
		for( j = 2; j < 5; j++ ){
                       	if( i != dice[j] )
                               	return 0;
		}
	}

	return 1;

}

// function to check if it is a full house ( 2 of one # and 3 of another # )
int check_fullHouse( int dice[5] ){

	int i;
	int type1;
	int type2 = 0;
	int total1 = 1;
	int total2 = 0;
	
	type1 = dice[0];
	
	for( i = 1; i < 5; i++ ){
		if( dice[i] == type1 )
			total1++;
		else if( type2 == 0 && dice[i] != type1 ){
			type2 = dice[i];
			total2++;
		}
		else if( dice[i] == type2 )
			total2++;
		else
			return 0;
	}

	if( (total1 == 2 && total2 == 3) || (total1 == 3 && total2 == 2) )
		return 1;

	return 0;
}

// function to sort the values of the dice in ascending order
void sort_dice( int dice[5] ){
        int i, j;
        int temp;

    	for (i = 0; i < 4; i++){
        	for (j = i + 1; j < 5; j++){
            		if (dice[i] > dice[j]){
                		temp =  dice[i];
                		dice[i] = dice[j];
                		dice[j] = temp;
            		}
        	}
   	}
}

// function to check if small straight (4 or more in a row)
int check_smallStraight( int dice[5] ){
	
	sort_dice( dice );
	int i, j;
	int count = 1;

	for( i = 0; i < 4; i++ ){
		j = i + 1;
		if( dice[i] != dice[j] && dice[j] != (dice[i] + 1) )
			count = 1;
		else if( dice[i] == dice[j] )
			count = count;
		else
			count++;	
		if( count >=4 )
			return 1;
	}

	return 0;
	
}

// function to check if large straight( 5 in a row )
int check_largeStraight( int dice[5] ){

        sort_dice( dice );
        int i;
	int j = 0;
	
	for( i = dice[0]; i < dice[0]+5; i++ ){
		if( dice[j] != i )
			return 0;
		j++;
	}

	return 1;
}

// function to check if yahtzee (5 of one #)
int check_yahtzee( int dice[5] ){

	if( dice[0] == dice[1] && dice[1] == dice[2] && dice[2] == dice[3] && dice[3] == dice[4] )
		return 1;
	return 0;

}

// function to score extra yahtzees
void bonus_yahtzee( int numPlayers, int whoPlayer, struct Player players[4], int dice[5], int x ){

	players[whoPlayer].yahtzee += 100; // player gets a 100 point yahtzee bonus

	gfx_color(255,255,255);
	gfx_fill_rectangle( x, 490, 60, 45);

        gfx_text( x, 517, num2str(players[whoPlayer].yahtzee));

	// then asked to choose another category
	choose_score( numPlayers, whoPlayer, players, dice );
}

// function to calculate and display top total
void top_total( int numPlayers, int whoPlayer, struct Player players[4] ){
	
        players[whoPlayer].topTotal = 0;

        if( players[whoPlayer].ones != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].ones;
        if( players[whoPlayer].twos != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].twos;
        if( players[whoPlayer].threes != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].threes;
        if( players[whoPlayer].fours != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].fours;
        if( players[whoPlayer].fives != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].fives;
        if( players[whoPlayer].sixes != -1 )
                players[whoPlayer].topTotal += players[whoPlayer].sixes;

	if (players[whoPlayer].topTotal >= 63 && players[whoPlayer].topTotal <= 98) // bonus for top section
		players[whoPlayer].topTotal += 35; 

        int x = 481 + (whoPlayer+1) * (290/(numPlayers+1));

	gfx_color(255, 255, 255 );
	gfx_fill_rectangle( x-20, 280, 40, 20 );
	
	gfx_color( 255, 0, 0 );
	gfx_text( x, 297, num2str(players[whoPlayer].topTotal) );
}

// function to calculate and display bottom total
void  bottom_total(int numPlayers, int whoPlayer, struct Player players[4]){

	players[whoPlayer].bottomTotal = 0;

	if( players[whoPlayer].threeKind != -1 )
		players[whoPlayer].bottomTotal += players[whoPlayer].threeKind;
	if( players[whoPlayer].fourKind != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].fourKind;
	if( players[whoPlayer].fullHouse != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].fullHouse;
	if( players[whoPlayer].smallStraight != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].smallStraight;
	if( players[whoPlayer].largeStraight != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].largeStraight;
	if( players[whoPlayer].yahtzee != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].yahtzee;
        if( players[whoPlayer].chance != -1 )
                players[whoPlayer].bottomTotal += players[whoPlayer].chance;

        int x = 481 + (whoPlayer+1) * (290/(numPlayers+1));

        gfx_color(255, 255, 255 );
        gfx_fill_rectangle( x-20, 580, 40, 20 );

        gfx_color( 255, 0, 0 );
        gfx_text( x, 600, num2str(players[whoPlayer].bottomTotal) );

}

// function to calculate and display the total score
void display_total( int numPlayers, int whoPlayer, struct Player players[4]){

	players[whoPlayer].total = players[whoPlayer].bottomTotal + players[whoPlayer].topTotal;

        int x = 481 + (whoPlayer+1) * (290/(numPlayers+1));

        gfx_color(255, 255, 255 );
        gfx_fill_rectangle( x-20, 620, 40, 20 );

        gfx_color( 255, 0, 0 );
        gfx_text( x, 640, num2str(players[whoPlayer].total) );
}

// function to go to an end of game screen
void end_game( int numPlayers, struct Player players[4] ){

	int i, j; 
	int temp; 
	int max=0;
	int winner = -1;  
	
	// finds the player with the highest score
	for (i=0; i <numPlayers; i++){
		if (players[i].total > max){ 
			max  = players[i].total; 		
			winner = i; 
		}
	}

	// clears and makes a blue screen
	gfx_clear();
	gfx_color(0,0,255);
	gfx_fill_rectangle(0,0,800,700);

	// prints the winner's # and total score
	gfx_changefont("lucidasans-bolditalic-24"); 
	gfx_color(255,255,255); 
	gfx_text(75, 350, "The winner is player " ); 
	gfx_text(355, 350, num2str(winner+1) );
	gfx_text(375, 350, " with a score of "); 
	gfx_text( 580, 350, num2str(max) );
	gfx_text( 620, 350, "!!!!!" );
	
}
