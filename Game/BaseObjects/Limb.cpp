//
//  Limb.cpp
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#include "Limb.h"
#include "Box2D.h"
#include "../Character.h"
#include "../../Source/Physics/Box2D/b2Helper.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include <math.h>

namespace GameDev2D
{
    
    Limb::Limb(std::string aType, vec2 aPosition, vec2 aSize, vec3 aStats, float aAngle, bool aIsPlayer) : PhysicsObject(aType)
    {        
        m_Health = aStats.x;
        m_Speed = aStats.y;
        m_Strength = aStats.z;

        m_Box2dObjectCreator = Box2dObjectCreator::GetInstance();
        
        vec2 temp = aSize;
        float bodyWidth = b2Helper::PixelsToMeters(aSize.x);
        float bodyHeight = b2Helper::PixelsToMeters(aSize.y);
        
        //Create the catapult body shape
        b2PolygonShape bodyShape = m_Box2dObjectCreator->CreateBoxShape(vec2(bodyWidth, bodyHeight));
        
        //Create the limb body fixture def
        b2FixtureDef bodyFixtureDef = m_Box2dObjectCreator->CreateFixtureDef(2, &bodyShape, false, 0.4f, 1.0f, 0.2f);
        
        //Setup the filter for each fixture
        if(aType.find("Head") != string::npos)
        {
            bodyFixtureDef.filter.categoryBits = CATEGORY_HEAD;
            bodyFixtureDef.filter.maskBits = MASK_HEAD;
        }
        if(aType.find("Arm") != string::npos)
        {
            bodyFixtureDef.filter.categoryBits = CATEGORY_ARM;
            bodyFixtureDef.filter.maskBits = MASK_ARM;
        }
        if(aType.find("Leg") != string::npos)
        {
            bodyFixtureDef.filter.categoryBits = CATEGORY_LEG;
            bodyFixtureDef.filter.maskBits = MASK_LEG;
        }
        if(aType.find("Torso") != string::npos)
        {
            bodyFixtureDef.filter.categoryBits = CATEGORY_TORSO;
            bodyFixtureDef.filter.maskBits = MASK_TORSO;
        }
        
        //Create the body and attach the fixture to it
        m_Limb = m_Box2dObjectCreator->CreateBody(vec2(aPosition.x, aPosition.y), &bodyShape, 0.0f, b2_dynamicBody, &bodyFixtureDef);
    }
    
    Limb::~Limb()
    {
        SafeDelete(m_Limb);
    }

    
    
    void Limb::MoveLimb(vec2 newPosition, float aSpeed)
    {
        float width = ServiceLocator::GetPlatformLayer()->GetWidth();
        float height = ServiceLocator::GetPlatformLayer()->GetHeight();
        
        glm::vec2 forceDir = glm::vec2(( newPosition.x - width / 2),
                                       ( newPosition.y - height / 2));
        
        float length = std::sqrt((forceDir.x * forceDir.x) + (forceDir.y * forceDir.y));
        forceDir /= length;
        forceDir *= aSpeed;
        
        m_Limb->ApplyForceToCenter(forceDir);
    }
    
    
    PhysicsObject* Limb::GetLimb()
    {
        return m_Limb;
    }
    
    float Limb::GetStat(int aStatType)
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
    
    
    
    void Limb::DealDamage(float aDamage)
    {
        m_Health -= aDamage;
    }
    
    
    
    void Limb::Update(double aDelta)
    {
        m_Limb->Update(aDelta);
        //PhysicsObject::Update(aDelta);
    }
    void Limb::Draw()
    {
        m_Limb->Draw();
        //PhysicsObject::Draw();
    }
}