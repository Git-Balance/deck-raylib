#include <raylib.h>

/*
* Deck --> Hand, Graveyard
* Card --> *ActiveCard*
*   Or should ActiveCard be an object that just has a Card variable inside of it
* Effect (Target, Damage)
* Trigger (TrigetType)
*   Skip, effects trigger when card is played/every turn
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
    AllySingle,
    AllyPlayer,
    EnemySingle,
    EnemyPlayer,
};

// NOTE: Is it effect or Affect
struct Effect {
    TargetType target;
    int damage;   
};

struct Card {
    int health;
    // Affects
}

class Deck {
private:
public:
};

int main() {
    return 0;
}
