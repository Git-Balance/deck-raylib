#include <raylib.h>
#include <string>
#include <vector>

/*
* Deck --> Hand, Graveyard
* Card --> *ActiveCard*
*   Or should ActiveCard be an object that just has a Card variable inside of it
* Effect (Target, Damage)
* Trigger (TrigetType)
*   Skip, effects trigger when card is played/every turn
* ActiveCard with EnemySingle
*/

struct CardColors {
    Color background;
    Color foreground;
    Color text;
    // Temp (mostly) until images for cards are added
    Color imageBackground;
};

enum TargetType {
    Itself,
    Ally,
    Enemy,
};
enum TargetPosition {
    NotPositionable,
    ActiveFirst,
    ActiveLast,
    Random, // Maybe skip this
};

struct Target {
    TargetType type;
    TargetType position;
};

// NOTE: Is it effect or Affect
struct Effect {
    TargetType target;
    int damage;
};

struct Card {
    // Health = 0 and an effectSummon is synonymous with a spell card
    std::string name;
    int health;
    // Affects
    Effect effectSummon;
    Effect effectActive;
};

class Health {
private:
    int maxHealth;
    int currentHealth;
public:
    bool isAlive() {
        return currentHealth > 0;
    }
    bool isDamagable() {
        return maxHealth > 0; // Should I add isAlive()?
    }
    Health(int maxHealth) {
        this->maxHealth = maxHealth;
        this->currentHealth = maxHealth;
    }
    bool damage(int damage) {
        currentHealth -= damage;
        return isAlive();
    }
};

class ActiveCard {
private:
    Card stats; // Just for use as "archival" data or whatever
    Health health;
    bool alive; // A lot of repetition, lol
public:
    ActiveCard(Card card);
    Effect summon();
    Effect turnEffect();
    void damage(int damage) {
        alive = health.damage(damage);
    }
    bool isAlive() { return alive; }
};
struct Field {
    std::vector<ActiveCard> cards;
};
// Essentially just a wrapper for vector
class CardPile {
private:
    std::vector<Card> cards;
public:
    void append(Card card);
    Card pop();
    Card& getFront(); // Why did I make getFront and getBack?
    Card& getBack();
    int size(); // There might be a better way of doing this
    void shuffle();
};

// Could CardPile and ActivePile be subclasses of some Pile object?
class ActivePile {
private:
    std::vector<Card> cards;
public:
    void append(ActiveCard card);
    Card pop();
    Card& getFront();
    Card& getBack();
    int size(); // There might be a better way of doing this
    void shuffle();
};
// Figure out what you want to use this for later
// Will probably be the initial 
class Deck {
private:
public:
};
struct Player {
    Health health;
    Field field;
    // Wait...Deck does not work here for hand and graveyard, since they do not have ActiveCard
    std::vector<ActiveCard> active;
    CardPile hand;
    CardPile graveyard;
};
// struct Game {}; // Maybe should be an object
class Game {
private:
    Player player1;
    Player player2;
    bool playerWon;
public:
    Game(Deck deck1, Deck deck2);
    void turn();

};

// Use a while loop for turns; a for each loop might break if the list gets altered
// Turn has steps
// Turn
//  Draw
//  Summon
//  Update
//      Steps (for each ActiveCard pair)

int main() {
    return 0;
}
