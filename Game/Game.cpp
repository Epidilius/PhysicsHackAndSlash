//
//  Game.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

/*
 TODO
 Stats:
    Death Counter: Keeps track of the number of times the player has died this session
    Kills Since Last Death Counter: Keeps track of the number of the kills the player has made since their last death
    Total Kills Counter: Keeps track of all kills this session

    Health Bar: Keeps track of health. Have this be a bright green, and in bar-chunks
    Time Since Death: Have a timer (with 1 decimal, so xy.z) keeping track of time since death
    Respawn Timer: When you die, have the numbers 3, 2, 1 appear taking up most of the screen, then shrink and fade
        Only appears when you die

    Total Kills Over All Sessions?: Keeps track of every single kill ever
        Not displayed on Game Screen
    Total Deaths Over All Sessions?: Keeps track of every single death ever 
        Not displayed on Game Screen
 
 Game Center:
    Leaderboards for the above stats
    Achievements (kill x enemies in a single life, kill x enemies, survive for x time, etc)
 
 Menus:
    Main Menu
        A "Play" button which starts a new session
        An "Instructions" button to open the instructions screen (simple screen with text and basic images)        
        A "Stats" button to open the stats menu
    Stats Menu:
        All the stats listed above and their values
        A "Reset" button that asks if you're sure, then resets your stats
    Pause Menu
        A "Resume" button that resumes the game?
            Maybe just tapping on not a button does this
        An "Instructions" button
        A "Main Menu" button which returns you to the main menu
 
 Game Screen:
    Pause Button?: This opens the Pause Menu
        This could also be done by a two or three finger tap
 
 Ads:
    Do these at the very end
    Ad banner in the main menu
    Ad banner in the instructions menu
    Ad banner in the pause menu
    NO ads in the Game Screen
 
 */

#include "Game.h"
#include "Managers/CharacterManager.h"
#include "Character.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Audio/Audio.h"
#include "../Source/UI/UI.h"
#include "../Source/Graphics/Graphics.h"
#include "../Source/Events/Input/InputEvents.h"


namespace GameDev2D
{
    Game::Game() : Scene("Game"), PhysicsWorld(BOX2D_GRAVITY)
    {
        m_LoadStep = 0;
        m_Box2dObjectCreator = Box2dObjectCreator::GetInstance();
    }
    
    Game::~Game()
    {
        for(unsigned int i = 0; i < PLATFORM_COUNT; i++)
        {
            SafeDelete(m_Platforms[i]);
        }
        SafeDeleteArray(m_Platforms);
        
        SafeDelete(m_CharacterManager);
        for(unsigned int j = 0; j < m_MapSprites.size(); j++) SafeDelete(m_MapSprites.at(j));
    }
    
    float Game::LoadContent()
    {
        switch (m_LoadStep)
        {
            case GameStepInitialize:
            {
                //Set the b2Helper scale and p2m ratio
                b2Helper::SetPixelsToMetersRatio(BOX2D_PIXELS_TO_METERS_RATIO);
                float scale = ServiceLocator::GetPlatformLayer()->GetScale();
                b2Helper::SetScale(scale);
                
                m_ScreenWidth = ServiceLocator::GetPlatformLayer()->GetWidth();
                m_ScreenHeight = ServiceLocator::GetPlatformLayer()->GetHeight();
                
                //Set the physics world
                ServiceLocator::SetPhysicsWolrd(this);
            }
            break;
            
            case GameStepLoadShapeCache:
            {
                //Load the shapes
                ServiceLocator::GetShapeCache()->LoadShapesFromFile("shapedefs");
            }
            break;
                
            case GameStepLoadGround:
            {
                //Load the map//Prepare the map
                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position = b2Helper::PixelsToMeters(m_ScreenWidth / 2, m_ScreenHeight / 2);
                
                for(int i = 0; i < NUMBER_OF_MAP_PIECES; i++)
                {
                    PhysicsObject* map = new PhysicsObject(&bodyDef);
                    
                    stringstream ss;
                    ss << i;
                    string index = ss.str();
                    
                    ServiceLocator::GetShapeCache()->AddFixtures(map, MAP_COLLISION_BASE_NAME + index);
                    vec2 anchorPoint = ServiceLocator::GetShapeCache()->GetAnchorPoint(MAP_COLLISION_BASE_NAME + index);
                    
                    Sprite* mapSprite = new Sprite(MAP_IMAGE_BASE_NAME + index);
                    mapSprite->SetAnchorPoint(anchorPoint);
                    
                    map->AddChild(mapSprite, true);   //TODO: Fix the map. Add extra background on the top and bottom
                    AddGameObject(map);
                    
                    //m_MapSprites.push_back(mapSprite);
                }
            }
            break;
            
            case GameStepLoadLevel:
            {
                //Create the array that stores the PhysicsObject pointers
                m_Platforms = new PhysicsObject*[PLATFORM_COUNT];
                
                //Calculate the x increment and the starting x percentage on where to place the platforms
                float incrementX = 1.0f - ((float)PLATFORM_COUNT / ((float)PLATFORM_COUNT + 1.0f));
                float percentageX = incrementX;
                
                b2PolygonShape platformShapes[PLATFORM_COUNT];
                
                //Cycle through and create the PhysicsObject objects
                for(unsigned int i = 0; i < PLATFORM_COUNT; i++)
                {
                    //Calculate the y percentage and the dersired body type
                    float percentageY = 0.5f - (0.1f * (float)(i % (PLATFORM_COUNT - 1)));
                    b2BodyType bodyType = (i % (PLATFORM_COUNT - 1)) == 0 ? b2_staticBody : b2_kinematicBody;
                
                    //Create a box for each platform at the calculate position and desired body type
                    platformShapes[0] = m_Box2dObjectCreator->CreateBoxShape(vec2(32.0f, 32.0f));
                    m_Platforms[i] = m_Box2dObjectCreator->CreateBody(vec2(m_ScreenWidth * percentageX, m_ScreenHeight * percentageY), &platformShapes[0], 0.0f, bodyType);
                    
                    //If the body is kinematic, set the linear velocity
                    if(bodyType == b2_kinematicBody)
                    {
                        m_Platforms[i]->SetLinearVelocity(vec2(0.0f, 5.0f));
                    }
                    
                    //Increment the x percentage
                    percentageX += incrementX;
                }
            }
            break;
            
            case GameStepLoadContent:
            {
                m_CharacterManager = new CharacterManager(m_ScreenWidth, m_ScreenHeight, this);
            }
            break;
            
            case GameStepLoadFinialize:
            {
                Reset();
            }
            break;

            default:
                break;
        }
        
        //Increment the load step
        m_LoadStep++;
        
        //Are we done loading?
        return (float)m_LoadStep / (float)GameStepLoadCount;
    }
    
    void Game::Update(double aDelta)
    {
        //Update the Physics world
        PhysicsWorld::Update(aDelta);
    
        m_CharacterManager->Update(aDelta);

        //Calculate the min and max y value
        float minY = m_ScreenHeight * 0.25f;
        float maxY = m_ScreenHeight * 0.75f;
        
        //Setting the Camera to follow the physics object
        vec2 camPos = m_CharacterManager->GetCharacterAtIndex(0)->GetCharacterPosition();
        this->GetCamera()->SetPosition(-camPos.x + m_ScreenWidth / 2, -camPos.y + m_ScreenHeight / 2);
        
        
        //Cycle through the platforms and check the position, see if the linear velocity needs to be reversed
        for(unsigned int i = 0; i < PLATFORM_COUNT; i++)
        {
            //Get the y position of the platform
            float y = m_Platforms[i]->GetWorldY();
            
            //Get the linear velocity for the y-axis of the platform
            float vy = m_Platforms[i]->GetLinearVelocity().y;
        
            //Check the platform's position and reverse direction if needed
            if((y >= maxY && vy > 0.0f) || (y <= minY && vy < 0.0f))
            {
                m_Platforms[i]->SetLinearVelocity(vec2(0.0f, vy * -1.0f));
            }
        }
        
        Scene::Update(aDelta);
    }
    
    void Game::Draw()
    {
        //Draw the Physics world (for debug drawing)
        //m_MapSprite->Draw();
        
        Scene::Draw();
        PhysicsWorld::Draw();
        m_CharacterManager->Draw();
    
    }
    
    void Game::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case TOUCH_EVENT:
                HandleTouchEvent((TouchEvent*)aEvent);
                break;
        
            default:
                break;
        }
    }
    
    void Game::BeginContact(b2Contact* aContact)
    {
        m_CharacterManager->OnContactStart(aContact);
    }
    void Game::EndContact(b2Contact* aContact)
    {
        m_CharacterManager->OnContactEnd(aContact);
    }
    
    void Game::HandleTouchEvent(TouchEvent* aTouchEvent)
    {
        m_CharacterManager->HandleTouchEvent(aTouchEvent);
    }
}



