//
//  Game.h
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include "../Source/Core/Scene.h"
#include "../Source/Physics/PhysicsWorld.h"

#include "Box2dObjectCreator.h"


namespace GameDev2D
{
    //Local enum
    enum GameLoadSteps
    {
        GameStepInitialize = 0,
        GameStepLoadShapeCache,
        GameStepLoadGround,
        GameStepLoadLevel,
        GameStepLoadContent,
        GameStepLoadFinialize,
        GameStepLoadCount
    };

    //Local constants
    //Map Stuff
    const unsigned short NUMBER_OF_MAP_PIECES = 4;
    const string MAP_COLLISION_BASE_NAME = "MapCollision";
    const string MAP_IMAGE_BASE_NAME = "Map";
    
    const float BOX2D_PIXELS_TO_METERS_RATIO = 32.0f;
    const vec2 BOX2D_GRAVITY = vec2(0.0f, 0.0f);    //Top down game
    const unsigned int PLATFORM_COUNT = 5;

    //Forward declarations
    class TouchEvent;
    class CharacterManager;
    class Sprite;

    class Game : public Scene, PhysicsWorld
    {
    public:
        Game();
        virtual ~Game();
        
        //Load game content here
        float LoadContent();
        
        //Updates the game
        void Update(double delta);
        
        //Draw the game
        void Draw();

        //Handle events in this method
        void HandleEvent(Event* event);
        
    protected:
        //b2ContactListener methods
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        
    private:
        //Conveniance method to handle touch events
        void HandleTouchEvent(TouchEvent* touchEvent);
        
        //Screen size variables
        float m_ScreenHeight;
        float m_ScreenWidth;
        
        //Object creator
        Box2dObjectCreator* m_Box2dObjectCreator;
        
        unsigned int m_LoadStep;
        PhysicsObject** m_Platforms;
        vector<Sprite*> m_MapSprites;
        
        //Character Manager
        CharacterManager* m_CharacterManager;
    };
}

#endif
