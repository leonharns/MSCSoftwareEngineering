#include "health.h"

void InitPlayerHealth(PlayerHealth *playerHealth, int initialHealth) {
    playerHealth->health = initialHealth;
}

void DecreasePlayerHealth(PlayerHealth *playerHealth, int amount) {
    playerHealth->health -= amount;
    if (playerHealth->health < 0) {
        playerHealth->health = 0;
    }
}

int GetPlayerHealth(PlayerHealth *playerHealth) {
    return playerHealth->health;
}
