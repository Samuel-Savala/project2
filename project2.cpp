/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   project2.cpp
 * 
 *
 * Created on June 3, 2026, 10:53 PM
 */
/**
 * @file.project2.cpp
 * @brief GoFish Card Game expanded with functions
 * practice inheritance, classes etc.
 * classes are used with crd, dek, pHand, player
 * Inheritance with HumanP1, easycpu, hardcpu from player
 * operator overload (==, !=, << on class
 * @author Samuel Savala
 * @date 6/7/2026
 * 
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>


using namespace std;

/**
 * @brief To show a single playing card with rank and suit
 */
class crd{
private:
    char rank; ///<rank of card
    char suit; ///< suit of card
    
public:
    /**
     * @brief constructor to create an empty card
     */
    crd() : rank(' '), suit(' ') {}
    
    /**
     * @brief constructor - create a card with a specific rank & suit
     */
    crd(char r, char s) : rank(r), suit(s) {}
    /**
     * @brief return the rank of the card
     * @return char rank of card
     */
    char getRank() const {return rank; }
    /**
     * @brief return the suit of the card
     * @return char suit of card
     */
    char getSuit() const {return suit; }

    /**
     * @brief compare two cards by their rank
     * @param other is the opposite card to compare
     * @return truwe if the rank matches
     */
    bool operator ==(const crd &other) const{
        return rank == other.rank;
    }
    /**
     * @brief Check if two cards have different ranks
     */
    bool operator !=(const crd &other) const{
        return rank != other.rank;
    }
    
    /**
     * @brief print card in format
     * @param out output stream reference
     * c Card prints
     */
    friend ostream& operator<<(ostream &out, const crd &c){
        out << c.rank << c.suit;
        return out;
    }
};
/**
 * @brief Template for the deck
 */

template <class T>
class dek{
private:
    crd crds[52]; // 52 card deck
    int left; // variable of remaining cards
    int top; // index of next available card
    
public:
    /**
     * @brief initializing the empty deck
     */
    dek() : left(0), top(0) {}
    /**
     * @brief building the 52 card deck
     */
    void dekbld(){
        const char ranks[] = "A23456789TJQK";
        const char suits[] = "CDHS";
        int index = 0;
        
        for (int s = 0; s < 4; s++){
            for(int r = 0; r < 13; r++){
                crds[index] = T(ranks[r], suits[s]);
                index++;
            }
        }
        left = 52;
        top = 0;
    }
    /**
     * @brief shuffles the deck with swap function. think fisher yates
     */
    void shuffle(){
        for(int i = 51; i > 0; i--){
            int j = rand() % (i + 1);
            if(i == j) continue;
            
            T temp = crds[i];
            crds[i] = crds[j];
            crds[j] = temp;
            
        }
    }
    /**
     * @brief deals the top card of deck
     */
    T dealCrd(){
        T deal = crds[top];
        top++;
        left--;
        return deal;
    }
    /**
     * @brief returns # of cards remaining in deck
     */
    int crdLeft() const {return left;}
    /**
     * @brief check if the deck is empty
     */
    bool empty() const {return left == 0;}
    
    
};
/**
 * @brief overarching class for players
 */

template <class T>
class pHand{
private:
    T cards[52]; //maimum cards a player can technically hold t holds any data type
    int count; // number of cards in the players hand
public:
    /**
     * @brief initialize an empty hand
     */
    pHand() : count(0) {};
    /**
     * @brief adding card to hand
     */
    void adCrd(T item){
        if (count < 52){
            cards[count] = item; // puts item in next opening
            count++;
        }
    }
    /**
     * @brief returns # of items in hand
     * @return count
     */
    int getCount() const {return count;}
    /**
     * @brief look at a by specific spot
     */
    T getCrd(int i) const {
        return cards[i];
    }
    /**
     * @brief will remove all items matching
     * @param target item matching and take
     */
    
    int takeCrd(T target){
        int remove = 0;
        T temp[52];
        int nCount = 0;
        
        for(int i = 0; i < count; i++){
            if(cards[i] == target){
                remove++;
            }
            else {
                temp[nCount] = cards[i];
                nCount++;
            }
        }
        for( int i = 0; i < nCount; i++){
            cards[i] = temp[i];
        }
        count = nCount;
        return remove;
    }
    
    /**
     * @brief checking if hand has a specific card
     * @return true if found
     */
    bool hasCrd(T target) const{
        for(int i = 0; i < count; i++){
            if(cards[i] == target){
                return true; // will loop through cards. if a match is found it's true
            }
        }
        return false;
    }
    
};


class player{
protected: //this and other classes can "inherit" it like an open door to apply for human and PC player
    char name[20];
    pHand<crd> hand; ///< hand template - hold any object
    int books;
    
public:
    /**
     * @brief constructor to set up player name
     * @param n name to assign to a player
     */
    player(const char *n) : books(0){ ///< initialize books at 0
        strcpy(name, n); ///< copy name into name array
    }
    virtual ~player(){} ///< virtual so derived classes clean up memory when object is deleted
    /**
     * @brief virtual function for taking turns
     * @param other array for other players
     * @param nOthers # of other players in game
     */
    virtual bool tkTurn(player *other[], int nOthers, dek<crd> &dek) = 0;
    // = 0 derived classes are forced to provide their own functions
        
    /**
     * @brief returns the players name
     * @return pointer to the name string
     */
    
    const char* getName() const{return name;}
    
    /**
     * @brief returns # of books a player has
     */
    int getBook() const {return books;}
    
    /**
     * @brief return # of cards in hand
     */
    int hndCont() const{return hand.getCount();}
    /**
     * @brief return reference to players hand
     * will allow derived classes to interact with hand
     */
    pHand<crd>& getHand() {return hand;} // & makes it a reference. remember
    
    void fulBok(){
        const char ranks[] = "A23456789TJQK";
        
        for(int r = 0; r < 13; r++){
            int cnt = 0;
            
            for(int i = 0; i < hand.getCount(); i++){
                if(hand.getCrd(i).getRank() == ranks[r]){
                    cnt++;
                }
            }
            
            if(cnt == 4){
                crd target(ranks[r], ' ');
                hand.takeCrd(target);
                books++;
                cout << name << "Gained a book of " << ranks[r] << "'s" << endl; 
            }
        }
    }
    /**
    * @brief print out the players hand
    * use operator << from crd class to display cards
    */
    void prntHnd() const{ //const so it only reads data - hand.getCount to get cards in hand currently
        cout << endl << name << "'s hand (" << hand.getCount() << " cards: ";
        
        //loops through the players hand printing each card
        for(int i = 0; i < hand.getCount(); i++){
            cout << hand.getCrd(i) << " ";
        }
        cout << endl;
    }
    
    /**
     * @brief draw card from the deck and add to hand
     * @return true if card is drawn, false if deck is empty
     */
    bool drawCrd(dek<crd> &dek){ // reference of the real deck
        if(dek.empty()){
            cout << "Deck is empty..." << endl;
            return false;
        }
        crd nCard = dek.dealCrd();
        hand.adCrd(nCard);
        cout << name << " drew a new card." << endl;
        return true;
    }
};

/**
 * @brief Logic for a human player in the game
 * Inheritance from class player
 */
class HumanP1 : public player {
public:
    /**
     * @brief Human Player constructor
     * @param n = human player name
     */
    HumanP1(const char *n) : player(n){}
    /**
     * @brief Allow player to choose which player to ask and what card to request
     * @return true if player gets another turn false if opposite
     */
    bool tkTurn(player *other[], int nOthers, dek<crd> & dek){
        //checking empty hand
        if(hand.getCount() == 0){
            drawCrd(dek); ///< inheritance of drawCrd
            return false;
        }
        
        prntHnd(); //show hand
        
        //asking for card to choose
        char rank;
        
        bool valid = false;
        
        while(!valid){
            cout << endl << name << " pick a card to ask for: ";
            
            cin >> rank;
            
            rank = toupper(rank);
            //if statement for player to have it in hand
            crd targetCrd(rank, ' ');
            
            if(hand.hasCrd(targetCrd)){
                valid = true;
            }
            else{
                cout << "Card not in hand... Ask for another: " << endl;
            }
        }
        
        int targeti = 0;
        
        if(nOthers > 1){ //if just one player we skip but if not we ask to pick
            cout << "Pick an opponent to ask for card: " << endl;
            for(int i = 0; i < nOthers; i++)//go through cpu listing
                cout << " " << (i + 1) << ". " << other[i]->getName() << endl;
        }
        
        bool vChoice = false;
        
        while (!vChoice){
            cout << "Enter choice (1-" << nOthers << "): ";
            cin >> targeti;
            targeti--;
            if(targeti >= 0 && targeti < nOthers){
                vChoice = true;
            }
            else{
                cout << "Invalid Choice." << endl;
            }
        }
    
    /**
     * @brief ask opponent you chose for a card
     */
    player *target = other[targeti];
    cout << name << " asks " << target->getName() << ", Do you have any "
            << rank << "'s?" << endl;
    
    crd crdLook(rank, ' '); //searching for card
    
    if(target->getHand().hasCrd(crdLook)){ //to check and get card
        int taken = target->getHand().takeCrd(crdLook);
        cout << target->getName() << " had " << taken
                << " card(s). Transfer... " << endl; //check if opponent has cards
        
        //add card to your hand
        for(int i = 0; i < taken; i++){
            crd c(rank, ' ');
            hand.adCrd(c);
        }
        fulBok();
        cout << name << " goes again." << endl;
        return true;;
    }
    else{
        cout << "Go Fish!" << endl;
        drawCrd(dek);
        fulBok(); //will check if you have a full book of 4 to add score
        return false;
    }
    }
};


/**
 * @brief template class to hold any data type in hand
 */


/**
 * @brief class for computer players
 * inherits player. will also be inherited to easy and hard cpu
 */
class cpuP : public player{
public:
    /**
     * @brief cpu constructor
     * @param n The name of the cpu player
     */
    cpuP(const char *n) : player(n) {}
    
    /**
     * @brief virt function for picking a rank also so easy and hard can use their own
     */
    virtual char pkRank() = 0;
    /**
     * automated cpu turn
     * @param others arr of pointers to other players
     * @param nOthers # of other players
     * @param deck reference
     * @return true for cpu to have +1 turn or false if not
     */
    bool tkTurn(player *others[], int nOthers, dek<crd> &dek){
        if(hand.getCount() == 0){
            drawCrd(dek);
            return false;
        }
        cout << "--- " << name << "'s Turn ---" << endl;
        char rank = pkRank(); // cpu will pick the card
        
        int targeti = rand() % nOthers; // cpu will pick a random player
        player *target = others[targeti];
        
        cout << name << " asks " << target->getName() << ", Do you have any "
                << rank << "'s?" << endl;
        
        crd crdLook(rank, ' ');
        if(target->getHand().hasCrd(crdLook)){
            int taken = target->getHand().takeCrd(crdLook);
            cout << target->getName() << " had " << taken << 
                    " card(s). Transfer..." << endl;
            
            for( int i = 0; i < taken; i++){
                crd c(rank, ' ');
                hand.adCrd(c);
            }
            fulBok();
            cout << name << " goes again." << endl;
            return true;
        }
        else{
            cout << "Go Fish!" << endl;
            drawCrd(dek);
            fulBok();
            return false;
        }
    }        
    
};

/**
 * @brief derived player class for easy cpu difficulty player
 */
class easyCPU : public cpuP{
public:
    /** easyCPU constructor
     */
    easyCPU(const char *n) : cpuP(n){}
    
    /**
     * @brief pick a random rank from hand
     */
    char pkRank(){
        int randomi = rand() % hand.getCount();//random number between hand size
        char cRank = hand.getCrd(randomi).getRank();
        return cRank; //get the rank and return
    }
    
};

/**
 * @brief Harder difficulty cpu player ineherited from cpuP
 */
class hardCPU : public cpuP{
public:
    /**
     * @brief hardCPU constructor
     */
    hardCPU( const char *n) : cpuP(n){}
    
    /**
     * @brief picks card rank of majority that the cpu has
     */
    
    char pkRank(){
        const char ranks[] = "A23456789TJQK";
        int icount = 0;
        char bRank = hand.getCrd(0).getRank();
        
        for(int r = 0; r < 13; r++){
            int cnt = 0; // loops through all 13 ranks
        
        
            for(int i = 0; i < hand.getCount(); i++){// count cards in hand
            if(hand.getCrd(i).getRank() == ranks[r]){ // match for the rank
                cnt++;
            }
        }
        
            if(cnt > icount){ //there's a better rank
                icount = cnt; // update count 
                bRank = ranks[r]; //update which rank has that count and return it
            }
        }
        return bRank;
    }
    
};

/**
 * @brief Save game result to a text file
 */
void svGame(player *players[], int totalP, const char *winName){
    ofstream outFile("gofishstats.txt", ios::app); //will add to the file instead of overwriting
    
    if(!outFile.is_open()){
        cout << "Error: Could not open stats file." << endl;
        return;
    }
    
    time_t now = time(0); // get current time
    char *dt = ctime(&now);
    
    outFile << "-----------------------------" << endl;
    outFile << "Game Played: " << dt;
    outFile << "Winner: " << winName << endl;
    outFile << "--Final Score--" << endl;
    
    for(int p = 0; p < totalP; p++){
        outFile << players[p]->getName() << ": " << 
                players[p]->getBook() << " books" << endl;
    }
    
    outFile << endl;
    outFile.close();
    
    cout << "Game Stats saved to gofish_stats.txt" << endl;
    
}

    

int main(int argc, char** argv) {
/**
 * @brief main function to run go fish game
 * use player pointer for different player types
 * @return 0 will exit game
 */
    srand(time(NULL)); // random generator
    
    bool playAgn = true;
    
    while(playAgn){
        char humanN[20];
        cout << "Go Fish Game. " << endl;
        cout << "Enter your name: " << endl;
        cin >> humanN; // set your name
        
        int numCPUs = 0; // variable for cpu number.
        while(numCPUs < 1 || numCPUs > 3){ // have to chose between 1-3 opponents
            cout << "Please choose total number of opponents (1-3): " << endl;
            cin >> numCPUs;
            if(numCPUs < 1 || numCPUs > 3){ // if outside 1-3 you have to choose again between
                cout << "Please choose again between 1-3: " << endl;
            }
        }
        
        char level;
        bool vLevel = false;
        while(!vLevel){
            cout << "Choose Computer Difficulty between Easy(E) or Hard(H): " << endl;
            cin >> level;
            level = toupper(level);
            if(level == 'E' || level == 'H'){
                vLevel = true;
            }
        }
        int totalP = numCPUs + 1;
        player *players[4]; // array to have 4 max players. you and 3 cpus max
        
        players[0] = new HumanP1(humanN); //human player
        
        char cpuName[3][20] = {"CPU 1", "CPU 2", "CPU 3"};
        for(int i = 0; i < numCPUs; i++){ // set new cpus as either easy or hard setting class to it
            if(level == 'E') {
                players[i + 1] = new easyCPU(cpuName[i]);
            }
            else{
                players[i + 1] = new hardCPU(cpuName[i]);
            }
        }
        dek<crd> gDeck;
        gDeck.dekbld();
        gDeck.shuffle(); // build and shuffle deck
        
        for(int round = 0; round < 7; round++){ //deal 7 cards per player0
            for( int p = 0; p < totalP; p++){
                crd deal = gDeck.dealCrd();
                players[p]->getHand().adCrd(deal);
            }
        }
        
        for(int p = 0; p < totalP; p++){
            players[p]->fulBok();
        }
        
        
        int turnNow = 0;
        int numBooks = 0;
        
        while(numBooks < 13){
            cout << endl << "--- Total Score ---" << endl;
            for( int p = 0; p < totalP; p++){
                cout << players[p]->getName() << ": " << players[p]->getBook() <<
                        " Books." << endl; // this will pull up the current score
            }
            cout << "deck: " << gDeck.crdLeft() << " cards left." << endl;
            
            player *others[3];
            int nOthers = 0;
            for(int p = 0; p < totalP; p++){
                if(p != turnNow){
                    others[nOthers] = players[p];
                    nOthers++;
                }
            }
            //take turn
            bool another = true;
            while(another){
                another = players[turnNow]->tkTurn(others, nOthers, gDeck);
            }
            numBooks = 0;
            for(int p = 0; p < totalP; p++){
                players[p]->fulBok();
                
                numBooks += players[p]->getBook(); // get the total book count
            }
            
            bool alEmpty = (gDeck.crdLeft() == 0);
            if(alEmpty){
                for(int p = 0; p < totalP; p++){
                    if(players[p]->hndCont() > 0){
                        alEmpty = false;
                        break;
                    }
                }
            }
            if(alEmpty) break;
            turnNow = (turnNow + 1) % totalP;
        }
        
        //For final results
        cout << endl << "Game Over.";
        int maxBook = 0;
        for(int p = 0; p < totalP; p++){
            cout << players[p]->getName() << ": " << players[p]->getBook() << 
                    " Books" << endl;
            if(players[p]->getBook() > maxBook){
                maxBook = players[p]->getBook();
            }
        }
        cout << endl << "Winner(s): ";
        for(int p = 0; p < totalP; p++){
            if(players[p]->getBook() == maxBook){
                cout << players[p]->getName() << ". ";
            }
        }
        cout << endl;
        
        //find winner name for stats file
        const char *winName = "Tie";
        int winCnt = 0;
        for(int p = 0; p < totalP; p++){
            if(players[p]->getBook() == maxBook){
                winName = players[p]->getName();
                winCnt++;
            }
        }
        if(winCnt > 1) winName = "Tie";
        
        //Save to text file
        svGame(players, totalP, winName);
        
        for (int p = 0; p < totalP; p++){
            delete players[p]; // virtual destructor to clean up memory
        }
        
        char again;
        cout << endl << "Play Again? Y/N: ";
        cin >> again;
        again = toupper(again);
        if(again != 'Y'){
            playAgn = false;
        }
    }
    cout << "Thank you for playing." << endl;
    
    return 0;
}

