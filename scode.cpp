#include <iostream>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

//converts a three large boolean array into an integer
int bin2dec(bool one, bool two, bool three);
//converts an integer to a boolean array (key)
bool* dec2bin(int num);
//displays a line and also writes it to save.txt
void displayLine(bool line[]);
//generates a random bool array size 8 (the key)
bool* randomKey();
//processes user's choice on what key they choose
bool* processKeyChoice(int choice);
//gives a 3 bit number
int leftMiddleRight(bool currentLine[], int i);


//runs the game of life
int gameOfLife(int maxGenerations);

	//TO DO
//input a binary number; output the binary number as a list of booleans/integers
bool* bin2barray(string bin);
//input a binary number as a list of booleans/integers; output a binary number
int barray2bin(bool* []);
//read the lines from a save file
string readFromFile();
//let user create the initial generation
int createInitialGeneration();

const int bits = 8;		//bits in the key

int lineSize = 79;		//the amount of bits in each generation
int generation = 0;		//lööp time
ofstream myfile;		//saving to file works until line 9 after which it stops adding newlines for some reason FIX

int main(){
	int maxGenerations;
	int keyChoice;
	myfile.open("save.txt");
	srand(time(NULL));		//sets the randomness seed based on the system clock
	bool currentLine[lineSize];
	bool nextLine[lineSize];

	//user input for the size of the line
	do{
		cout << "Line Size: (Max 79) ";
		cin >> lineSize;
		cin.clear();
		cin.ignore(10000, '\n');
	}while(lineSize < 1 or lineSize > 100 or cin.fail());

	//user input for amount of max cycles
	do{
		cout << "Number of generations: (0 for infinite) ";
		cin >> maxGenerations;
		cin.clear();
		cin.ignore(10000, '\n');
	}while(maxGenerations < 0 or cin.fail());

	//user input for key options
	do{
		cout << "1) Default Key (30)\n2) Enter Key\n3) Random Key\n4) 3D Game of Life" << endl;
		cin >> keyChoice;
		cin.clear();
		cin.ignore(10000, '\n');
	}while(keyChoice < 1 or keyChoice > 5 or cin.fail());

	if(keyChoice == 4){
		gameOfLife(maxGenerations);
		myfile.close();
		return 0;
	}
	bool *key = processKeyChoice(keyChoice);

	//filling both lines with 0s and then adding one 1 in the middle of the first line
	for(int i = 0; i < lineSize; i++){
		currentLine[i] = false;
		nextLine[i] = false;
	}
	currentLine[lineSize/2] = true;

	//displays the key before the automaton
	cout << "Key: ";
	for(int i = 0; i < bits; i++){
		cout << key[i];
	}
	cout << endl;

	//draws top of the box
	for(int i = 0; i < lineSize + 4; i++){
		cout << "=";
	}
	cout << endl;

	displayLine(currentLine);

	//generation loop
	while (true){
		usleep(250000);		//waits

		//changes the next line based on the current line
		for(int i = 0; i < lineSize; i++){
			switch(leftMiddleRight(currentLine, i)){
				case 0 :nextLine[i] = key[7];
						break;
				case 1 :nextLine[i] = key[6];
						break;
				case 2 :nextLine[i] = key[5];
						break;
				case 3 :nextLine[i] = key[4];
						break;
				case 4 :nextLine[i] = key[3];
						break;
				case 5 :nextLine[i] = key[2];
						break;
				case 6 :nextLine[i] = key[1];
						break;
				case 7 :nextLine[i] = key[0];
						break;
			}
		}

		displayLine(nextLine);

		//makes the current line equal to the next line
		for(int i = 0; i < lineSize; i++){
			currentLine[i] = nextLine[i];
		}

		//checks if the generation is the last one
		if(maxGenerations != 0 and generation == maxGenerations)
			break;
		generation++;
	}
	for(int i = 0; i < lineSize + 4; i++){
		cout << "=";
	}
	cout << endl;
	myfile.close();
	return 0;
}

//converts a three large boolean array into an integer
int bin2dec(bool one, bool two, bool three){
	return one * 4 + two * 2 + three * 1;
}

//converts an integer to a boolean array (key)
bool* dec2bin(int num){
	bool *key = new bool[bits];
	num++;
	for (int i = 0; i < bits; i++){
		if(0 < (num - pow(2, (bits - 1 - i) )) ){
			key[i] = true;
			num = num - pow(2, (bits - 1 - i) );
		}
		else{
			key[i] = false;
		}
	}
	return key;
}

//displays a line and also writes it to save.txt
void displayLine(bool line[]){
	cout << "||";
	for(int i = 0; i < lineSize; i++){
		if(line[i] == true){
			cout << "@";
			myfile << "@";
		}
		else{
			cout << " ";
			myfile << " ";
		}
	}
	cout << "||";
	cout << endl;
	myfile << endl;
}

//generates a random bool array size 8 (the key)
bool* randomKey(){
	bool *key = new bool[bits];
	for(int i = 0; i < bits; i++){
		key[i] = rand()%2;
	}
	return key;
}

//processes user's choice on what key they choose
bool* processKeyChoice(int choice){
	switch(choice){
		case 1: {
			bool *key = dec2bin(30);
			return key;
		}
		case 2: {
			int intKey;
			do{
				cout << "Enter number 0-255: ";
				cin >> intKey;
				cin.clear();
				cin.ignore(10000, '\n');
			}while(intKey < 0 or intKey > 255 or cin.fail());
			bool *key = dec2bin(intKey);
			return key;
		}
		// case 3: {
		// 	string boolKey;
		// 	cout << "Enter boolean number 0000000-1111111: ";
		// 	cin >> boolKey;
		// 	bool *key = bin2barray(boolKey);
		// 	return key;
		// }
		default: {
			bool *key = randomKey();
			return key;
		}
	}
}

//gives a 3 bit number
int leftMiddleRight(bool currentLine[], int i){
	int read;
	if(i == 0){
		read = bin2dec(false, currentLine[i], currentLine[i+1]);
	}
	else if(i == lineSize - 1){
		read = bin2dec(currentLine[i-1], currentLine[i], false);
	}
	else{
		read = bin2dec(currentLine[i-1], currentLine[i], currentLine[i+1]);
	}
	return read;
}

//couldn't quite get these two to work
//input a binary number; output the binary number as a list of booleans/integers
 bool* bin2barray(string bin){
 	int binSize = sizeof(bin)/sizeof(bin[0]);
 	bool *barray = new bool[binSize];
 	for(int i = 0; i < binSize; i++){
 		barray[i] = bin[i];
 		cout << barray[i] << " " << bin[i] << " " << endl;
 	}
 	 cout << endl;

 	return barray;
 }

 //input a binary number as a list of booleans/integers; output a binary number
  int barray2bin(bool barray[], int barraySize){
 	string str[barraySize];
	stringstream number;
	int bin;
	for(int i = 0; i < barraySize; i++){
 		str[i] = barray[i];
		number<<str[i];
 	}
 	number >> bin;
 	return bin;
 }

//read the lines from a save file
string readFromFile(){
	string line;
  ifstream myfile ("save.txt");
  if (myfile.is_open())
  {
    getline (myfile,line);
    myfile.close();
		return line;
  }
	else return "";
}

//let user create the initial generation
//use read from file :)
int createInitialGeneration(){
	return 0;
}

//runs the game of life
int gameOfLife(int maxGenerations){
	int counter, neighbor, generation = 1;
	bool currentLine[lineSize + 2][lineSize + 2];
	bool nextLine[lineSize + 2][lineSize + 2];

    //initialize arrays
	for (int i = 0; i < lineSize + 2; i++) {
    	for (int j = 0; j < lineSize + 2; j++) {
	        currentLine[i][j] = 0;
	    	nextLine[i][j] = 0;
    	}
  	}

    //randomize living cells
    for (int i = 1; i <= lineSize; i++) {
    	for (int j = 1; j <= lineSize; j++) {
      		currentLine[i][j] = rand()%2;
      		nextLine[i][j] = currentLine[i][j];
    	}
  	}

  	//Main loop - count neighbors, kill/resurrect/leave cells
  	do{
    	//initialize counter and print Game of Life
    	counter = 0;
      	cout<<"Generation: " << generation << endl << flush;
	    for(int i = 0; i < lineSize + 4; i++){
	        cout << "=";
	    }
	    cout << endl;

	    //make Game of Life 3D by copying outer columns and rows on the other side of table
	    for (int i = 1; i <= lineSize; i++) {
	        for (int j = 1; j <= lineSize; j++) {
	        	if(j == 1){
	          		nextLine[i][lineSize + 1] = nextLine[i][j];
	        	}
	        	else if(j == lineSize){
	          		nextLine[i][0] = nextLine[i][j];
	        	}
	        	if(i == 1){
	            	nextLine[lineSize+1][j] = nextLine[i][j];
	        	}
	        	if(i == lineSize){
	            	nextLine[0][j] = nextLine[i][j];
	        	}
	      	}
	    }

	    //count neighbors
	    for (int i = 1; i <= lineSize; i++) {
	        for (int j = 1; j <= lineSize; j++) {
	        	neighbor = 0;
	          	if(nextLine[i - 1][j - 1])
	            	neighbor++;
	          	if(nextLine[i][j - 1])
	            	neighbor++;
	          	if(nextLine[i + 1][j - 1])
	            	neighbor++;
	          	if(nextLine[i - 1][j + 1])
	            	neighbor++;
	          	if(nextLine[i - 1][j])
	            	neighbor++;
	          	if(nextLine[i][j + 1])
	            	neighbor++;
	          	if(nextLine[i + 1][j])
	            	neighbor++;
	          	if(nextLine[i + 1][j + 1])
	            	neighbor++;

		        //rules of Game of life
		        //if cell has less than 2 or more than 3 cells, kill it
		        if(neighbor < 2 || neighbor > 3){
		            if(nextLine[i][j] != 0){
		            	counter++;
		            }
		            currentLine[i][j] = 0;
		        }
		        //if cell has 3 neighbors, resurrect it
		        else if (nextLine[i][j] == 0 && neighbor == 3){
		            currentLine[i][j] = 1;
		            counter++;
		        }
		        //else keep it as it is
		        else{
		            currentLine[i][j] = nextLine[i][j];
		        }

		        //draw the Game of Life
		        if(j == 1){
		        	cout << "||";
		        }

		        if(currentLine[i][j] == 1){
		        	cout << "@";
		        	// myfile << "@";
		        }
		        else{
		        	cout << " ";
		        	// myfile << " ";
		        }
		        if(j == lineSize){
		        	cout << "||";
		        }
	      	}
	    cout << endl;
	    // myfile << endl;
	    }

	    //count generations
	    generation++;

	    //copy new currentLine to temporary
	    for (int i = 0; i < lineSize + 2; i++) {
	      	for (int j = 0; j < lineSize + 2; j++) {
	        	nextLine[i][j] = currentLine[i][j];
	      	}
	    }

	    for(int i = 0; i < lineSize + 4; i++){
	      cout << "=";
	    }
	    cout << endl << flush;
    	usleep(250000);

	}while(counter != 0 && maxGenerations != generation);
    cout << "====generations: " << generation - 2 << "======" << endl << flush;
    return 0;
}
