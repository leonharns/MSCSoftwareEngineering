#ifndef SCORE_H
#define SCORE_H

typedef struct {
    int score;
} PlayerScore;

void InitScore(PlayerScore *pScore, int initScore);
void IncreaseScore(PlayerScore *pScore, int amount);
int GetPlayerScore(PlayerScore *pScore);

#endif
