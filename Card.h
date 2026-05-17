#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

class Card {
    public:
        Card(int n){
            int r = n % 13; //Determine rank
            if (r == 1){
                rank = "A";
                value = 11;
            }
            else if (r == 11){
                rank = "J";
                value = 10;
            }
            else if (r == 12){
                rank = "Q";
                value = 10;
            }
            else if (r == 0){
                rank = "K";
                value = 10;
            }
            else{
                rank = std::to_string(r);
                value = r;
            }

            int s = (n-1) / 13; // Determine suit

            switch (s){
                case 0:
                    suit = "S";
                    break;
            case 1:
                    suit = "D";
                    break;
            case 2:
                    suit = "C";
                    break;
            case 3:
                    suit = "H";
                    break;
            }
        }
        Card(){
            rank = "";
            value = 0;
            suit = "";
        }
        
        std::string getSuit() const {return suit;}

        int getValue() const {return value;}

        void setValue(int v) {value = v;}

        std::string getRank() const {return rank;}

        void printCard() const {
            std::cout << rank << " of " << suit;
        }

    private:
        std::string rank;
        int value;
        std::string suit;
};

#endif
  