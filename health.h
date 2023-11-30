#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
    int health;
} PlayerHealth;

void InitPlayerHealth(PlayerHealth *playerHealth, int initialHealth);
void DecreasePlayerHealth(PlayerHealth *playerHealth, int amount);
int GetPlayerHealth(PlayerHealth *playerHealth);

#endif
