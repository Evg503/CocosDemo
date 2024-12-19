#pragma once

#include "cocos2d.h"

constexpr int PLAYER_TAG = 1;
constexpr int ENEMY_TAG = 2;

struct DragonData
{
    cocos2d::Vec2 pos{0, 0};
    cocos2d::Vec2 speed{0, 0};
    cocos2d::Rect pos_bound{{0, 0}, {0, 0}};
    cocos2d::Vec2 maxspeed{500, 500};
    void up()
    {
        speed.y += maxspeed.y;
    }
    void down()
    {
        speed.y -= maxspeed.y;
    }
    void right()
    {
        speed.x += maxspeed.x;
    }
    void left()
    {
        speed.x -= maxspeed.x;
    }
    void stop()
    {
        speed.x = 0;
        speed.y = 0;
    }
    bool update(float delta)
    {
        if (!speed.isZero())
        {
            pos += speed * delta;
            pos.clamp(pos_bound.origin, pos_bound.origin + pos_bound.size);
            return true;
        }
        return false;
    }
};
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void update(float delta) override;

    CREATE_FUNC(GameScene);
    bool onContactBegin(cocos2d::PhysicsContact &contact);

private:
    void addBackButton();
    void addActor();
    void addEnemy(const cocos2d::Vec2 &position);
    cocos2d::Sprite *createDragonAnimation(bool isFlip);
    void emitEnemyDragons(float interval);

    DragonData _dragon_data;

    cocos2d::Sprite *_sprite = nullptr;
    cocos2d::Label *_label = nullptr;
    cocos2d::Texture2D *_texture = nullptr;
};
