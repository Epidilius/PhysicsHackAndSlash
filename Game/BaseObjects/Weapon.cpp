//
//  Weapon.cpp
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#include "Weapon.h"
#include "Box2D.h"
#include "../Character.h"
#include "../../Source/Physics/Box2D/b2Helper.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../Box2dObjectCreator.h"
#include <math.h>

namespace GameDev2D
{
    
    Weapon::Weapon(std::string aType, vec2 aPosition, vec2 aSize, vec3 aStats, float aAngle) : PhysicsObject(aType)
    {        
        m_Health = aStats.x;
        m_Speed = aStats.y;
        m_Strength = aStats.z;
        
        m_Box2dObjectCreator = Box2dObjectCreator::GetInstance();
        
        vec2 temp = aSize;
        float bodyWidth = b2Helper::PixelsToMeters(aSize.x);
        float bodyHeight = b2Helper::PixelsToMeters(aSize.y);
        
        //Create the body shape
        b2PolygonShape bodyShape = m_Box2dObjectCreator->CreateBoxShape(vec2(bodyWidth, bodyHeight));
        
        //Create the weapon body fixture def
        b2FixtureDef bodyFixtureDef = m_Box2dObjectCreator->CreateFixtureDef(2, &bodyShape, false, 0.4f, 1.0f, 0.2f);
        
        //Setup the filter for the fixture
        //Do an if - if else thing for each type of limb (torso, head, arm, leg)
        bodyFixtureDef.filter.categoryBits = 32;    //Weapon category. Used a number because I couldn't redeclare it
        
        //Create the body and attach the fixture to it
        m_Weapon = m_Box2dObjectCreator->CreateBody(vec2(aPosition.x, aPosition.y), &bodyShape, 0.0f, b2_dynamicBody, &bodyFixtureDef);
    }
    
    Weapon::~Weapon()
    {        
        SafeDelete(m_Weapon);
    }
    
    
    
    PhysicsObject* Weapon::GetWeapon()
    {
        return m_Weapon;
    }
        
    float Weapon::GetStat(int aStatType)
    {
        switch(aStatType)
        {
            case StatTypeHealth:
                return m_Health;
                break;
                
            case StatTypeSpeed:
                return m_Speed;
                break;
                
            case StatTypeStrength:
                return m_Strength;
                break;
                
            default:
                return -1;
                break;
        }
    }
    
    
    
    void Weapon::DealDamage(float aDamage)
    {
        m_Health -= aDamage;
    }
    
    
    
    void Weapon::Update(double aDelta)
    {        
        m_Weapon->Update(aDelta);
        //PhysicsObject::Update(aDelta);
    }
    void Weapon::Draw()
    {
        m_Weapon->Draw();
        //PhysicsObject::Draw();
    }
}