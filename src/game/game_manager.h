#pragma once

#include "level.h"
#include "player.h"

class GameManager {
public:
    void Update(double dt);
    void Draw();
private:
    Player player;
    Level level;
};