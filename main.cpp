//Yazan Abughazaleh
//YFA190000

//Compile command for reference
// g++ Main.cpp -Wall -Wextra -Wuninitialized -pedantic-errors -Wconversion -o a.out

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

//2D array which is declared to hold theater seating arrangement

//Function prototypes of all used functions in the program
int bestAvailable(char (&theaterMap)[10][26], int row, int numOfSeats);
void displayAuditorium(char (&theaterMap)[10][26]);
void reserveSeats(char (&theaterMap)[10][26], int row, char seat, int adult, int senior, int child);
bool checkAvailability(char (&theaterMap)[10][26], int row, char seat, int total);
void writeTheatertoFile(char (&theaterMap)[10][26]);
void displayReport(char (&theaterMap)[10][26]);
void parseFiletoArray(char (&theaterMap)[10][26]);
int countSeats(char (&theaterMap)[10][26]);
int countRows(char (&theaterMap)[10][26]);
void fillArrayEmpty(char (&theaterMap)[10][26]);

int main() {
    char theaterMap[10][26];
    //ensures no miscellaneous characters are in array
    fillArrayEmpty(theaterMap);
    //Writes individual character values in the input file to spaces in the 2d array
    parseFiletoArray(theaterMap);
    
    bool exit = true;
    int selection = 0;
    
    while(exit){
        //Presentation of the Main menu options with input for selection
        while(selection !=1  && selection !=2){
            std::cout<<"1. Reserve Seats"<<std::endl;
            std::cout<<"2. Exit"<<std::endl;
            std::cin>> selection;
        }
        //counts the number of used rows and seats in the 2d array of the theater to be able to check valid values
        int rowNum = countRows(theaterMap);
        int seatnum = countSeats(theaterMap);
        if(selection == 1){
            displayAuditorium(theaterMap);
            int row = -1;
            char startseat = '\n';
            int adultTick = -1;
            int childTick = -1;
            int seniorTick = -1;
            //UI dialog model to prompt for parameters to reserve seats, which are then passed on to the reserveSeats() function.
            //Validates inputs using while loops to reprompt if inputs are bad.
            while(!(row > 0 && row <= rowNum)){
                std::cout<<"Please select a row number: "<<std::endl;
                std::cin >> row;
            }
            while(!(startseat >64 && startseat <= seatnum + 64)){
                std::cout<<"Please select a starting seat letter: "<<std::endl;
                std::cin >> startseat;
            }
            while(!(adultTick >= 0)){
                std::cout<<"Please input the number of adult tickets to purchase: "<<std::endl;
                std::cin >> adultTick;
            }
            while(!(childTick >= 0)){
                std::cout<<"Please input the number of child tickets to purchase: "<<std::endl;
                std::cin >> childTick;
            }
            while(!(seniorTick >= 0)){
                std::cout<<"Please input the number of senior tickets to purchase: "<<std::endl;
                std::cin >> seniorTick;
            }
            //passes in all parameters to the reserveSeats function which then allows
            reserveSeats(theaterMap, row, startseat, adultTick, seniorTick, childTick);
        }
        else{
            //Writes the status of the 2d array to the A1.txt file
            writeTheatertoFile(theaterMap);
            displayReport(theaterMap);
            break;
        }
        selection = 0;
    }
    
    return 0;
}

void fillArrayEmpty(char (&theaterMap)[10][26]){
    for(int row = 0; row < 10; row++){
        for(int col = 0; col < 26; col++){
            theaterMap[row][col] = '\0';
        }
    }
}

void reserveSeats(char (&theaterMap)[10][26], int row, char seat, int adult, int senior, int child){
    //First checks if given seats to reserve are open. If they are, proceeds with reservation
    if(checkAvailability(theaterMap, row, seat, adult + senior + child)){
        int startSeat = seat - 65;
        //Loops through the quantity of adult seats and resernves them
        for(int a = 0; a < adult; a++){
            theaterMap[row-1][startSeat+a] = 'A';
        }
        startSeat+=adult;
        //Loops through the quantity of child seats and reserves them after adult seats
        for(int c = 0; c < child; c++){
            theaterMap[row-1][startSeat+c] = 'C';
        }
        startSeat+=child;
        //Loops through quantity of  senior seats and reserves them after child seats
        for(int s = 0; s < senior; s++){
            theaterMap[row-1][startSeat+s] = 'S';
        }
        std::cout<<"Seats have been reserved."<<std::endl;
    }
    //If the seats are not open, triggers the bestAvaiable function to find best seats.
    else{
        //saves the number given by bestAvailable to use in reservation and display process
        int bestSeatStart = bestAvailable(theaterMap, row, adult + senior + child);
        //Checks to seat that bestAvaiable found valid seats
        if(bestSeatStart > -1){
            //Displays range of best avaiable seats in proper format
            std::cout<<"Best available seats"<<std::endl;
            std::cout<<row<<(char)(bestSeatStart + 65)<<'-'<<row<<(char)(bestSeatStart + adult + senior + child + 64)<<std::endl;
            char resSeats = 'A';
            //Prompt for user to reserve best available seats
            while(resSeats != 'Y' && resSeats != 'N'){
                std::cout<<"Would you like to reserve these seats. Y/N";
                std::cin >> resSeats;
                //Reserves seats if user selected yes using same process shown above
                if(resSeats == 'Y'){
                    for(int a = 0; a < adult; a++){
                        theaterMap[row-1][bestSeatStart+a] = 'A';
                    }
                    bestSeatStart+=adult;
                    for(int c = 0; c < child; c++){
                        theaterMap[row-1][bestSeatStart+c] = 'C';
                    }
                    bestSeatStart+=child;
                    for(int s = 0; s < senior; s++){
                        theaterMap[row-1][bestSeatStart+s] = 'S';
                    }
                    std::cout<<"Seats have been reserved."<<std::endl;
                }
            }
        }
        //If no valid seats are found, display "no seats available"
        else{
            std::cout<<"no seats available"<<std::endl;
        }
    }
}

bool checkAvailability(char (&theaterMap)[10][26], int row, char seat, int total){
    int startSeat = seat - 65;
    //Checks to seat that given selection of seats is open. Returns false if seat selection is not avaiable
    for(int pos = 0; pos < total; pos++){
        if(theaterMap[row - 1][startSeat+pos] != '.'){
            //std::cout<<"no seats available"<<std::endl;
            return false;
        }
    }
    return true;
}

int bestAvailable(char (&theaterMap)[10][26], int row, int numOfSeats){
    //stores total number of seats in row
    int N = countSeats(theaterMap);
    int distance = N;
    int startseatum = -1;
    //Loops through number of seats to reserve to find where full seat avaiablity exists while checking all combos of seat starting positions
    for(int seat = 0; seat < (N-numOfSeats+1); seat++){
        bool seatEmpty = true;
        for(int seatPlus = 0; seatPlus < numOfSeats; seatPlus++){
            if(theaterMap[row-1][seat + seatPlus] != '.'){
                seatEmpty = false;
                break;
            }
        }
        if(seatEmpty){
            //Checks to see absolute distance from center of row. Lowest will be returned to user
            if(std::abs((((seat +1) + (numOfSeats-1)/2) - (N+1)/2)) < distance){
                distance = std::abs((((seat+1) + (numOfSeats-1)/2) - (N+1)/2));
                startseatum = seat;
            }
        }
       
    }
    //Returns a start seat num if found.
    if(startseatum > -1){
        return startseatum;
    }
    return -1;
}

void parseFiletoArray(char (&theaterMap)[10][26]){
    std::string infilename;
    //Prompt for the user to enter in a file name containing data regarding the current theater
    std::cout<<"Please enter in a file name for the seating arrangement"<<std::endl;
    std::cin >> infilename;
    std::cout<<'\n';
    std::string line;
    std::ifstream theatreFile;
    theatreFile.open(infilename);
    if( theatreFile.is_open()){
    int row = 0;
    while( getline(theatreFile, line) ){
            //Cycles through each line and assign found value to string line
            for(int i = 0; i < (int)line.length();i++){
                //Assigns each character in line to a position in the theaterMap 2d array
                theaterMap[row][i] = line[i];
            }
        //Incrementor for the row assignment
            row++;
        }
    }
        theatreFile.close();
    
}

void displayAuditorium(char (&theaterMap)[10][26]){
    char rowLetter;
    //Prints out a spacing to line up seat letters with seat positions
    std::cout<<"  ";
    //Loops through the alphabet to be able to assign a corresponding letter to each seat number
    for(int col = 0; col < 26; col++){
        rowLetter = (char)(col + 65);
        //prints out a letter for a seat only if the seat is represented be a valid character
        if(theaterMap[0][col] > 30){
            std::cout<< rowLetter;
        }
    }
    std::cout<<'\n';
    //Cycles through all seats in the theater
    for(int row = 0; row < 10; row++){
        if(theaterMap[row][0] > 0){
            std::cout<<row + 1<<" ";
        }
        for(int col = 0; col < 26; col++){
            //replaces A C and S with # on printout of theater
                if(theaterMap[row][col] == 'A' || theaterMap[row][col] == 'C' || theaterMap[row][col] == 'S'){
                    std::cout<<'#';
                
                }
                else if(theaterMap[row][col] == '.'){
                    std::cout<<theaterMap[row][col];
                }
            
        }
        //Prints a newline character only if the row contains data
        if(theaterMap[row][0] > 0){
            std::cout<<'\n';
        }
    
    }
    std::cout<<'\n';
    
}

void displayReport(char (&theaterMap)[10][26]){
    int seats = 0;
    int ticketsSold = 0;
    int adultTickets = 0;
    int childTickets = 0;
    int seniorTickets = 0;
    double totalSales = 0.0;
    //Loos through rows and columns of 2d array
    for(int row = 0; row < 10; row++){
        for(int col = 0; col < 26; col++){
                if(theaterMap[row][col] == 'A' || theaterMap[row][col] == 'C' || theaterMap[row][col] == 'S' || theaterMap[row][col] == '.'){
                    seats++;
                    if(theaterMap[row][col] == 'A' || theaterMap[row][col] == 'C' || theaterMap[row][col] == 'S'){
                        ticketsSold++;
                        //Increments number of tickets sold by type
                        switch (theaterMap[row][col]) {
                            case 'A':
                                adultTickets++;
                                break;
                            case 'C':
                                childTickets++;
                                break;
                            default:
                                seniorTickets++;
                                break;
                        }
                    }
                }
            
        }
        
    
    }
    totalSales = (adultTickets*10)+(childTickets*5)+(seniorTickets*7.50);
    std::cout<<'\n';
    std::cout<<"Total Seats:    "<<seats<<std::endl;
    std::cout<<"Total Tickets:  "<<ticketsSold<<std::endl;
    std::cout<<"Adult Tickets:  "<<adultTickets<<std::endl;
    std::cout<<"Child Tickets:  "<<childTickets<<std::endl;
    std::cout<<"Senior Tickets: "<<seniorTickets<<std::endl;
    std::cout<<"Total Sales:    $"<<std::setprecision(2)<<std::fixed<<totalSales<<std::endl;
}

void writeTheatertoFile(char (&theaterMap)[10][26]){
    std::ofstream outfile;
    int cntRows = countRows(theaterMap);
    int cntSeats = countSeats(theaterMap);
    //creates A1.txt
    outfile.open("A1.txt");
    // writes out each character from theater array to file by looping throgh lines and columns
    for(int row = 0; row < cntRows; row++){
        for(int col = 0; col < cntSeats; col++){
            if((theaterMap[row][col] == 'A' || theaterMap[row][col] == 'C' || theaterMap[row][col] == 'S') || theaterMap[row][col] == '.'){
                outfile << theaterMap[row][col];
            }
        }
        outfile << '\n';
    }
    outfile.close();
    
}

int countSeats(char (&theaterMap)[10][26]){
    int seats = 0;
    //counts the number of seats in a row by seeing which columns of the 2d array are in use
    for(int col = 0;col < 26;col++){
        if(theaterMap[0][col] > 30){
            seats++;
        }
    }
    return seats;
}

int countRows(char (&theaterMap)[10][26]){
    int rows = 0;
    //counts the number of rows in the theater by checking which rows in the 2d array are in use
    for(int row = 0;row < 10;row++){
        if(theaterMap[row][0] > 30){
            rows++;
        }
    }
    return rows;
}

