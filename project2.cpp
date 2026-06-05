/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   project2.cpp
 * Author: saval
 *
 * Created on June 3, 2026, 10:53 PM
 */
/**
 * @file.project2.cpp
 * @brief GoFish Card Game expanded with functions
 * practice inheritance, classes etc.
 * @author Samuel Savala
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

struct dek{
    crd crds[52]; // 52 card deck
    int left; // variable of remaining cards
    int top; // index of next available card

};

// *d is our pointer * means to go to that address
void dekbld (dek *d){
    const char ranks[] = "A23456789TJQK"; // all the different cards in a deck
    const char suits[] = "CDHS"; // = clubs, diamonds, hearts, spades

    int index = 0; // will keep track of which of the 52 cards to fill

    // goes through our suits CDHS
    for (int s = 0; s < 4; s++){

        for (int r = 0; r < 13; r++){

            d->crds[index].rank = ranks[r];
            d->crds[index].suit = suits[s];
            index++;

        }
    }
    d->left = 52; //pointer will go to address and can change the value
    d->top = 0;
}

void shufdek(dek *d){
    //imagine the deck dwindling down as you pick up cards. so start at top of deck
    for(int i = 51; i > 0; i--){
        //picks at random
        int j = rand() % (i + 1);
        if(i == j) continue;
        
        //swap formula
        
        crd temp = d->crds[i];
        d->crds[i] = d->crds[j];
        d->crds[j] = temp;
        
    }
}

struct pHand{
    crd cards[52]; //maimum cards a player can technically hold
    int count; // number of cards in the players hand
};

struct p1{ //about player 1
    char name[20];//For players name
    pHand h; //cards in players hand currently referencing pHand struct
    int books; // numbers of books player has scored
    
};

//function to add a card to players hand
void adCrd(pHand *h, crd c){
    h->cards[h->count] = c; //pust card in the next open slot
    h->count++; // increments players current hand size
}

// function to check if hand has card of a specific rank
int hasCrd(const pHand *h, char rank){
    for(int i = 0; i < h->count; i++){
        if(h->cards[i].rank == rank)
            return 1; // return if found in hand
    }
    return 0; //return if not found in hand
}

//function to give up cards if you have an opponent matches
int takeCrd(pHand *h, char rank){
    int remove = 0;
    int newC = 0;
    crd temp[52];
    
    for (int i = 0; i < h->count; i++){
        if(h->cards[i].rank == rank){
            remove++; //card is counted as removed
        }
        else{
            temp[newC] = h->cards[i]; // will keep the card
            newC++;
        }
    }
    //will move the kept cards back in your hand
    for(int i = 0; i < newC; i++)
        h->cards[i] = temp[i];
    
    h->count = newC; //will update your hand
    return remove;
    
}

//function to check for a complete book in hand
void fulBok(p1 *p){
    const char ranks[] = "A23456789TJQK";
    
    for(int r = 0; r < 13; r++){
        int cnt = 0;
        
        //will check for how many of a specific card is in hand
        for(int i = 0; i < p->h.count; i++){
            if(p->h.cards[i].rank == ranks[r])
                    cnt++;
        }
        // This is to mark a complete book/point
        if(cnt == 4){
            takeCrd(&p->h, ranks[r]); // calling function
            p->books++; //adds a book to player
            cout << p->name << " Gained Book " << ranks[r] << "s." << endl;
        }
    }
    
}

// function to deal cards to players
void dealCrd(dek *d, p1 *p, p1 *p2){
    for(int i = 0; i < 7; i++){ // will start with 7
        adCrd(&p->h, d->crds[d->top]);
        d->top++;
        d->left--;
        
        adCrd(&p2->h, d->crds[d->top]);
        d->top++;
        d->left--;
    }
        
}

//function to draw from deck to hand
int drawCrd(dek *d, p1 *p){
    if(d->left == 0){
        cout << "No cards left... " << endl;
        return 0;
    }
    
    adCrd(&p->h, d->crds[d->top]); // player will take the top card
    d->top++; //adds the card on top to player p hand
    d->left--; //minus one card from the overall deck
    cout << p->name << " received a new card." << endl;
    return 1;
}

void prntHnd(const p1 *p){
    cout << endl << p->name << "'s hand (" << p->h.count << " cards): ";
    for(int i = 0; i < p->h.count; i++){
        cout << p->h.cards[i].rank << p->h.cards[i].suit;
    }
    cout << endl;
}

//file i/o
void svGame(const p1 *p, const p1 *p2, const dek *d){
    FILE *f = fopen("gofish.dat", "wb"); //open file to write in
    if(f == NULL){
        cout << "Save Game Error... ";
        return;
    }
    fwrite(p, sizeof(p1), 1, f); //write in for player 1
    fwrite(p2, sizeof(p1), 1, f); //write in player 2
    fwrite(d, sizeof(dek), 1, f); //for the deck
    fclose(f);
    cout << "Game Saved." << endl;
    
}

void LdGame(p1 *p, p1 *p2, dek *d){
    FILE *f = fopen("gofish.dat", "rb"); //open file to write in
    if(f == NULL){
        cout << "No Save Data... ";
        return;
    }
    fread(p, sizeof(p1), 1, f); //read back in for player 1
    fread(p2, sizeof(p1), 1, f); //read back in player 2
    fread(d, sizeof(dek), 1, f); //for the deck
    fclose(f);
    cout << "Game Loaded." << endl;
}

//loop for turns
int turns(p1 *asking, p1 *asked, dek *pool){
    //won't take a turn if the hand is empty
    if(asking->h.count == 0){
        drawCrd(pool, asking);
        return 0;
    }
    prntHnd(asking);
    
    //player will choose card to ask about
    char rank;
    int valid = 0;
    
    while(!valid){
        cout << endl << asking->name << " pick a card to ask for: ";
        cin >> rank;
        rank = toupper(rank); // will help to automatically capitalize
        
    //for a rank they actually have
        if(hasCrd(&asking->h, rank))
            valid = 1;
        else
            cout << "Error: Ask for a rank you hold... " << endl;
    }
    
    //ask the other person
    cout << asking->name << " asks " << asked->name << ", Do you have " << rank << "'s" << endl;
    
    if(hasCrd(&asked->h, rank)){
        int taken = takeCrd(&asked->h, rank);
        cout << asked->name << " had " << taken << " Card. Transfer... " << endl;
        
    //adding card to whoever asked hand
        for(int i = 0; i < taken; i++){
            crd c;
            c.rank = rank;
            c.suit = '?';
            adCrd(&asking->h, c);
        }
        fulBok(asking);
        cout << asking->name << " go again." << endl;
        return 1;
    }
    else{
        cout << "Go Fish!" << endl;
        drawCrd(pool, asking);
        fulBok(asking);
        return 0;
    }
}

//computer initialize
char pickRnk(const pHand *h){
    const char ranks[] = "A23456789TJQK";
    int best = 0;
    
    char bstRank = h->cards[0].rank;
    
    for(int r = 0; r < 13; r++){
        int cnt = 0;
        for(int i = 0; i < h->count; i++){
            if(h->cards[i].rank == ranks[r])
                cnt++;
        }
        if(cnt >best){
            best = cnt;
            bstRank = ranks[r];
        }
    }
    return bstRank;
}


//computer turn
int pcTurn(p1 *asking, p1 *asked, dek *pool){
    if(asking->h.count == 0){
        drawCrd(pool, asking);
        return 0;
    }
    
    cout << "---Computer's Turn---" << endl;
    char rank = pickRnk(&asking->h); //automatica pick card
    
    cout << asking->name << " asks " << asked->name <<": Do you have " << rank << "'s" << endl;
    
    if (hasCrd(&asked->h, rank)){
        int taken = takeCrd(&asked->h, rank);
        cout << asked->name << " had " << taken << " card. Transfer..." << endl;
        
        for(int i = 0; i < taken; i++){
            crd c;
            c.rank = rank;
            c.suit = ' ';
            adCrd(&asking->h, c);
        }
        fulBok(asking);
        cout << asking->name << " goes again." << endl;
        return 1;
    }
    else{
        cout << "Go Fish!" << endl;
        drawCrd(pool, asking);
        fulBok(asking);
        return 0;
    }
    
    
}

int main(int argc, char** argv) {
//random function
    srand(time(NULL));
    
    
    dek myDeck;
    
    p1 p, p2;
    
    //initialize hands and books
    p.h.count = 0;
    p.books = 0;
    p2.h.count = 0;
    p2.books = 0;
    
    //input player names
    cout << "Enter name for Player 1: ";
    cin >> p.name;
    cout << "Enter name for Player 2: ";
    strcpy(p2.name, "Computer");
    
    //input functions for building, shuffling and dealing the deck
    dekbld(&myDeck);
    shufdek(&myDeck);
    dealCrd(&myDeck, &p, &p2);
    
    //check books/sets
    fulBok(&p);
    fulBok(&p2);
    
    //save to file
    svGame(&p, &p2, &myDeck);
    
    //main loop
    int turn = 0;
    int ttlbook = 0;
    int again;
    
    while(ttlbook < 13 ){
        cout << "Books - " << p.name << ": " << p.books << "||" << p2.name << ": " << p2.books << endl;
        cout << "Deck: " << myDeck.left << " cards left." << endl;
        
        again = 1;
        
        while(again){
            if(turn == 0)
                again = turns(&p, &p2, &myDeck);
            else
                again = pcTurn(&p2, &p, &myDeck);
        }
        ttlbook = p.books + p2.books;
        
        //check for empty hand and deck
        if(p.h.count == 0 && p2.h.count == 0 && myDeck.left == 0)
            break;
        turn = 1 - turn; // turn is obselete
    }
    
    cout << "--- Game Over! ---" << endl;
    cout << p.name << " : " << p.books << " complete sets. " << endl;
    cout << p2.name << " : " << p2.books << " complete sets. " << endl;
    
    //to decide who wins - game outcome
    if(p.books > p2.books)
        cout << p.name << " wins." << endl;
    else if(p2.books > p.books)
        cout << p2.name << " wins." << endl;
    else
        cout << "Tied game" << endl;
    
    return 0;
}

