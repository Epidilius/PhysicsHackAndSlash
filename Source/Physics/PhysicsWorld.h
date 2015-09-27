//
//  PhysicsWorld.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PhysicsWorld__
#define __GameDev2D__PhysicsWorld__

#include "PhysicsObject.h"
#include "Box2D/b2Helper.h"
#include "../Platforms/PlatformMacros.h"
#include "../Physics/Box2D/b2DebugDraw.h"
#include "Box2D.h"

using namespace glm;

namespace GameDev2D
{
    class PhysicsWorld : public b2ContactListener
    {
    public:
        //The constructor takes in the gravity vector to initialize the box2d world with
        PhysicsWorld(vec2 gravity);
        virtual ~PhysicsWorld();
        
        //Updates and draws the box2d world object
        void Update(double delta);
        void Draw();
        
        //Creates a physics body
        virtual b2Body* CreatePhysicsBody(const b2BodyDef* bodyDef, const b2FixtureDef* fixtureDef = nullptr);
        virtual b2Body* CreatePhysicsBody(const b2BodyDef* bodyDef, const b2Shape* shape, float density);
        
        //Creates a physics joint
        virtual b2Joint* CreateJoint(const b2JointDef* jointDef);
        
        //Destroys a physics body from the box2d world
        virtual void DestroyPhysicsBody(b2Body* body);
        
        //Destroys a physics joint from the box2d world
        virtual void DestroyJoint(b2Joint* joint);
        
        //Change the global gravity vector.
        virtual void SetGravity(vec2 gravity);
	
        //Get the global gravity vector.
        virtual vec2 GetGravity();
        
        //Ray-cast the world for all fixtures in the path of the ray. Your callback
        //controls whether you get the closest point, any point, or n-points.
        //The ray-cast ignores shapes that contain the starting point.
        virtual void RayCast(b2RayCastCallback* callback, vec2 point1, vec2 point2);
        
        //Sets the velocity iterations
        void SetVelocityIterations(int velocityIterations);
        
        //Sets the position iterations
        void SetPositionIterations(int positionIterations);
        
    protected:
        //b2ContactListener methods
        virtual void BeginContact(b2Contact* contact);
        virtual void EndContact(b2Contact* contact);
        
    private:
        //Member variables
        b2World* m_World;
        b2DebugDraw* m_DebugDraw;
        int m_VelocityIterations;
        int m_PositionIterations;
    };
}

#endif /* defined(__GameDev2D__PhysicsWorld__) */
