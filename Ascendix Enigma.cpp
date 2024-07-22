#include <iostream> // use to handle input output operations
#include <conio.h> // used for console function _getch function
#include <cstdlib> // Used for general utilities , system function to clear the screen system(cls) , srand(time(0))
// and rand() to seed the random number to put in the grid
#include <ctime>  //Used for time-related functions like time() for seeding the random number generator 
// and clock() for tracking elapsed time
#include <string> // string operations , store player name and password
#include <windows.h> // Include the Windows API header for Beep
#include <iomanip> // For better formatting, for setw
#include <limits> // For input validation Provides constants that define the limits of fundamental data types,
// useful for input validation (numeric_limits<streamsize>::max() is used to ignore invalid input).

using namespace std;

int gridSize = 2; // Start with 2x2 grid
const int maxGridSize = 10; // Maximum grid size is 10x10
int grid[10][10]; // Define the largest possible grid
string correctPassword = "0000"; // Define the initial password
string playerName; // To store the player's name
int padding = (80 - playerName.length() - 6) / 2;
int moveCount = 0; // To count the number of moves
clock_t startTime, endTime; // To track time

// Function prototypes
void initializeGrid(); // Where we generate number and swap them in the grid for every running of the program
void displayGrid(); // Display the grid on the console screen after clearing the screen
void findEmptySpace(int &x, int &y); 	// & = address operator to link the value to the variables in order to swap
// Links the function parameters to the variables to modify their values
bool isSolved(); // to check the grid is solved
void moveBlock(char direction); // get the charachter 
bool validatePassword();
void changePassword();
void getPlayerName();
void selectLevel();
void startTimer();
void endTimer();
void showElapsedTime();

int main() {
    cout << string(padding, ' ') <<"Welcome to The World of Asendix Enigma!" << endl; // Welcome message
    getPlayerName(); // Get the player's name at the start

    if (!validatePassword()) {
        return 0; // Exit if the password is incorrect
    }

    char choice;
    cout<<"Please choose an Option \n 1)Play Game\n2)Change Password"<<endl;
    cin>>choice;
    if(choice == '2'){
    	
  
          changePassword();
        cout << "Please enter the new password to continue: "<<endl;
        if (!validatePassword()) {
            return 0; // Exit if the new password is incorrect
        }
    }

    selectLevel(); // Allow the user to select the level

    while (gridSize <= maxGridSize) {
        initializeGrid();
        moveCount = 0; // Reset move count for new level
        startTimer(); // Start the timer

        while (true) {
            displayGrid();
            cout << playerName << ",Use Arrow Keys for movement of numbers \n Use # to exit: ";
            char move = _getch();
            if (move == '#') {
                cout << "Exiting the game..." << endl;
//                 showElapsedTime(); 
                return 0;
            }
            moveBlock(move);
            moveCount++; // Increment move count

            if (isSolved()) {
                endTimer(); // End the timer
                displayGrid();
                showElapsedTime(); // Show the elapsed time
                cout << "Congratulations, " << playerName << "! You solved the " << gridSize << "x" << gridSize << " puzzle in " << moveCount << " moves!" << endl;
                gridSize++;
                break;
            }
        }

        if (gridSize > maxGridSize) {
            cout << "You've completed all levels, " << playerName << "! Well Done!" << endl;
            break;
        }
    }
    return 0;
}

// Function to initialize the grid with numbers 1 to (gridSize^2 - 1) and an empty space (0)
void initializeGrid() {
    int numbers[maxGridSize * maxGridSize];
    for (int i = 0; i < gridSize * gridSize; i++) {
        numbers[i] = i;
    }

    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
    for (int i = 0; i < gridSize * gridSize; i++) {
        int j = rand() % (gridSize * gridSize);
      swap(numbers[i], numbers[j]); // Swap numbers to shuffle the grid
    }

    int index = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            grid[i][j] = numbers[index++];
        }
    }
}

// Function to display the grid with borders and spacing
void displayGrid() {
    system("cls");
    cout << "Player: " << playerName << " | Level: " << gridSize << "x" << gridSize << " | Moves: " << moveCount << endl;
    cout << "------------------------------------" << endl;
    
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            cout << "| ";
            if (grid[i][j] == 0)
                cout << "   ";
            else
                 cout << setw(2) << grid[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << "------------------------------------" << endl;
   
}
 
// Function to find the empty space (0) position
void findEmptySpace(int &x, int &y) 
{
	// Iterate through the grid to find the empty space (0)
    for (int i = 0; i < gridSize; i++) { // Check if current position is empty space
        for (int j = 0; j < gridSize; j++) {
            if (grid[i][j] == 0) {
               x = i;//Update x-coordinate with the row of the empty space
                y = j; //Update y-coordinate with the column of the empty space
                return; //Exit the function once the empty space is found
            }
        }
    }
}

// Function to check if the puzzle is solved
bool isSolved() {
    int correctNumber = 1;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (grid[i][j] != correctNumber % (gridSize * gridSize)) {
                return false;
            }
            correctNumber++;
        }
    }
    return true;
}


void moveBlock(char direction) {
    int x, y;
    findEmptySpace(x, y);

    switch (direction) {
        case 80: // Up arrow key
            if (x > 0) {
                swap(grid[x][y], grid[x - 1][y]);
            } else {
                Beep(500, 200); // Beep when trying to move up beyond the boundary
            }
            break;
        case 72: // Down arrow key
            if (x < gridSize - 1) {
                swap(grid[x][y], grid[x + 1][y]);
            } else {
                Beep(500, 200); // Beep when trying to move down beyond the boundary
            }
            break;
        case 77: // Left arrow key
            if (y > 0) {
                swap(grid[x][y], grid[x][y - 1]);
            } else {
                Beep(500, 200); // Beep when trying to move left beyond the boundary
            }
            break;
        case 75: // Right arrow key
            if (y < gridSize - 1) {
                swap(grid[x][y], grid[x][y + 1]);
            } else {
                Beep(500, 200); // Beep when trying to move right beyond the boundary
            }
            break;
        default:
            break;
    }
}

// Function to prompt the user for a password and validate it
bool validatePassword() {
    string inputPassword;
    while (true) {
      cout <<"Hey, "<<playerName << "! \n Please enter the password to access the game: "<<endl;
        cin >> inputPassword;
        if (inputPassword == correctPassword) {
            return true;
        } else {
            cout << "Incorrect password. Please try again." << endl;
        }
    }
}

// Function to change the password
void changePassword() {
    string newPassword;
    cout << "Enter a New Password: ";
    cin >> newPassword;
    correctPassword = newPassword;
    cout << "Password Changed Successfully!" << endl;
}

// Function to get the user's name
void getPlayerName() {
    cout << "Enter your name: ";
    getline(cin, playerName);
}

// Function to select the game level with input validation
void selectLevel() {
    int level;
    while (true) {
        cout << "Select level (2 to " << maxGridSize << "): ";
        cin >> level;

        if (cin.fail()) { // Check if the input is not an integer
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number between 2 and " << maxGridSize << "." << endl;
        } else if (level >= 2 && level <= maxGridSize) {
            gridSize = level;
            break;
        } else {
            cout << "Invalid level. Please select a level between 2 and " << maxGridSize << "." << endl;
        }
    }
}

// Function to start the timer
void startTimer() {
    startTime = clock();
}

// Function to end the timer
void endTimer() {
    endTime = clock();
}

// Function to show the elapsed time
void showElapsedTime() {
    double elapsed = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "Time taken: " << elapsed << "  Seconds" <<setprecision(8)<< endl;
}
