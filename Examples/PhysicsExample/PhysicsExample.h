//
//  PhysicsExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PhysicsExample__
#define __GameDev2D__PhysicsExample__

#include "../../Source/Core/Scene.h"
#include "../../Source/Physics/PhysicsWorld.h"


namespace GameDev2D
{
    enum PhysicsExampleLoadSteps
    {
        PhysicsExampleLoadInitialize = 0,
        PhysicsExampleLoadGroud,
        PhysicsExampleLoadPhysicsObject,
        PhysicsExampleLoadCount
    };

    //Local constants
    const vec2 PHYSICS_EXAMPLE_GRAVITY_VECTOR = vec2(0.0f, -9.8f);
    const float PHYSICS_EXAMPLE_PIXELS_TO_METERS_RATIO = 32.0f;

    //Forward declarations
    class PhysicsObject;

    class PhysicsExample : public Scene, public PhysicsWorld
    {
    public:
        PhysicsExample();
        ~PhysicsExample();
        
        //Load game content here
        float LoadContent();
        
        //Overridden to update and draw the box2d world
        void Update(double delta);
        void Draw();

        //Handle events in this method
        void HandleEvent(Event* event);
        
    private:
        //Member variables
        PhysicsObject* m_PhysicsObject;
        unsigned int m_LoadStep;
    };
}

#endif /* defined(__GameDev2D__PhysicsExample__) */
