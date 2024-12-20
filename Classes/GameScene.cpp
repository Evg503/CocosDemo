#include "common.h"
#include "GameScene.h"
#include <algorithm>

cocos2d::Scene *GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{

    if (!Scene::init())
    {
        return false;
    }
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    scheduleUpdate();
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event *event)
    {
        switch (key_code)
        {
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            _dragon_data.up();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _dragon_data.down();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _dragon_data.left();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _dragon_data.right();
            break;

        default:
            return;
            break;
        }
    };
    listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event *event)
    {
        switch (key_code)
        {
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            _dragon_data.down();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _dragon_data.up();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _dragon_data.right();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _dragon_data.left();
            break;

        default:
            return;
            break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto bg = cocos2d::Sprite::create("res/background.png");
    auto bg_size = bg->getContentSize();
    bg->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto scale = std::max(visibleSize.width / bg_size.width, visibleSize.height / bg_size.height);
    bg->setScale(scale);
    addChild(bg, -5);
    _label = cocos2d::Label::create();
    _label->setPosition(300.0f, 30.0f);
    _label->setString("+100");
    addChild(_label);
    addBackButton();
    _texture = cocos2d::Director::getInstance()->getTextureCache()->addImage("dragon_animation-hd.png");
    addActor();

    // Start emitting enemy dragons every 3 seconds
    emitEnemyDragons(3.0f);
    // Set up collision detection
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}
void GameScene::addActor()
{
    if (_sprite)
    {
        removeChild(_sprite);
        _sprite = nullptr;
    }

    //
    // Animation using Sprite BatchNode
    //
    _sprite = createDragonAnimation(true);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    float dx = 207;
    float dy = 207;
    _dragon_data.pos = {dx / 2, visibleSize.height / 2};
    _dragon_data.pos_bound.setRect(dx / 2, dy / 2, visibleSize.width / 2, visibleSize.height - dy / 2);

    _sprite->setPosition(_dragon_data.pos);
    addChild(_sprite);
}
void GameScene::addBackButton()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto backItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        [](cocos2d::Ref *)
        { cocos2d::Director::getInstance()->popScene(); });

    if (backItem == nullptr ||
        backItem->getContentSize().width <= 0 ||
        backItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - backItem->getContentSize().width / 2;
        float y = origin.y + backItem->getContentSize().height / 2;
        backItem->setPosition(cocos2d::Vec2(x, y));
    }
    auto menu = cocos2d::Menu::create(backItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);
}

void GameScene::update(float delta)
{
    if (_dragon_data.update(delta))
    {
        _sprite->setPosition(_dragon_data.pos);
    }
}
cocos2d::Sprite *GameScene::createDragonAnimation(bool isFlip)
{
    float dx = 207;
    float dy = 207;

    // manually add frames to the frame cache
    auto frame0 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 0, dy * 0, dx, dy));
    auto frame1 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 1, dy * 0, dx, dy));
    auto frame2 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 2, dy * 0, dx, dy));
    auto frame3 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 3, dy * 0, dx, dy));
    auto frame4 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 0, dy * 1, dx, dy));
    auto frame5 = cocos2d::SpriteFrame::createWithTexture(_texture, cocos2d::Rect(dx * 1, dy * 1, dx, dy));

    // Animation using Sprite BatchNode
    auto sprite = cocos2d::Sprite::createWithSpriteFrame(frame0);

    // Add physics body
    auto physicsBody = cocos2d::PhysicsBody::createBox(sprite->getContentSize());
    physicsBody->setContactTestBitmask(true);
    sprite->setPhysicsBody(physicsBody);

    // Set tag for identification
    sprite->setTag(isFlip ? PLAYER_TAG : ENEMY_TAG);

    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames(6);
    animFrames.pushBack(frame0);
    animFrames.pushBack(frame1);
    animFrames.pushBack(frame2);
    animFrames.pushBack(frame3);
    animFrames.pushBack(frame4);
    animFrames.pushBack(frame5);

    auto animation = cocos2d::Animation::createWithSpriteFrames(animFrames, 0.2f);
    auto animate = cocos2d::Animate::create(animation);
    sprite->setFlippedX(isFlip);
    sprite->runAction(cocos2d::RepeatForever::create(animate));
    return sprite;
}

void GameScene::emitEnemyDragons(float interval)
{
    this->schedule([this](float dt)
                   {
        // Create the enemy dragon sprite
        auto enemyDragon = createDragonAnimation(false); // Assuming createDragonAnimation can be reused for enemy dragons

        // Set a random position for the enemy dragon
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        float x = cocos2d::RandomHelper::random_real(visibleSize.width / 2, visibleSize.width);
        float y = cocos2d::RandomHelper::random_real(0.0f, visibleSize.height);
        enemyDragon->setPosition(cocos2d::Vec2(x, y));

        // Add the enemy dragon to the scene
        this->addChild(enemyDragon);

        // Add movement or other actions for the enemy dragon if needed
        auto moveAction = cocos2d::MoveTo::create(5.0f, cocos2d::Vec2(-enemyDragon->getContentSize().width, y));
        auto removeAction = cocos2d::RemoveSelf::create();
        enemyDragon->runAction(cocos2d::Sequence::create(moveAction, removeAction, nullptr)); }, interval, "emit_enemy_dragons_key");
}
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto spriteA = contact.getShapeA()->getBody()->getNode();
    auto spriteB = contact.getShapeB()->getBody()->getNode();

    if (spriteA && spriteB)
    {
        // Handle collision between player and enemy dragon
        if (spriteA->getTag() == PLAYER_TAG && spriteB->getTag() == ENEMY_TAG)
        {
            // Collision logic here
            CCLOG("Player collided with enemy dragon!");
        }
        else if (spriteA->getTag() == ENEMY_TAG && spriteB->getTag() == PLAYER_TAG)
        {
            // Collision logic here
            CCLOG("Player collided with enemy dragon!");
        }
    }

    return true;
}
