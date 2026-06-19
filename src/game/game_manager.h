#pragma once

#include "level.h"
#include "player.h"
#include "save/save_data.h"

class GameManager {
public:
    GameManager(SaveData&& save);
    void Update(double dt);
    void Draw();
private:
    Player player;
    Level level;
};