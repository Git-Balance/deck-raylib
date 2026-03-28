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
class Deck {
private:
public:
};
struct Player {
    Health health;
    Field field;
    // Wait...Deck does not work here for hand and graveyard, since they do not have ActiveCard
    Deck deck;
    Deck hand;
    Deck graveyard;
};
// struct Game {}; // Maybe should be an object
class Game {
    Player player1;
    Player player2;

};

// Turn has steps
// Turn
//  Draw
//  Summon
//  Update
//      Steps (for each ActiveCard pair)

int main() {
    return 0;
}
