#include "UNO.h"
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>
#include <map>

using namespace std;

// card info
struct CardInfo {
    string col;
    string val;

    bool matches(const CardInfo &other) const {
        return col == other.col || val == other.val;
    }
};

// keeps track of game stuff
class GameData {
public:
    int numPlayers;
    vector<vector<CardInfo>> playerCards;
    vector<CardInfo> discardPile;
    vector<CardInfo> deck;
    int currentTurn;
    int direction; // 1 for clockwise, -1 for counter

    GameData(int n) : numPlayers(n), currentTurn(0), direction(1) {
        playerCards.resize(numPlayers);
    }

    // create the deck
    vector<CardInfo> buildDeck() {
        vector<CardInfo> cards;
        vector<string> colors = {"Red", "Blue", "Green", "Yellow"};

        for(auto &color : colors) {
            // just one 0
            cards.push_back({color, "0"});

            // everything else comes in pairs
            for(int i = 1; i <= 9; i++) {
                cards.push_back({color, to_string(i)});
                cards.push_back({color, to_string(i)});
            }

            // action cards
            cards.push_back({color, "Skip"});
            cards.push_back({color, "Skip"});
            cards.push_back({color, "Reverse"});
            cards.push_back({color, "Reverse"});
            cards.push_back({color, "Draw Two"});
            cards.push_back({color, "Draw Two"});
        }
        return cards;
    }

    void setup() {
        deck = buildDeck();

        // shuffle using fixed seed
        mt19937 gen(1234);
        shuffle(deck.begin(), deck.end(), gen);

        // deal cards
        int pos = 0;
        for(int p = 0; p < numPlayers; p++) {
            playerCards[p].clear();
            for(int i = 0; i < 7; i++) {
                playerCards[p].push_back(deck[pos++]);
            }
        }

        // starter card
        discardPile.push_back(deck[pos++]);

        // remove dealt cards from deck
        deck.erase(deck.begin(), deck.begin() + pos);
    }

    // pick best card to play
    int findBest(const vector<CardInfo> &hand, const CardInfo &top) {
        int bestIdx = -1;
        int bestPrio = 999;

        for(int i = 0; i < hand.size(); i++) {
            if(!hand[i].matches(top)) continue;

            int prio = 999;

            // color match beats everything
            if(hand[i].col == top.col) {
                prio = 1;
            }
            // then value match
            else if(hand[i].val == top.val) {
                // actions get special priority
                if(hand[i].val == "Skip") prio = 3;
                else if(hand[i].val == "Reverse") prio = 4;
                else if(hand[i].val == "Draw Two") prio = 5;
                else prio = 2;
            }

            if(prio < bestPrio) {
                bestPrio = prio;
                bestIdx = i;
            }
        }
        return bestIdx;
    }

    void play() {
        if(finished()) return;

        CardInfo topCard = discardPile.back();
        auto &hand = playerCards[currentTurn];

        // see if we can play something
        int cardIdx = findBest(hand, topCard);

        if(cardIdx != -1) {
            // play the card
            CardInfo card = hand[cardIdx];
            hand.erase(hand.begin() + cardIdx);
            discardPile.push_back(card);

            // check for special cards
            if(card.val == "Skip") {
                // next player gets skipped
                currentTurn = (currentTurn + direction + numPlayers) % numPlayers;
            }
            else if(card.val == "Reverse") {
                // change direction
                direction *= -1;
            }
            else if(card.val == "Draw Two") {
                // next player draws and gets skipped
                int next = (currentTurn + direction + numPlayers) % numPlayers;
                for(int i = 0; i < 2 && !deck.empty(); i++) {
                    playerCards[next].push_back(deck.back());
                    deck.pop_back();
                }
                currentTurn = (next + direction + numPlayers) % numPlayers;
                return;
            }
        }
        else {
            // gotta draw
            if(!deck.empty()) {
                CardInfo drawnCard = deck.back();
                deck.pop_back();

                // try playing it right away
                if(drawnCard.matches(topCard)) {
                    discardPile.push_back(drawnCard);

                    // check if drawn card is special
                    if(drawnCard.val == "Skip") {
                        currentTurn = (currentTurn + direction + numPlayers) % numPlayers;
                    }
                    else if(drawnCard.val == "Reverse") {
                        direction *= -1;
                    }
                    else if(drawnCard.val == "Draw Two") {
                        int next = (currentTurn + direction + numPlayers) % numPlayers;
                        for(int i = 0; i < 2 && !deck.empty(); i++) {
                            playerCards[next].push_back(deck.back());
                            deck.pop_back();
                        }
                        currentTurn = (next + direction + numPlayers) % numPlayers;
                        return;
                    }
                }
                else {
                    // add to hand
                    hand.push_back(drawnCard);
                }
            }
        }

        // move to next player
        currentTurn = (currentTurn + direction + numPlayers) % numPlayers;
    }

    string getStatus() const {
        ostringstream s;
        s << "Player " << currentTurn << "'s turn, Direction: "
          << (direction == 1 ? "Clockwise" : "Counter-clockwise")
          << ", Top: " << discardPile.back().col << " " << discardPile.back().val
          << ", Players cards: ";

        for(int p = 0; p < numPlayers; p++) {
            s << "P" << p << ":" << playerCards[p].size();
            if(p < numPlayers - 1) s << ", ";
        }
        return s.str();
    }

    bool finished() const {
        for(auto &hand : playerCards) {
            if(hand.empty()) return true;
        }
        return false;
    }

    int getWinner() const {
        for(int i = 0; i < numPlayers; i++) {
            if(playerCards[i].empty()) return i;
        }
        return -1;
    }
};

// storage for all games
static map<const UNOGame*, GameData*> allGames;

static GameData* getData(const UNOGame* game) {
    return allGames.count(game) ? allGames[game] : nullptr;
}

UNOGame::UNOGame(int numPlayers) {
    allGames[this] = new GameData(numPlayers);
}

void UNOGame::initialize() {
    GameData* data = getData(this);
    if(data) data->setup();
}

void UNOGame::playTurn() {
    GameData* data = getData(this);
    if(data) data->play();
}

bool UNOGame::isGameOver() const {
    GameData* data = getData(this);
    return data ? data->finished() : true;
}

int UNOGame::getWinner() const {
    GameData* data = getData(this);
    return data ? data->getWinner() : -1;
}

string UNOGame::getState() const {
    GameData* data = getData(this);
    return data ? data->getStatus() : "";
}
