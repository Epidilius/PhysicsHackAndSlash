//
//  Box2dObjectCreator.h
//  GameDev2D
//
//  Created by Jordan Vassilev on 2015-04-18.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Box2dObjectCreator__
#define __GameDev2D__Box2dObjectCreator__

#include <stdio.h>
#include "Box2D.h"
#include "../Source/Physics/Box2D/b2Helper.h"

using glm::vec2;

namespace GameDev2D
{
    //Local Enum
    enum ShapeTypes
    {
        ShapeTypesEdgeShape = 0,
        ShapeTypesCircleShape,
        ShapeTypesPolygonShape,
        ShapeTypesChainShape
        
    };
    
    //Local Constants
    
    
    //Forward Declarations
    class PhysicsObject;
    
    class Box2dObjectCreator
    {
    private:
        Box2dObjectCreator(b2World* world = nullptr);
        
    public:
        ~Box2dObjectCreator();
        
        static Box2dObjectCreator* GetInstance(b2World* world = nullptr);
        
        b2EdgeShape CreateEdgeShape(vec2 vertexA, vec2 vertexB);
        
        b2CircleShape CreateCircleShape(float radius);
        
        b2PolygonShape CreateBoxShape(vec2 size);
        
        b2PolygonShape CreatePolygonShape(b2Vec2* points, int count);
        
        b2FixtureDef CreateFixtureDef(int type, b2Shape* aShape, bool isSensor = false, float friction = 0.2f, float density = 1.0f, float restitution = 0.5f);
        
        PhysicsObject* CreateBody(vec2 position, b2Shape* shape, float angle = 0.0f, b2BodyType bodyType = b2_dynamicBody, b2FixtureDef* fixtureDef = nullptr);
        
        b2World* GetWorld();
        
    protected:
        
        
    private:
        //Member Variables
        b2World* m_World;
    };
    static Box2dObjectCreator* m_Box2dObjectCreator = nullptr;
    
}

#endif /* defined(__GameDev2D__Box2dObjectCreator__) */
