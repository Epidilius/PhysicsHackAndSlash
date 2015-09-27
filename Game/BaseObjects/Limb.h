//
//  Limb.h
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Limb__
#define __GameDev2D__Limb__

#include "../../Source/Physics/PhysicsObject.h"

#include "../Box2dObjectCreator.h"

namespace GameDev2D
{
    //Local Constants
    const unsigned short CATEGORY_WALLS = 1;
    const unsigned short CATEGORY_ARM = 2;
    const unsigned short CATEGORY_LEG = 4;
    const unsigned short CATEGORY_HEAD = 8;
    const unsigned short CATEGORY_TORSO = 16;
    const unsigned short CATEGORY_WEAPON = 32;
        
    //THINGS THAT OTHER THINGS WILL COLLIDE WITH
    const unsigned short MASK_WALLS = 65535;
    const unsigned short MASK_ARM = CATEGORY_WALLS | CATEGORY_HEAD | CATEGORY_TORSO | CATEGORY_WEAPON;
    const unsigned short MASK_LEG = CATEGORY_WALLS | CATEGORY_WEAPON;
    const unsigned short MASK_HEAD = CATEGORY_WALLS | CATEGORY_ARM | CATEGORY_TORSO | CATEGORY_WEAPON;
    const unsigned short MASK_TORSO = CATEGORY_WALLS | CATEGORY_HEAD | CATEGORY_ARM | CATEGORY_WEAPON;
    
    //Forward Declarations
    class Sprite;
    
    class Limb : public PhysicsObject
    {
    public:
        //The stats are done Health, Speed, Strength
        Limb(std::string type, vec2 position = vec2(0, 0), vec2 size = vec2(32, 32), vec3 stats = vec3(10, 10, 10), float angle = 0.0f, bool isPlayer = false);
        ~Limb();

        void MoveLimb(vec2 newPosition, float speed);
        
        PhysicsObject* GetLimb();
        float GetStat(int statType);
        
        void DealDamage(float damage);

        void Update(double delta);
        void Draw();
    private:
        //Stats
        short m_Strength;
        short m_Health;
        short m_Speed;
        //Limb
        PhysicsObject* m_Limb;
        //Object Creator
        Box2dObjectCreator* m_Box2dObjectCreator;
    };
}

#endif /* defined(__GameDev2D__Limb__) */
