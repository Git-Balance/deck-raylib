#include <list>
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
    TargetNotPositionable = -1,
    TargetActiveFirst,
    TargetActiveLast,
    TargetRandom, // Maybe skip this
    TargetPlayer,
};

struct Target {
    TargetType type;
    TargetType position;
};


enum EffectType {
    Damage,
    Drain,
};

// NOTE: Is it effect or Affect
struct Effect {
    EffectType type;
    int effectAmount;
};
// "Spell" is a general name for what a card is able to do
struct Spell {
    /*
     * Spell:
     * <TargetType> <TargetPosition> <EffectType> <EffectAmount>
     * Ally Player Damage -1
     * Enemy Mana Damage 1 # Should the mana thing be destroyable
     * Enemy Mana Drain 1 # 
     */
    TargetType target;
    Effect effect;
};

struct Card {
    // Health = 0 and an effectSummon is synonymous with a spell card
    std::string name;
    int health;
    // Affects
    Spell spellSummon;
    Spell spellActive;
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
    Spell summon();
    Spell turnSpell();
    void damage(int damage) {
        alive = health.damage(damage);
    }
    bool isAlive() { return alive; }
};
// Figure out what you want to use this for later
// Will probably be the initial 
class Deck {
private:
    // Cards originalCards
    // Cards cardsThatAreDrawn
public:
    void addCards(Card baseCard, int numCopies); // Append baseCard numCopies amount of times
    void addCards(std::list<Card> addCards);
};
class CardPile {
private:
    std::vector<Card> cards;
public:
    void append(Card card);
    Card pop(); // Pop end
    Card pop(int index);
};
/*
* ActiveDeck contains "deck", hand, and graveyard
*   These probably can all just be Deck/CardPile bojects
* Field contains ActiveCards for *both players*
*   Think of the field as the battlefield where engagements take place
*/
class ActiveCardPile {};
// For some reason naming this struct "Player" raises the error: Must use 'struct' tag to refer to type 'Player' in this scope
// EDIT: Turns out the enum thing TargetPosition.Player was causing problems, renaming that fixed it
struct Player {
    Health health;
};
class Field {
private:
    Player& player1;
    Player& player2;
public:
    Field(Player& playerA, Player& playerB) : player1(playerA), player2(playerB) {} // Check that the constructor is initializing player1 & 2 correctly, you only just learned how to do this
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
