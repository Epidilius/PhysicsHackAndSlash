//
//  Box2dObjectCreator.cpp
//  GameDev2D
//
//  Created by Jordan Vassilev on 2015-04-18.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#include "Box2dObjectCreator.h"
#include "../Source/Physics/PhysicsObject.h"


namespace GameDev2D
{
    Box2dObjectCreator::Box2dObjectCreator(b2World* aWorld)
    {
        if(aWorld != nullptr)
            m_World = aWorld;
    }
    
    Box2dObjectCreator::~Box2dObjectCreator()
    {
        
    }
    
    Box2dObjectCreator* Box2dObjectCreator::GetInstance(b2World* aWorld)
    {
        if(m_Box2dObjectCreator == nullptr)
        {
            Box2dObjectCreator(aWorld);
        }
        
        return m_Box2dObjectCreator;
    }
    
    b2EdgeShape Box2dObjectCreator::CreateEdgeShape(vec2 aVertexA, vec2 aVertexB)
    {
        b2EdgeShape edgeShape; // Create the shape
        b2Vec2 vertA = b2Vec2(aVertexA.x, aVertexA.y);
        b2Vec2 vertB = b2Helper::PixelsToMeters(aVertexB.x, aVertexB.y);
        edgeShape.Set(vertA, vertB);
        return edgeShape;
    } // ENDOF: b2EdgeShape Game::CreateEdgeShape(vec2 aVertexA, vec2 aVertexB)
    
    b2CircleShape Box2dObjectCreator::CreateCircleShape(float aRadius)
    {
        b2CircleShape circleShape;
        circleShape.m_p(aRadius);
        
        return circleShape;
    } // ENDOF: b2CircleShape Game::CreateCircleShape(float aRadius)
    
    b2PolygonShape Box2dObjectCreator::CreateBoxShape(vec2 aSize)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(b2Helper::PixelsToMeters(aSize.x), b2Helper::PixelsToMeters(aSize.y));
        return boxShape;
    } // ENDOF: b2PolygonShape Game::CreateBoxShape()
    
    b2PolygonShape Box2dObjectCreator::CreatePolygonShape(b2Vec2* aPoints, int aCount)
    {
        b2PolygonShape polygonShape;
        polygonShape.Set(aPoints, aCount);
        
        return polygonShape;
    } // ENDOF: b2PolygonShape Game::CreateBoxShape()
    
    //// FIXTURE DEF CREATION FUNCTIONS ////
    
    //Takes the type of
    b2FixtureDef Box2dObjectCreator::CreateFixtureDef(int aType, b2Shape* aShape, bool aIsSensor, float aFriction, float aDensity, float aRestitution)
    {
        switch (aType)
        {
            case ShapeTypesEdgeShape: // The ShapeType passed is an EdgeShape.
            {
                b2FixtureDef Fixture;
                Fixture.shape = (b2EdgeShape*)aShape;
                Fixture.isSensor = aIsSensor;
                Fixture.friction = aFriction;
                Fixture.density = aDensity;
                Fixture.restitution = aRestitution;
                
                return Fixture;
            }
                break;
                
            case ShapeTypesCircleShape: // Circle
            {
                b2FixtureDef Fixture;
                Fixture.shape = (b2CircleShape*)aShape;
                Fixture.isSensor = aIsSensor;
                Fixture.friction = aFriction;
                Fixture.density = aDensity;
                Fixture.restitution = aRestitution;
                
                return Fixture;
            }
                break;
                
            case ShapeTypesPolygonShape: // Polygon
            {
                b2FixtureDef Fixture;
                Fixture.shape = (b2PolygonShape*)aShape;
                Fixture.isSensor = aIsSensor;
                Fixture.friction = aFriction;
                Fixture.density = aDensity;
                Fixture.restitution = aRestitution;
                
                return Fixture;
            }
                break;
                
            case ShapeTypesChainShape: // Chain
            {
                b2FixtureDef Fixture;
                Fixture.shape = (b2PolygonShape*)aShape;
                Fixture.isSensor = aIsSensor;
                Fixture.friction = aFriction;
                Fixture.density = aDensity;
                Fixture.restitution = aRestitution;
                
                return Fixture;
            }
                break;
                
            default:
            {
                // Handle error...
                
                b2FixtureDef Fixture;
                Fixture.shape = (b2EdgeShape*)aShape;
                Fixture.isSensor = aIsSensor;
                Fixture.friction = aFriction;
                Fixture.density = aDensity;
                Fixture.restitution = aRestitution;
                
                return Fixture;
            }
                break;
        }
    } // ENDOF:
    
    PhysicsObject* Box2dObjectCreator::CreateBody(vec2 aPosition, b2Shape* aShape, float aAngle, b2BodyType aBodyType, b2FixtureDef* aFixtureDef)
    {
        //Create the box's body
        b2BodyDef bodyDef;
        bodyDef.type = aBodyType;
        bodyDef.position = b2Helper::PixelsToMeters(aPosition.x, aPosition.y);
        bodyDef.angle = aAngle * TO_RADIANS;
        
        //Create the physics object and add it to the scene
        PhysicsObject* physicsObject = new PhysicsObject(&bodyDef);
        //AddGameObject(physicsObject); ///////// Part of Scene /////////
        
        //Check if A fixture def was passed
        if(aFixtureDef == nullptr)
        {
            //Setup the box's fixture
            b2FixtureDef fixtureDef;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 1.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.shape = aShape;
            physicsObject->CreateFixture(&fixtureDef);
            return physicsObject;
        }
        else
        {
            //Attach the fixture to the body and return it
            physicsObject->CreateFixture(aFixtureDef);
            return physicsObject;
        }
    } // ENDOF: PhysicsObject* Box2dObjectCreator::CreateBody(vec2 aPosition, b2Shape* aShape, float aAngle, b2BodyType aBodyType, b2FixtureDef* aFixtureDef)
    
    b2World* Box2dObjectCreator::GetWorld()
    {
        return m_World;
    }
    
}