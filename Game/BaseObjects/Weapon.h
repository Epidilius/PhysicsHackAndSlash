//
//  Weapon.h
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Weapon__
#define __GameDev2D__Weapon__

#include <stdio.h>
#include "PhysicsObject.h"

namespace GameDev2D
{
    //Local Constants
    //const unsigned short CATEGORY_WEAPON = 32;
    
    //Forward Declarations
    class Sprite;
    class Box2dObjectCreator;
    
    class Weapon: public PhysicsObject
    {
    public:
        //The stats are done Health, Speed, Strength
        Weapon(std::string type, glm::vec2 position = vec2(0, 0), glm::vec2 size = vec2(32, 32), glm::vec3 stats = vec3(10, 10, 10), float angle = 0.0f);
        ~Weapon();
        
        PhysicsObject* GetWeapon();
        float GetStat(int statType);
        
        void DealDamage(float damage);
        
        void Update(double delta);
        void Draw();

      
    private:
        //Stats
        short m_Strength;
        short m_Health;
        short m_Speed;
        //Weapon
        PhysicsObject* m_Weapon;
        //Object Creator
        Box2dObjectCreator* m_Box2dObjectCreator;
    };
}

#endif /* defined(__GameDev2D__Weapon__) */

