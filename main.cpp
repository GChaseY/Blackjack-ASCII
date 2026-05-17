#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <limits>
#include <cstdlib>

#include "Card.h"

void startGame();

double getWager(double);

void playRound(double&, double&, bool);

void rules();

void drawCard(std::vector<Card>&, std::vector<int>&);

int getHandValue(std::vector<Card>&);

void printHand(std::vector<Card>&);

bool yourTurn(std::vector<Card>&, std::vector<int>&, std::vector<Card>&, double&, double&, bool);

bool dealerTurn(std::vector<Card>&, std::vector<int>&, int, double&, double&, bool);

bool enablePushing();


int main(){
    srand(time(0));

    startGame();

    double playerMoney = 100.00;
    double wager = getWager(playerMoney);

    bool push {enablePushing()};
    
    if (wager == -1){ // Ends program if not enough money
        return 0;
    }
    else {
        playerMoney -= wager;

        std::cout << "You wagered $" << wager << ".  Best of luck to you.\n" << std::endl;
        rules(); // Displays the rules

        playRound(playerMoney, wager, push);

        return 0;
    }
}

void startGame(){ //Text to start game
    std::cout << "========================================" << std::endl;
    std::cout << "=          WELCOME TO ASCII            =" << std::endl;
    std::cout << "=              BLACKJACK               =" << std::endl;
    std::cout << "=                                      =" << std::endl;
    std::cout << "========================================" << std::endl;
}

double getWager(double m){ // Gets wager
    double w {};
    std::cout << "Your balance is: $" << m << std::endl;
    std::cout << "How much would you like to wager?  Enter wager here: ";

    while (!(std::cin >> w)){
        std::cout << "Invalid input. Try again: ";
        std::cin.clear(); // Reset input errors
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Remove bad input
    }

    if (w <= 0){ // No money
        std::cout << "No money?  Come back when you're a bit... richer.  Heheh.\n" << std::endl;
        return -1;
    }
    else{
        while (w > m){ // Too much wagered
            std::cout << "Hey buddy, I REALLY wouldn't do that if I were you.  Go lower.  Enter wager here: ";
            std::cin >> w;

            if (w <= 0){
                std::cout << "Yeah, that's right, go home now!\n" << std::endl;
                return -1;
            }
        }
        return w;
    }
}

void playRound(double& m, double& w, bool push){
    std::vector<Card> yourHand;
    std::vector<Card> dealerHand;

    std::vector<int> usedCards;

    for (int i = 0; i < 2; i++){
        drawCard(yourHand, usedCards);
        drawCard(dealerHand, usedCards);
    }

    std::cout << "Dealer's hand:" << std::endl;
    printHand(dealerHand);

    std::cout << "Your hand: " << std::endl;
    printHand(yourHand);

    bool result;
    bool blackjack {false};

    if (getHandValue(dealerHand) == 21){
        std::cout << "You lose!  (Dealer got a blackjack)" << std::endl;
        result = false;
    }
    else if (getHandValue(yourHand) == 21){
        std::cout << "BLACKJACK! YOU WIN!" << std::endl;
        result = true;
        blackjack = true;
    }
    else{
        result = yourTurn(yourHand, usedCards, dealerHand, m, w, push);
    }

    if (result){
        if (blackjack){m += w*5;}
        else {m += w*3;}
    }

     std::cout << "Now you have $" << m << "." << std::endl;
     std::cout << "Play again? (y/N) ";

    std::string again;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, again);

    if (again == "y" || again == "Y"){
        w = getWager(m);
        if (w == -1){ // Ends program if not enough money
            exit(0);
        }
        else {
            push = enablePushing();
            m -= w;
            
            std::cout << "You wagered $" << w << ".  Best of luck to you.\n" << std::endl;
            playRound(m, w, push);
        }
    }
    else{
        exit(0);
    }
}

void rules(){
    std::cout << "Rules:" << std::endl;
    std::cout << "Try to get a 21 with your cards.  Face cards are worth 10, aces are worth 1 or 11." << std::endl;
    std::cout << "Over 21 and you lose.  Closest value to 21 is the winner.  Ties go to the house." << std::endl;
    std::cout << "If you win normally, you get 3x your wager.  Blackjacks give 5x your wager." << std::endl;
    std::cout << "Now LET THE GAMES BEGIN!\n" << std::endl;
}

void drawCard(std::vector<Card>& hand, std::vector<int>& dupes){
    int n = (rand() % 52) + 1;
    int cnt =  count(dupes.begin(), dupes.end(), n);

    while (cnt > 0){ // Checks to see if card is already drawn, if it is, reroll
        n = (rand() % 52) + 1;
        cnt =  count(dupes.begin(), dupes.end(), n);
    }

    Card card = Card(n);
    hand.push_back(card);
    dupes.push_back(n);
}

int getHandValue(std::vector<Card>& hand) {
    int value {0};

    for (auto i = 0; i < hand.size(); i++) {
        value += hand[i].getValue();
    }

    if (value > 21){
        int aces = 0;

        for (auto i = 0; i < hand.size(); i++) {
            if (hand[i].getRank() == "A"){
                aces++;
            }
        }

        while (value > 21 && aces > 0){
            value -= 10;
            aces--;
        }
    }

    return value;
}

void printHand(std::vector<Card>& hand){
    // Pre-pad ranks to 2 chars
    std::vector<std::string> ranks;
    for (int i = 0; i < hand.size(); i++){
        std::string r = hand[i].getRank();
        if (r.length() == 1) r = r + " ";
        ranks.push_back(r);
    }

    // Top border
    for (int i = 0; i < hand.size(); i++) {std::cout << " -------  ";}
    std::cout << "\n";

    // Rank top-left
    for (int i = 0; i < hand.size(); i++) {std::cout << "| " << ranks[i] << "    | ";}
    std::cout << "\n";

    // Suit middle
    for (int i = 0; i < hand.size(); i++) {std::cout << "|   " << hand[i].getSuit() << "   | ";}
    std::cout << "\n";

    // Rank bottom-right
    for (int i = 0; i < hand.size(); i++) {std::cout << "|     " << ranks[i] << "| ";}
    std::cout << "\n";

    // Bottom border
    for (int i = 0; i < hand.size(); i++) {std::cout << " -------  ";}
    std::cout << "\n";
    std::cout << "Total value: " << getHandValue(hand) << std::endl;
}

bool yourTurn(std::vector<Card>& hand, std::vector<int>& used, std::vector<Card>& dealer, double& m, double& w, bool push){
    int input;

    std::cout << "Your turn!" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "1) Hit - Draw a card" << std::endl;
    std::cout << "2) Stay - End your turn" << std::endl;
    std::cout << "3) Rules" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "What will you do? ";

    while (true){
        while (!(std::cin >> input)){
            std::cout << "Invalid input. Try again: ";
            std::cin.clear(); // Reset input errors
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Remove bad input
        }

        if (input == 1){ // Hit
            drawCard(hand, used);

            std::cout << "Your hand: " << std::endl;
            printHand(hand);

            if (getHandValue(hand) > 21){
                std::cout << "You lose! (Busted with a value of " << getHandValue(hand) << ")" << std::endl;
                return false;
            }
            else if (getHandValue(hand) == 21){
                std::cout << "You win!" << std::endl;
                return true;
            }
            else{
                std::cout << "\nWhat will you do? ";
            }
        }

        else if (input == 2){
            int val = getHandValue(hand);
            bool result = dealerTurn(dealer, used, val, m, w, push);

            return result;
        }

        else if (input == 3){
            rules();
            
            std::cout << "What will you do? ";
        }
    }
}

bool dealerTurn(std::vector<Card>& hand, std::vector<int>& used, int playerVal, double& money, double& wager, bool push){
    std::cout << "\nDealer's turn!" << std::endl;
    while (getHandValue(hand) <= 17){
        drawCard(hand, used);
    }

    std::cout << "Dealer's hand: " << std::endl;
    printHand(hand);

    if (getHandValue(hand) > 21){
        std::cout << "You win! (Dealer busted with a hand of " << getHandValue(hand) << ")" << std::endl;
        return true;
    }
    else if (playerVal > getHandValue(hand)) {
        std::cout << "You win! (Your hand was greater than the dealer's)" << std::endl;
        return true;
    }
    else if (playerVal == getHandValue(hand) && push){
        std::cout << "It's a tie!  Wager refunded." <<std::endl;
        money += wager;
        return false;

    }
    std::cout << "You lose! (Dealer had a greater hand or tied)" << std::endl;
    return false;
}

bool enablePushing(){
    std::string enablePush;

    std::cout << "Enable pushes? If you tie, your wager is refunded. (y/N) ";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, enablePush);

    if (enablePush == "y" || enablePush == "Y") {return true;}
    else {return false;}
}