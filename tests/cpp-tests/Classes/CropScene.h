//
//  CropScene.hpp
//  cocos2d_tests
//
//  Created by gongchangyou on 17/2/10.
//
//

#ifndef CropScene_h
#define CropScene_h
#include "CropImage.h"
using namespace cocos2d;

#import "cocos2d.h"
class CropScene : public Scene{
public:
    CREATE_FUNC(CropScene);
private:
    bool init();
    void crop();
    CropImage * m_cropImage;
    Menu * m_menu;
};
#endif /* CropScene_hpp */
