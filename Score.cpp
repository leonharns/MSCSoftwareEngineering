#include "Score.h"

void InitScore(PlayerScore *pScore, int initScore) {
    pScore->score = initScore;
}

void IncreaseScore(PlayerScore *pScore, int amount) {
    pScore->score += amount;
}

int GetPlayerScore(PlayerScore *pScore) {
    return pScore->score;
}
