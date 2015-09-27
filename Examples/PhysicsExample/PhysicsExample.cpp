//
//  PhysicsExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PhysicsExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Audio/Audio.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Graphics/Graphics.h"
#include "../../Source/Events/Input/InputEvents.h"
#include "Box2D.h"


namespace GameDev2D
{
    PhysicsExample::PhysicsExample() : Scene("PhysicsExample"), PhysicsWorld(PHYSICS_EXAMPLE_GRAVITY_VECTOR),
        m_PhysicsObject(nullptr),
        m_LoadStep(0)
    {

    }
    
    PhysicsExample::~PhysicsExample()
    {
        ServiceLocator::SetPhysicsWolrd(nullptr);
    }
    
    float PhysicsExample::LoadContent()
    {
        switch (m_LoadStep)
        {
            case PhysicsExampleLoadInitialize:
            {
                //Set the box2d helper ratio and scale
                b2Helper::SetPixelsToMetersRatio(PHYSICS_EXAMPLE_PIXELS_TO_METERS_RATIO);
                b2Helper::SetScale(ServiceLocator::GetPlatformLayer()->GetScale());
                
                //Set the game pointer for the service locator
                ServiceLocator::SetPhysicsWolrd(this);
                
                //Create the shape cache
                ServiceLocator::GetShapeCache()->LoadShapesFromFile("shapedefs");
            }
            break;
            
            case PhysicsExampleLoadGroud:
            {
                //Get the screen width
                float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
            
                //Define the ground body.
                b2BodyDef groundBodyDef;
                groundBodyDef.position.Set(0.0f, 0.0f); // bottom-left corner

                //Define the ground box shape.
                b2EdgeShape groundShape;
                b2Vec2 vertex1 = b2Vec2(0.0f, 0.0f);
                b2Vec2 vertex2 = b2Vec2(b2Helper::PixelsToMeters(screenWidth), 0.0f);
                groundShape.Set(vertex1, vertex2);

                //Call the body factory which allocates memory for the ground body
                //from a pool and creates the ground box shape (also from a pool).
                //The body is also added to the world.
                CreatePhysicsBody(&groundBodyDef, &groundShape, 0.0f);
            }
            break;
            
            case PhysicsExampleLoadPhysicsObject:
            {
                //Get the screen width and height
                float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
                float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
            
                //Create the physics object
                m_PhysicsObject = new PhysicsObject(vec2(screenWidth * 0.5f, screenHeight * 0.75f), 0.0f, b2_dynamicBody);
                ServiceLocator::GetShapeCache()->AddFixtures(m_PhysicsObject, "AC-Logo");
                AddGameObject(m_PhysicsObject);
                
                //Create a sprite and add it to the physics object as a child
                Sprite* sprite = new Sprite("AC-Logo");
                sprite->SetAnchorPoint(ServiceLocator::GetShapeCache()->GetAnchorPoint("AC-Logo"));
                m_PhysicsObject->AddChild(sprite, true);
            }
            break;

            default:
            break;
        }
    
        //Increment the load step
        m_LoadStep++;

        //Calculate the percentage loaded
        return (float)m_LoadStep / (float)PhysicsExampleLoadCount;
    }
    
    void PhysicsExample::Update(double aDelta)
    {
        PhysicsWorld::Update(aDelta);
        Scene::Update(aDelta);
    }
    
    void PhysicsExample::Draw()
    {
        PhysicsWorld::Draw();
        Scene::Draw();
    }
    
    void PhysicsExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case MOUSE_CLICK_EVENT:
            {
                MouseClickEvent* mouseClickEvent = (MouseClickEvent*)aEvent;
                if(mouseClickEvent->GetMouseClickEventType() == MouseClickDown)
                {
                    m_PhysicsObject->SetLocalPosition(mouseClickEvent->GetPosition());
                    m_PhysicsObject->SetLinearVelocity(vec2(0.0f, 0.0f));
                }
                
            }
            break;
            
            case TOUCH_EVENT:
            {
                TouchEvent* touchEvent = (TouchEvent*)aEvent;
                if(touchEvent->GetTouchType() == TouchBegan)
                {
                    m_PhysicsObject->SetLocalPosition(touchEvent->GetTouchLocation());
                    m_PhysicsObject->SetLinearVelocity(vec2(0.0f, 0.0f));
                }
            }
            break;
        
            default:
                break;
        }
    }
}