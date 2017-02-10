//
//  CropScene.cpp
//  cocos2d_tests
//
//  Created by gongchangyou on 17/2/10.
//
//

#include "CropScene.h"

bool CropScene::init(){
    m_cropImage = CropImage::create("butterfly.jpg");
    m_cropImage->setAnchorPoint(Point::ZERO);
    m_cropImage->setPosition(Point::ZERO);
    this->addChild(m_cropImage, 0);
    
    //Add crop button.
    TTFConfig ttfConfig("arial.ttf", 20);
    auto label = Label::createWithTTF(ttfConfig, "Click me to crop");
    label->setColor(Color3B::RED);
    auto menuItem = MenuItemLabel::create(label, std::bind(&CropScene::crop, this));
    auto menu = Menu::create(menuItem, nullptr);
    
    menu->setPosition(Vec2(300,50));
    menuItem->setPosition(Point::ZERO);
    
    this->addChild(menu, 1);
    
    return true;
}

void CropScene::crop(){
    auto callback = [&](const std::string & filePath){
    };
    m_cropImage->cropImage(callback);
}
