//
//  PhysicsObject.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PhysicsObject.h"
#include "../Physics/Box2D/b2Helper.h"
#include "../Services/ServiceLocator.h"
#include "../../Game/Game.h"


namespace GameDev2D
{
    PhysicsObject::PhysicsObject(b2BodyDef* aBodyDef) : GameObject("PhysicsObject"),
        m_PhysicsBody(nullptr),
        m_BodyType(aBodyDef->type),
        m_UpdateFlags(0)
    {
        //Create the physics body from the body def
        m_PhysicsBody = ServiceLocator::GetPhysicsWorld()->CreatePhysicsBody(aBodyDef);
    
        //Set the body's user data so that we can identify which PhysicsObject is associated with which b2Body
        m_PhysicsBody->SetUserData(this);
    }
    
    PhysicsObject::PhysicsObject(vec2 aPosition, float aAngle, b2BodyType aBodyType) : GameObject("PhysicsObject"),
        m_PhysicsBody(nullptr),
        m_BodyType(aBodyType),
        m_UpdateFlags(0)
    {
        //Setup the b2BodyDef
        b2BodyDef bodyDef;
        bodyDef.type = m_BodyType;
        bodyDef.position = b2Helper::PixelsToMeters(aPosition.x, aPosition.y);
        bodyDef.angle = aAngle * TO_RADIANS;
        
        //Create the physics body from the body def
        m_PhysicsBody = ServiceLocator::GetPhysicsWorld()->CreatePhysicsBody(&bodyDef);
    
        //Set the body's user data so that we can identify which PhysicsObject is associated with which b2Body
        m_PhysicsBody->SetUserData(this);
    }

    PhysicsObject::PhysicsObject(const string& aType) : GameObject(aType),
        m_PhysicsBody(nullptr),
        m_BodyType(b2_staticBody),
        m_UpdateFlags(0)
    {
    
    }
    
    PhysicsObject::~PhysicsObject()
    {
        //Destroy the physics body
        if (m_PhysicsBody != nullptr && ServiceLocator::GetPhysicsWorld() != nullptr)
        {
            ServiceLocator::GetPhysicsWorld()->DestroyPhysicsBody(m_PhysicsBody);
            m_PhysicsBody = nullptr;
        }
    }
    
    void PhysicsObject::Update(double aDelta)
    {
        //If this assert is hit, it means the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //If there is a physics update, make sure to awaken the physics body
        if(m_UpdateFlags != PhysicsUpdateNone)
        {
            m_PhysicsBody->SetAwake(true);
        }
        
        //Is the update transform bit set?
        if((m_UpdateFlags & PhysicsUpdateTransform) > 0)
        {
            //Turn the transform bit off
            m_UpdateFlags &= ~PhysicsUpdateTransform;
            
            //Convert the position from pixels into meters
            b2Vec2 position = b2Helper::PixelsToMeters(m_Position.x, m_Position.y);
            
            //Convert the angle from degrees into radians
            float angle = m_Angle * TO_RADIANS;
        
            //Set the physics body's transform
            m_PhysicsBody->SetTransform(position, angle);
        }
        
        //Is the update active bit set?
        if((m_UpdateFlags & PhysicsUpdateActive) > 0)
        {
            //Turn the active bit off
            m_UpdateFlags &= PhysicsUpdateActive;
            
            //Set the physics body active flag
            m_PhysicsBody->SetActive(m_IsEnabled);
        }
        
        //Is the update body type bit set?
        if((m_UpdateFlags & PhysicsUpdateBodyType) > 0)
        {
            //Turn the body type bit off
            m_UpdateFlags &= PhysicsUpdateBodyType;
            
            //Set the physics body type
            m_PhysicsBody->SetType(m_BodyType);
        }
        
        //Reset the model matrix, this ensure the game object and its children are drawn at the same location as the box2d body
        ResetModelMatrix();
    
        //Update the base object
        GameObject::Update(aDelta);
    }
    
    void PhysicsObject::Reset()
    {
        GameObject::Reset();
    }
    
    void PhysicsObject::SetTransform(float aX, float aY, float aAngle, double aDuration, EasingFunction aEasing, bool aReverse, int aRepeatCount)
    {
        SetLocalX(aX, aDuration, aEasing, aReverse, aRepeatCount);
        SetLocalY(aY, aDuration, aEasing, aReverse, aRepeatCount);
        SetLocalAngle(aAngle, aDuration, aEasing, aReverse, aRepeatCount);
    }
    
    void PhysicsObject::SetTransform(vec2 aPosition, float aAngle, double aDuration, EasingFunction aEasing, bool aReverse, int aRepeatCount)
    {
        SetTransform(aPosition.x, aPosition.y, aAngle, aDuration, aEasing, aReverse, aRepeatCount);
    }
    
    void PhysicsObject::SetLocalX(float aX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the x position
            m_Position.x = aX;
            
            //Set the transform bit in the update flags
            m_UpdateFlags |= PhysicsUpdateTransform;
        }
        else
        {
            if(GetLocalX() != aX)
            {
                m_TweenLocalX.Set(GetLocalX(), aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }

    void PhysicsObject::SetLocalY(float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the y position
            m_Position.y = aY;

            //Set the transform bit in the update flags
            m_UpdateFlags |= PhysicsUpdateTransform;
        }
        else
        {
            if(GetLocalY() != aY)
            {
                m_TweenLocalY.Set(GetLocalY(), aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    vec2 PhysicsObject::GetLocalPosition()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Convert the physics body's position from meters to pixels
        b2Vec2 position = b2Helper::MetersToPixels(m_PhysicsBody->GetPosition());
        return vec2(position.x, position.y);
    }
    
    void PhysicsObject::SetLocalAngle(float aAngle, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the angle
            m_Angle = aAngle;

            //Set the transform bit in the update flags
            m_UpdateFlags |= PhysicsUpdateTransform;
        }
        else
        {
            if(m_Angle != aAngle)
            {
                m_TweenAngle.Set(m_Angle, aAngle, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    float PhysicsObject::GetLocalAngle()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Convert the physics body's angle to degrees from radians
        float angle = m_PhysicsBody->GetAngle() * TO_DEGREES;
        return angle;
    }
    
    void PhysicsObject::SetIsEnabled(bool aIsEnabled)
    {
        //Set the enabled flag
        m_IsEnabled = aIsEnabled;
        
        //Set the active bit in the update flags
        m_UpdateFlags |= PhysicsUpdateActive;
    }
    
    bool PhysicsObject::IsEnabled()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return wether the physics body is active
        return m_PhysicsBody->IsActive();
    }
    
        b2Fixture* PhysicsObject::CreateFixture(const b2FixtureDef* aFixtureDef)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Create the fixture
        return m_PhysicsBody->CreateFixture(aFixtureDef);
    }
    
    void PhysicsObject::DestroyFixture(b2Fixture* aFixture)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Create the fixture
        return m_PhysicsBody->DestroyFixture(aFixture);
    }
    
    void PhysicsObject::SetLinearVelocity(vec2 aLinearVelocity)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Set the linear velocity
        m_PhysicsBody->SetLinearVelocity(b2Vec2(aLinearVelocity.x, aLinearVelocity.y));
    }
    
    vec2 PhysicsObject::GetLinearVelocity()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return physics body's linear velocity
        b2Vec2 linearVelocity = m_PhysicsBody->GetLinearVelocity();
        return vec2(linearVelocity.x, linearVelocity.y);
    }
    
    void PhysicsObject::SetAngularVelocity(float aAngularVelocity)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the angular velocity
        m_PhysicsBody->SetAngularVelocity(aAngularVelocity);
    }
    
    float PhysicsObject::GetAngularVelocity()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return the physics body's angular velocity
        return m_PhysicsBody->GetAngularVelocity();
    }
    
    void PhysicsObject::ApplyForce(vec2 aForce, vec2 aPoint)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the force and point of impact
        m_PhysicsBody->ApplyForce(b2Vec2(aForce.x, aForce.y), b2Vec2(aPoint.x, aPoint.y));
    }
    
    void PhysicsObject::ApplyForceToCenter(vec2 aForce)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the force and point of impact
        m_PhysicsBody->ApplyForceToCenter(b2Vec2(aForce.x, aForce.y));
    }
    
    void PhysicsObject::ApplyTorque(float aTorque)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the torque
        m_PhysicsBody->ApplyTorque(aTorque);
    }
    
    void PhysicsObject::ApplyLinearImpulse(vec2 aLinearImpulse, vec2 aPoint)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the linear impulse and point of impact
        m_PhysicsBody->ApplyLinearImpulse(b2Vec2(aLinearImpulse.x, aLinearImpulse.y), b2Vec2(aPoint.x, aPoint.y));
    }
    
    void PhysicsObject::ApplyLinearImpulseToCenter(vec2 aLinearImpulse)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the linear impulse and point of impact
        b2Vec2 center = m_PhysicsBody->GetWorldCenter();
        m_PhysicsBody->ApplyLinearImpulse(b2Vec2(aLinearImpulse.x, aLinearImpulse.y), center);

    }
    
    void PhysicsObject::ApplyAngularImpulse(float aAngularImpulse)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the angular impulse
        m_PhysicsBody->ApplyAngularImpulse(aAngularImpulse);
    }

    void PhysicsObject::SetLinearDamping(float aLinearDamping)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Set the linear damping
        m_PhysicsBody->SetLinearDamping(aLinearDamping);
    }
    
    float PhysicsObject::GetLinearDamping()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return the physics body's linear damping
        return m_PhysicsBody->GetLinearDamping();
    }

    void PhysicsObject::SetAngularDamping(float aAngularDamping)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Set the angular damping
        m_PhysicsBody->SetAngularDamping(aAngularDamping);
    }

    float PhysicsObject::GetAngularDamping()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return the physics body's angular damping
        return m_PhysicsBody->GetAngularDamping();
    }
    
    void PhysicsObject::SetGravityScale(float aGravityScale)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
    
        //Set the gravity scale
        m_PhysicsBody->SetGravityScale(aGravityScale);
    }
    
    float PhysicsObject::GetGravityScale()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return the physics body's gravity scale
        return m_PhysicsBody->GetGravityScale();
    }
    
    void PhysicsObject::SetIsBullet(bool aIsBullet)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Set wether the physics body's is a bullet
        m_PhysicsBody->SetBullet(aIsBullet);
    }
    
    bool PhysicsObject::IsBullet()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return wether the physics body's is a bullet
        return m_PhysicsBody->IsBullet();
    }
    
    void PhysicsObject::SetIsAwake(bool aIsAwake)
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Set wether the physics body's is awakw
        m_PhysicsBody->SetAwake(aIsAwake);
    }
    
    bool PhysicsObject::IsAwake()
    {
        //If this assert is hit, then the physics body is null
        assert(m_PhysicsBody != nullptr);
        
        //Return wether the physics body's is awake
        return m_PhysicsBody->IsAwake();
    }
    
    void PhysicsObject::SetBodyType(b2BodyType aBodyType)
    {
        //Set the body type
        m_BodyType = aBodyType;
        
        //Set the body type bit in the update flags
        m_UpdateFlags |= PhysicsUpdateBodyType;
    }
    
    b2BodyType PhysicsObject::GetBodyType()
    {
        //If the assert is hit, it means the physics body is null
        assert(m_PhysicsBody != nullptr);
        return m_PhysicsBody->GetType();
    }
    
    b2Body* PhysicsObject::GetBody()
    {
        return m_PhysicsBody;
    }
}