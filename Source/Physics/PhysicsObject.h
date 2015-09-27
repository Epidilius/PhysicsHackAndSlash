//
//  PhysicsObject.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PhysicsObject__
#define __GameDev2D__PhysicsObject__

#include "../Core/GameObject.h"
#include "Box2D.h"


namespace GameDev2D
{
    //Enum to manage updating the b2Body object's properties
    enum PhysicsUpdateFlags
    {
        PhysicsUpdateNone = 0,                  //(0000 0000 0000 0000)
        PhysicsUpdateTransform = 1,             //(0000 0000 0000 0001)
        PhysicsUpdateActive = 2,                //(0000 0000 0000 0010)
        PhysicsUpdateBodyType = 4,              //(0000 0000 0000 0100)

    };
    
    //The PhyiscsObject inherits from GameObject and wraps a functionality around a b2Body object,
    //it automatically converts pixels to meters and degrees to radians and vice versa
    class PhysicsObject : public GameObject
    {
    public:
        //Creates a PhysicsObject using a bodyDef which will be used to create to b2Body
        PhysicsObject(b2BodyDef* bodyDef);
        
        //Creates a PhysicsObject using a position (in pixels), an angle (in degrees) and a body type (default is dynamic)
        PhysicsObject(vec2 position, float angle, b2BodyType bodyType = b2_dynamicBody);
        
        //Destructor
        virtual ~PhysicsObject();
        
        //Updates and resets the physics body
        virtual void Update(double delta);
        virtual void Reset();
        
        //Sets the transform of the physics body
        virtual void SetTransform(float x, float y, float angle, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetTransform(vec2 position, float angle, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the local position of the physics body
        virtual void SetLocalX(float x, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalY(float y, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);

        //Returns the local position of the physics body
        virtual vec2 GetLocalPosition();

        //Sets the local angle of the physics body
        virtual void SetLocalAngle(float angle, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);

        //Returns the local angle of the physics body
        virtual float GetLocalAngle();

        //Sets wether the physics body is enabled
        virtual void SetIsEnabled(bool isEnabled);
        
        //Returns wether the physics body is enabled
        virtual bool IsEnabled();

        //Creates a fixture and attaches it to the physics body
        b2Fixture* CreateFixture(const b2FixtureDef* fixtureDef);
        
        //Destory a fixture attached to the physics body
        void DestroyFixture(b2Fixture* fixture);

        //Sets the linear velocity for the PhysicsObject
        void SetLinearVelocity(vec2 linearVelocity);
        
        //Returns the Physics body's linear velocity
        vec2 GetLinearVelocity();

        //Sets the angular velocity for the PhysicsObject
        void SetAngularVelocity(float angularVelocity);
        
        //Returns the Physics body's angular velocity
        float GetAngularVelocity();

        //Apply force to a point on the physics body
        void ApplyForce(vec2 force, vec2 point);
        
        //Apply force to the center of the physics body
        void ApplyForceToCenter(vec2 force);

        //Apply torque to the physics body
        void ApplyTorque(float torque);
        
        //Apply linear impulse to the physics body
        void ApplyLinearImpulse(vec2 linearImpulse, vec2 point);
        
        //Apply linear impulse to the center of the physics body
        void ApplyLinearImpulseToCenter(vec2 linearImpulse);
        
        //Apply angular impulse to the physics body
        void ApplyAngularImpulse(float angularImpulse);

        //Sets the Physics body's linear damping
        void SetLinearDamping(float linearDamping);

        //Returns the Physics body's linear damping
        float GetLinearDamping();

        //Sets the Physics body's angular damping
        void SetAngularDamping(float angularDamping);
        
        //Returns the Physics body's angular damping
        float GetAngularDamping();

        //Sets the Physics body's gravity scale
        void SetGravityScale(float gravityScale);
        
        //Returns the Physics body's gravity scale
        float GetGravityScale();
        
        //Returns wether the Physics body is a bullet
        void SetIsBullet(bool isBullet);
        
        //Returns wether the Physics body is a bullet
        bool IsBullet();
        
        //Set wether the Physics body is awake
        void SetIsAwake(bool isAwake);
        
        //Returns wether the Physics body is awake
        bool IsAwake();
        
        //Set the Physics body's type
        void SetBodyType(b2BodyType bodyType);
        
        //Returns the Physics body's type
        b2BodyType GetBodyType();
        
        b2Body* GetBody();
        
    protected:
        //Used when the Physics Object is subclassed
        PhysicsObject(const string& type);
    
        //Member variables
        b2Body* m_PhysicsBody;
        b2BodyType m_BodyType;
        unsigned char m_UpdateFlags;
    };
}

#endif /* defined(__GameDev2D__PhysicsObject__) */
