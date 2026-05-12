#include <algorithm>
#include <cassert>
#include <cstdio>
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

struct CardColors { Color background; Color foreground;
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
// "Action" is a general name for what a card is able to do
struct Action {
    /*
     * Action:
     * <TargetType> <TargetPosition> <AffectType> <AffectAmount>
     * Ally Player Damage -1
     * Enemy Mana Damage 1 # Should the mana thing be destroyable?
     * Enemy Mana Drain 1 # 
     */
    Target target;
    Affect affect;
};

struct Card {
    // Health = 0 and an affectSummon is synonymous with a spell card
    std::string name;
    CardColors colors;
    int health;
    // Affects
    Action actionSummon;
    Action actionActive;
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
    Action summon();
    Action turnAction();
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
/*
class ActiveCardPile {
private:
    std::vector<ActiveCard> cards;
public:
    void append(Card card) {
        cards.push_back(ActiveCard{card});
    }
    size_t size() {
        return cards.size();
    }
};
*/

using ActiveCardPile = std::vector<ActiveCard>;

// Returns true if index can access an element within ActiveCardPile
bool withinActiveCardPile(const ActiveCardPile& pile, int index) {
    if (pile.empty()) return false;
    if (index >= 0 && index < pile.size()) return true;
    return false;
}


// For some reason naming this struct "Player" raises the error: Must use 'struct' tag to refer to type 'Player' in this scope
// EDIT: Turns out the enum thing TargetPosition.Player was causing problems, renaming that fixed it
struct Player {
    Health health;
};
// A "Team" is just an active player and their stats
struct Team {
    Player player;
    ActiveCardPile cards;
    CardPile hand;
    CardPile graveyard;
};

Action getAction(Team team, int cardIndex) {
    return team.cards.at(cardIndex).turnAction();
}

class Field {
private:
    Team team1;
    Team team2;
public:
    Field(Team team1, Team team2) {
        team1 = team1;
        team2 = team2;
    }
    void addCard(Card card, bool isTeam1) {
        if (isTeam1) team1.cards.emplace_back(card);
        else team2.cards.emplace_back(card);
    }
    void turn() {
        // step, act, action, checks (?)

        // TODO: Get rid of this mess
        int team1Actions = team1.cards.size() - 1;
        int team2Actions = team2.cards.size() - 1;
        while (std::max(team1Actions, team2Actions) >= 0) {
            assert(std::max(team1Actions, team2Actions) >= 0);
            
            // For now, just pop the cards into the graveyards
            // TODO: Add actions and don't just delete cards

            team1.cards.pop_back();
            team1Actions -= 1;
            team2.cards.pop_back();
            team2Actions -= 1;
            printf("%zu %i\n", team1.cards.size(), team1Actions);
            printf("%zu %i\n", team2.cards.size(), team2Actions);
            printf("-----------\n");
        }

        // Every step:
        //  For each team simultaniously:
        //      If card at currentCard exists && has action:
        //          Do Action, calculate effects
        //      Check if card is alive:
        //          If dead
        //              Pop card into graveyard (convert from ActiveCard to Card? Could use "stats" variable)
        //              (wait, is this next calculation necessary?) currentCard{teamNum} -= 1
        //      currentCard -= 1

        int currentCardTeam1 = team1.cards.size() - 1;
        int currentCardTeam2 = team2.cards.size() - 1;
        while (std::max(currentCardTeam1, currentCardTeam2) >= 0) {
            if (withinActiveCardPile(team1.cards, currentCardTeam1)) {
                getAction(team1, currentCardTeam1);
            }
        }
    }
    bool isFieldEmpty();
    bool arePlayersAlive(); // Returns true if *both* players are still alive
    bool isPlayer1Dead();
    bool isPlayer2Dead();
}; 

class Game {
private:
    Field field;
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
    printf("Start field\n");
    Field testField = Field(Team(), Team());
    printf("Start cards\n");

    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), true);
    // testField.addCard(Card(), false);
    // testField.addCard(Card(), false);
    // testField.addCard(Card(), false);
    // testField.addCard(Card(), false);
    //
    printf("Start turn\n");
    //
    // testField.turn();

    Card card{};
    ActiveCard active{card};

    printf("test?\n");

    Player player{};
    Player player2{};

    Team team1{player, ActiveCardPile{}};
    Team team2{player2, ActiveCardPile{}};

    Field newField{team1, team2};
    newField.addCard(card, true);
    newField.addCard(card, true);
    newField.addCard(card, true);
    newField.addCard(card, false);
    newField.addCard(card, false);
    newField.turn();


    return 0;
}
