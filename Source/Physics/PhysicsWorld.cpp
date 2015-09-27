//
//  PhysicsWorld.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PhysicsWorld.h"


namespace GameDev2D
{
    PhysicsWorld::PhysicsWorld(vec2 aGravity) :
        m_World(nullptr),
        m_DebugDraw(nullptr),
        m_VelocityIterations(4),
        m_PositionIterations(1)
    {
        //Create the world object with the gravity vector
        m_World = new b2World(b2Vec2(aGravity.x, aGravity.y));
        m_World->SetContactListener(this);
        
    #if DEBUG && BOX2D_DRAW_DEBUG_DATA
        //Create the debug draw for Box2d
        m_DebugDraw = new b2DebugDraw();

        //Set the debug draw flags
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        flags += b2Draw::e_jointBit;
        flags += b2Draw::e_centerOfMassBit;
        m_DebugDraw->SetFlags(flags);

        //Set the Box2d world debug draw instance
        m_World->SetDebugDraw(m_DebugDraw);
    #endif
    }
    
    PhysicsWorld::~PhysicsWorld()
    {
        SafeDelete(m_World);
        SafeDelete(m_DebugDraw);
    }

    void PhysicsWorld::Update(double aDelta)
    {
        if(m_World != nullptr)
        {
            m_World->Step((float)aDelta, m_VelocityIterations, m_PositionIterations);
        }
    }
    
    void PhysicsWorld::Draw()
    {
        #if DEBUG && BOX2D_DRAW_DEBUG_DATA
        if(m_World != NULL)
        {
            m_World->DrawDebugData();
        }
        #endif
    }
    
    b2Body* PhysicsWorld::CreatePhysicsBody(const b2BodyDef* aBodyDef, const b2FixtureDef* aFixtureDef)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
    
        //If this assert is hit, it means the bodydef pointer is null
        assert(aBodyDef != nullptr);

        //Create the b2Body
        b2Body* body = m_World->CreateBody(aBodyDef);
        
        //Is there a fixture def to attach to the body?
        if(aFixtureDef != nullptr)
        {
            body->CreateFixture(aFixtureDef);
        }
        
        //Return the body
        return body;
    }
    
    b2Body* PhysicsWorld::CreatePhysicsBody(const b2BodyDef* aBodyDef, const b2Shape* aShape, float aDensity)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
    
        //If this assert is hit, it means the bodydef pointer is null
        assert(aBodyDef != nullptr);
        
        //If this assert is hit, it means the shape pointer is null
        assert(aShape != nullptr);

        //Create the b2Body
        b2Body* body = m_World->CreateBody(aBodyDef);
        
        //Attach the shape to the body
        body->CreateFixture(aShape, aDensity);
        
        //Return the body
        return body;
    }
    
    b2Joint* PhysicsWorld::CreateJoint(const b2JointDef* aJointDef)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
    
        //If this assert is hit, it means the jointdef pointer is null
        assert(aJointDef != nullptr);

        //Create the 2Joint
        return m_World->CreateJoint(aJointDef);
    }
    
    void PhysicsWorld::DestroyPhysicsBody(b2Body* aBody)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
    
        //If this assert is hit, it means the body pointer is null
        assert(aBody != nullptr);

        //Destroy the body
        m_World->DestroyBody(aBody);
    }
    
    void PhysicsWorld::DestroyJoint(b2Joint* aJoint)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
    
        //If this assert is hit, it means the joint pointer is null
        assert(aJoint != nullptr);
        
        //Destroy the joint
        m_World->DestroyJoint(aJoint);
    }
    
    void PhysicsWorld::SetGravity(vec2 aGravity)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
        
        //Set the world's gravity vector
        m_World->SetGravity(b2Vec2(aGravity.x, aGravity.y));
    }
	
    vec2 PhysicsWorld::GetGravity()
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
        
        //Get the world's gravity vector
        b2Vec2 gravity= m_World->GetGravity();
        return vec2(gravity.x, gravity.y);
    }
    
    void PhysicsWorld::RayCast(b2RayCastCallback* aCallback, vec2 aPoint1, vec2 aPoint2)
    {
        //If this assert is hit, it means the Box2D world object is null
        assert(m_World != nullptr);
        
        //Convert the points
        b2Vec2 point1 = b2Vec2(aPoint1.x, aPoint1.y);
        b2Vec2 point2 = b2Vec2(aPoint2.x, aPoint2.y);
        
        //Perform the ray cast
        m_World->RayCast(aCallback, point1, point2);
    }
    
    void PhysicsWorld::SetVelocityIterations(int aVelocityIterations)
    {
        m_VelocityIterations = aVelocityIterations;
    }
    
    void PhysicsWorld::SetPositionIterations(int aPositionIterations)
    {
        m_PositionIterations = aPositionIterations;
    }

    void PhysicsWorld::BeginContact(b2Contact* aContact)
    {
        //Override this method to be notified when a Box2D contact begins
    }
    
    void PhysicsWorld::EndContact(b2Contact* aContact)
    {
        //Override this method to be notified when a Box2D contact ends
    }
}