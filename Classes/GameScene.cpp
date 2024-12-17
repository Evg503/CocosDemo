#include "common.h"
#include "GameScene.h"

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

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto bg = cocos2d::Sprite::create("res/background.png");
    auto bg_size = bg->getContentSize();
    bg->setPosition( cocos2d::Vec2(visibleSize.width/2, visibleSize.height/2) );
    auto scale = std::max(visibleSize.width/bg_size.width, visibleSize.height/bg_size.height);
    bg->setScale(scale);
    addChild(bg, -5);
    addBackButton();
    return true;
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
