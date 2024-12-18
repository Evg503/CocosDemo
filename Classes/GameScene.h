#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void update(float delta) override;

    CREATE_FUNC(GameScene);

private:
    void addBackButton();
    void addActor();
    float _dragon_y = 0;
    float _dragon_speed = 0;

    cocos2d::Sprite *_sprite = nullptr;
    cocos2d::Label *_label = nullptr;
    cocos2d::Texture2D *_texture = nullptr;
};
