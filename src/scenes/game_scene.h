#include "app_context.h"
#include "scene.h"
#include "game/game_manager.h"
#include "save/save_data.h"

class GameScene : public Scene {
public:
    explicit GameScene(SaveData save);
    void Enter() override;
    void Leave() override;
    void Update(double dt) override;
    void Draw() override;
private:
    GameManager game;
};