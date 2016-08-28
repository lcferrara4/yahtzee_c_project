A Yahtzee Game by Emily Obaditch and Lauren Ferrara

From the user's perspective:

When the user runs the program, he/she is prompted in the terminal window to choose how many players they want in the game. Then a window opens with a welcome message to the game. It indicates that the user should press 's' to start the game. When the user presses 's', a screen pops up with the first player, a red roll button, a green instruction on how to end the turn early, 5 blank die, and a score card with columns for each player. When the user presses the roll button, the die change to show random values between 1 and 6. The user can click on dice to save them. When clicked, they have a red border. After 3 rolls, the user is prompted to choose a category. The user has to enter what is in parenthesis on the score card to choose a category. If the user enters an improper answer, a warning message pops up in green. If the user chooses a category that has already been scored, a warning pops up. Otherwise the score is recorded on the score card for whatever they chose. The top total, bottom total, and total then change and show up in red on the score card. Then the next user must press roll to start their turn. If a user enters a space in the middle of a turn, they are prompted as if 3 rolls have taken place. If the user presses 'q' repeatedly, the game quits. After 13 rounds have been played, the screen clears, and displays a white message on a blue screen saying who the winner is and their score.

How the program works internally:

The program sets up a struct for Player that includes all score categories, top total, bottom total, total, turns, and rounds all as int variables. In main, an array of 5 ints is created to store the die. An array of 5 ints is created to save the dice ( 0 indicates a saved die ). printf writes a message to the terminal asking the user how many players are in the game. The answer is stored in numPlayers. An array of the struct Player called players is then made using numPlayers. gfx_open is called to open an 800x700 window for the game. A while loop is then entered. In this loop, the welcome message is displayed. If the user presses 's', the game is setup using the game_setup function and the check_event function is called to wait for the next user input. If 'q' is entered, the while loop is exited and the program ends.

The num2str function that Ramzi wrote is included to allow us to display scores and player numbers more easily.

game_setup is a function that sets up the board and starts the game. In it, the white score card is created using gfx_fill_rectangle(). Then horizontal and vertical blue lines are drawn to make the spaces for the categories and players on the score card using gfx_line(). The option to end the round early is put on the screen with gfx_text. Other functions are then called to finish the setup.

One of these functions is initialize_scores(). This function uses a for loop to go through the array of struct Player to initialize all the ints in that struct. Categories that can be chosen by the user are set to -1 as a check for whether the category has been scored yet. Other categories are set to 0 for adding purposes.

Another function called is print_categories(). This function uses  a double char array to hold the names of all categories with the option of how to score it in parentheses. A for loop is then used to call gfx_text() to write these categories on the score card in the right places.

Another function called is print_players(). A for loop calls gfx_text() to write the number of the players across the top of the score card.

A function refresh_saved is called in game setup to initialize all the saved array to 1, which means the die is unsaved. This is called after each roll to unsave all the die.

A function create_die is called, which draws the 5 blank white die using gfx_fill_rectangle(). In addition, gfx_text and num2str() are used to print the current player and the current number of rolls taken.

print_roll is also called. This function just creates the roll button in the middle of the screen.

Then check_event is entered, because it was called in main. 
The function checks if the player has 3 or more rolls. If he does, choose_score is called for the user to pick a category. The turns variable for the struct is changed to 0 and the rounds for that player is incremented by 1. whoPlayer is changed to the proper next player using an if-else statement to keep it from going beyond the players entered. An if-else statement checks to see if the game is over (the next players has 13 rounds). If so, end_game is called. Otherwise, it calls itself, check_event, again.
If the player does not have 3 or more rolls, gfx_wait() assigned to char d to find out what the user does next. If the user clicks the mouse (d==1) , the x and y positions of that click are stored. If they are in the region of the roll button, player turns is incremented and create_die, roll_dice, refresh_saved, and check_event are called. If the user presses in the region of any of the dice, the die gets a red border using gfx_rectangle() and the saved array in that position is changed to 0. If the user presses the space, the program calls the function end_early and assigns the int it returns to whoPlayer.  If the player presses 'q', the game quits. A default else prints an error message and asks the user to try again.
After each run through, turns is incremented, create_die, roll_dice, refresh_saved, and check_event are called.

The roll_dice function, which was called during check_event, assigns random values between 1 and 6 (using rand()%6+1) using a switch statement to each of the dice in the dice array if saved at that value is 1. Randomness is ensured by the srand(time(NULL)) in main.

end_early(), which could be called by pressing the space, does everything that would happen if 3 rolls had been completed and returns the number of the next player.

choose_score contains a switch statement to deal with the value stored in char c from gfx_wait when this function is called. The user can enter '1' (to score ones), '2' (to score twos), '3' (to score threes), '4' (to score fours), '5' (to score fives), '6' (to score sixes), 't' (to score three of a kind), 'f' (to score four of a kind), 'h' (to score full house), 's' (to score small straight), 'l' (to score large straight), 'y' (to score yahtzee), or 'c' (to score chance) to choose a category to be scored. If the user enters 'q', the program quits. If the user enters something else, a warning message occurs, and choose_score is called again. In each of the proper category options, an if statement checks if that category is -1. If it is, it scores it. If it isn't -1, it warns the user that the score has already been filled and calls choose_score again. For the number categories, if it is -1, a for loop goes through each die and uses an if statement to add all the die that are equal to the number chosen. For the other categories, a function is called to check whether the dice actually contain that option. If they do, then they are given the appropriate score. If they do not, the category is scored as 0. For three of a kind, four of a kind, and chance, the proper score is the total of all the dice which is calculated using a for loop in a separate function called total_dice. The score for full house is 25, the score for small straight is 30, and the score for large straight is 40. A first yahtzee is scored as 50. However in this case, if the score for yahtzee is greater than or equal to 50, the bonus_yahtzee function is called. This function adds 100 to the yahtzee category and then calls choose_event again for the player to score an additional category. After the switch statement is complete, functions top_total, bottom_total, and display_total are called.

In check_threeKind(), 6 int variables are made to include 3 different types and 3 totals. This is because in a three of a kind, there can be no more than 3 different numbers present. Then with a for loop and a series of if statements, the number of different dice numbers is counted. If any of these totals is greater than or equal to 3, the function returns 1. Otherwise it returns 0. It also returns 0 if more than 3 types exist.

In check_fourKind(), the values of the first and second dice array elements are stored. If neither of these equal 3 other dice array elements, 0 is returns. Otherwise, 1 is returned.

In check_fullHouse(), int values for two types and two totals are created. It works like check_threeKind in that it finds two types present. If there are more than two types of die values, it returns 0. It also returns 0 if the total of one is not equal to 2 and the total of the other is not equal to 3. Otherwise, it returns 1.

A function sort_dice() was also created to assist with the straights. This function sorts the dice values in ascending order.

check_smallStraight() calls sort_dice(). It then goes through a for loop up until the second to last element. It checks if that element doesn't equal the one after it and the one after it is not one more than the current element. If this is true, a variable count is set back to 1. If the dice and the one after it are equal, count is not changed. Otherwise, count is incremented because the next element is part of the straight. As soon as count is greater than or equal to 4, the function returns 1 because this is a small straight. If count never gets to 4, it returns 0.

check_largeStraight() cals sort_dice(). If checks to make sure that each dice array element is one more than the previous one. If so, it returns 1. Otherwise, it returns 0.

check_yahtzee() makes sure each element of the dice array is equal to the next element. If all are equal, it returns 1. Otherwise, it returns 0.

top_total() adds up categories "ones", "twos", "threes", "fours", "fives", and "sixes" as long as the category isn't currently -1 (meaning it hasn't been scored yet. If that total is great than or equal to 63 and less than or equal to 98 (hasn't already gotten the bonus), 35 points are added to the topTotal. This is then displayed in red in that category of the score card.

bottom_total() adds up categories "threeKind", "fourKind", "fullHouse", "smallStraight", "largeStraight", "yahtzee". and "chance" as long as the category isn't currently -1 (meaning it hasn't been scored yet. This is then displayed in red in that category of the score card.

display_total() adds bottomTotal and topTotal for the current player, stores this in topTotal for that player and displays it in red in that category of the score card.

end_game() finds the max total of all the players and stores that player's score and position in the players array. gfx_clear is called. gfx_fill_rectangle changed the screen to blue. The winner and his/her score is displayed in white. The user can then click 'q' to quit and close the screen.

Checking the program:

Checking the program:
The program was checked by running through the game for 1, 2, 3, and 4 players.  The program successfully displays the proper amount of players and runs according to how many players are chosen.  

To test the functions in the program, all options that the user could do were tested.  The user’s turn successfully ends early if the user presses space and the user is able to enter in the score of their choice before the program moves onto the next player.  If space was not pressed the program allowed each player three turns and then prompted for the score choice.  
The dice rolled were random each time the roll button was pressed and only the dice that were meant to be saved stayed the same.  Once the dice that the user chooses to save are clicked they cannot be unsaved until the roll button is pressed.  

The program was tested to ensure that a user is not able to enter a score for a category that already has a score.  This was tested for multiple rolls and was successful every time.  The warning message that the category has already been scored pops up and the user is able to enter a new score. 
The scoring system in the program was tested by attempting to place scores in invalid categories.  For example, if a full house was chosen yet the dice read 1, 2, 3, 4, 5, the score placed would read a zero.  The program correctly display the scores for valid category choices.  This was validated through hand calculating what the score should be and comparing to what the program outputted.  

The program’s score categories were tested by manually adding up the score on the score card and it was determined that each of the total functions work properly.  

The text that displays what roll the program is on and who is playing was tested through running through the program.  The text always displays the correct number of rolls and the correct player.  

The quit function works with one press of the ‘q’ key, as long as the user does not quit in the middle of their turn.  For example, to ensure a safe quit the user must wait until they have rolled and entered a score before quitting.  The quit function was tested at different points in the game and works with one press of the q key at the allowed time to quit (in between turns).  The program was tested to quit in the middle of a turn and it was found that the q key has to be pressed multiple times to exit the multiple loops the program is in when in the middle of a turn.  

The program’s end display was tested, and proved correct, by completing multiple games and receiving the correct output each time.  

The program is a successful version of Yahtzee.  

