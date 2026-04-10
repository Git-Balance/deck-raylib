#include <list>
#include <raylib.h>
#include <string>
#include <vector>

/*
* Deck --> Hand, Graveyard
* Card --> *ActiveCard*
*   Or should ActiveCard be an object that just has a Card variable inside of it
* Affect (Target, Damage)
* Trigger (TrigetType)
*   Skip, affects trigger when card is played/every turn
* ActiveCard with EnemySingle
*/

/*
 * A Storage class might be useful
 *  Stuff like CardPile and ActiveCardPile share a lot of functions like append()
 *  I could make a class with a template and make CardPile and other stuff be subclasses of it
 *  However, one of the biggest points of this project is getting it done, and templates have caused "problems" in the past
 *  So yeah, I am not going to make a Storage class right now
 *  Just writing down my thoughts
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


enum AffectType {
    Damage,
    Drain,
};

struct Affect {
    AffectType type;
    int affectAmount;
};
// "Spell" is a general name for what a card is able to do
// TODO: Rename "Spell" to "Action"
struct Spell {
    /*
     * Spell:
     * <TargetType> <TargetPosition> <AffectType> <AffectAmount>
     * Ally Player Damage -1
     * Enemy Mana Damage 1 # Should the mana thing be destroyable
     * Enemy Mana Drain 1 # 
     */
    TargetType target;
    Affect affect;
};

struct Card {
    // Health = 0 and an affectSummon is synonymous with a spell card
    std::string name;
    CardColors colors;
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
    Health() {
        maxHealth = 0;
        maxHealth = 0;
    }
    bool damage(int damage) {
        currentHealth -= damage;
        return isAlive();
    }
};

class ActiveCard {
private:
    std::string name;
    Card stats; // Just for use as "archival" data or whatever
    Health health;
    bool alive; // A lot of repetition, lol
public:
    ActiveCard(Card card) : health(Health(card.health)) {
        stats = card;
        name = card.name;
        if (health.isDamagable()) {
            alive = true;
        } else {
            alive = false;
        }
    }
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
    std::vector<Card> cards;
public:
    Deck() {
        
    }
    void addCards(Card baseCard, int numCopies); // Append baseCard numCopies amount of times
    void addCards(std::list<Card> addCards);
};
class CardPile {
private:
    std::vector<Card> cards;
public:
    CardPile();
    void append(Card card) {
        cards.push_back(card);
    }
    // Append baseCard numCopies amount of times
    void appendCards(Card baseCard, int numCopies) {
        for (int card; card<numCopies; card++) {
            append(baseCard);
        }
    }
    void appendCards(std::list<Card> addCards);
    Card pop(); // Pop end
    Card pop(int index);
};
/*
* ActiveDeck contains "deck", hand, and graveyard
*   These probably can all just be Deck/CardPile bojects
* Field contains ActiveCards for *both players*
*   Think of the field as the battlefield where engagements take place
*   It will not contain, say, a player's hand
*/
class ActiveCardPile {
private:
    std::vector<ActiveCard> cards;
public:
    void append(Card card) {
        cards.push_back(ActiveCard{card});
    }
};

// For some reason naming this struct "Player" raises the error: Must use 'struct' tag to refer to type 'Player' in this scope
// EDIT: Turns out the enum thing TargetPosition.Player was causing problems, renaming that fixed it
struct Player {
    Health health;
};
struct Team {
    Player player;
    ActiveCardPile cards;
};
class Field {
private:
    Team team1;
    Team team2;
public:
    Field(Player player1, Player player2) {}
    void addCard(Card card, bool isTeam1) {
        team1.cards.append(card);
    }
    void turn() {

    }
    bool isFieldEmpty();
    bool arePlayersAlive(); // Returns true if *both* players are still alive
    bool isPlayer1Dead();
    bool isPlayer2Dead();
}; 

class Game {
private:
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
