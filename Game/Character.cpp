//
//  Character.cpp
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#include "Character.h"
#include "BaseObjects/Limb.h"
#include "BaseObjects/Weapon.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/UI/Sprite/Sprite.h"
#include "../Game/BaseObjects/Weapon.h"
#include "Box2dObjectCreator.h"
#include "../Libraries/box2d/Box2D.h"
#include "PhysicsWorld.h"
#include "Random.h"
#include "b2PrismaticJoint.h"
//TODO: A few secs after a player is destroyed, delete them and make a new one

namespace GameDev2D 
{
    Character::Character(int aNumberOfLimbs, glm::vec2 aRangeOfLimbSize, glm::vec2 aRangeOfStats, glm::vec2 aPosition, bool aIsPlayer)
    {
        m_SwingLeft = false;
        m_Timer = nullptr;
        m_DamageTimer = nullptr;
        
        m_Destroyed = false;
        m_DestroyPlayer = false;
        m_SlatedForDestruction = false;
        
        m_Health = DEFAULT_HEALTH;
        m_Strength = DEFAULT_STRENGTH;
        m_Speed = DEFAULT_SPEED;

        m_IsPlayer = aIsPlayer;
        
        m_NumberOfLimbs = aNumberOfLimbs;
        bool leftArm = true;
        bool leftLeg = true;
        string limbType;
        Random* random = new Random();
        
        for(float i = 0; i < m_NumberOfLimbs; i++)
        {
            //TODO: Fine tune the positioning based off of image size
            //Assume all characters face the same direction on spawn
            vec2 position = aPosition;
            random->RandomizeSeed();
            
            //TODO: Make a local variable, give it a 1% chance of becoming a second head. If it is, set the local variable to i, i to 1 (for head), make the head, then set i to the local variable

            if (i == 0)
            {
                //Torso, do nothing
                if(m_IsPlayer == true)
                    limbType = "CharTorso"; //TODO: Make these constants
                else
                    limbType = "DemonTorso";
            }
            else if (i == 1)
            {
                //Gives visibility to the body better. Decide if we want to do this
                position.y -= 2;
                if(m_IsPlayer == true)
                    limbType = "CharHead";
                else
                    limbType = "DemonHead";
            }
            else
            {
                if ((int)i % 2 == 0)
                {
                    if (leftArm == true)
                    {
                        position.x -= 16;
                        if(m_IsPlayer == true)
                            limbType = "CharArmLeft";
                        else
                            limbType = "DemonArmLeft";
                    }
                    else
                    {
                        position.x += 16;
                        if(m_IsPlayer == true)
                            limbType = "CharArmRight";
                        else
                            limbType = "DemonArmRight";
                    }

                    position.y += i;// / m_NumberOfLimbs;
                    leftArm = !leftArm;
                }
                else
                {
                    if (leftLeg == true)
                    {
                        position.x -= 8;   //TODO: Change this to be based on the image width of m_Limbs.at(0)
                        if(m_IsPlayer == true)
                            limbType = "CharLegLeft";
                        else
                            limbType = "DemonLegLeft";
                    }
                    else
                    {
                        position.x += 8;
                        if(m_IsPlayer == true)
                            limbType = "CharLegRight";
                        else
                            limbType = "DemonLegRight";
                    }

                    position.y -= 16;
                    leftLeg = !leftLeg;
                }
            }
            
            Sprite* sprite = new Sprite(limbType);
            
            glm::vec2 size = glm::vec2(sprite->GetWidth(), sprite->GetHeight());
            
            size.x *= ((float)random->RandomRange(aRangeOfLimbSize.x * 10, aRangeOfLimbSize.y * 10)) / 10.0f; //The multiple and divide is done because the function returns an int
            size.y *= ((float)random->RandomRange(aRangeOfLimbSize.x * 10, aRangeOfLimbSize.y * 10)) / 10.0f;
            
            if(size.x == 0)
                size.x = sprite->GetWidth();
            if(size.y == 0)
                size.y = sprite->GetHeight();
            
            vec3 stats = vec3(10, 10, 10);
            stats.x *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f; //The multiple and divide is done because the function returns an int
            stats.y *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f;
            stats.x *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f;
            
            Limb* limb = CreateALimb(limbType, position, size, stats, 0.0f);
            
            //Attach the shape to the physics object
            ServiceLocator::GetShapeCache()->AddFixtures(limb->GetLimb(), limbType);
            vec2 anchorPoint = ServiceLocator::GetShapeCache()->GetAnchorPoint(limbType);
            
            sprite->SetAnchorPoint(anchorPoint.x, anchorPoint.y);
            limb->GetLimb()->AddChild(sprite, true);

            m_Limbs.push_back(limb);
            
            if(i == 2)
            {
                Sprite* swordSprite = new Sprite("Sword");
                glm::vec2 swordSize = glm::vec2(swordSprite->GetWidth(), swordSprite->GetHeight());
                
                vec3 swordStats = vec3(10, 10, 10);
                swordStats.x *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f;
                swordStats.y *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f;
                swordStats.x *= ((float)random->RandomRange(aRangeOfStats.x * 10, aRangeOfStats.y * 10)) / 10.0f;
                
                // Weapon(std::string type, glm::vec2 position = vec2(0, 0), glm::vec2 size = vec2(32, 32), glm::vec3 stats = vec3(10, 10, 10), float angle = 0.0f);

                m_Weapon = new Weapon("Sword", m_Limbs.at(2)->GetLimb()->GetLocalPosition() - swordSize, swordSize, swordStats, 180.0f);
                m_Weapon->GetWeapon()->SetType("WEAPON");
                
                //Attach the shape to the physics object
                ServiceLocator::GetShapeCache()->AddFixtures(m_Weapon->GetWeapon(), "Sword");
                vec2 swordAnchorPoint = ServiceLocator::GetShapeCache()->GetAnchorPoint("Sword");
                
                swordSprite->SetAnchorPoint(swordAnchorPoint.x, swordAnchorPoint.y);
                m_Weapon->GetWeapon()->AddChild(swordSprite, true);
                
                m_Weapon->GetWeapon()->SetLocalPosition(m_Limbs.at(2)->GetLimb()->GetLocalPosition() + vec2(0, 20));   //TODO: Tweak this
            }
        }
        
        SafeDelete(random);
    }

    Character::~Character()
    {
        if (m_Timer != nullptr)
        {
            SafeDelete(m_Timer);
            m_Timer = nullptr;
        }
        if (m_DamageTimer != nullptr)
        {
            SafeDelete(m_DamageTimer);
            m_DamageTimer = nullptr;
        }
        
        SafeDelete(m_Weapon);
        for(int i = 0; i < m_Limbs.size(); i++) SafeDelete(m_Limbs.at(i));
    }
    
    void Character::CreateJoints(PhysicsWorld* aPhysicsWorld)
    {
        m_World = aPhysicsWorld;
        b2Body* torsoBody = m_Limbs.at(0)->GetLimb()->GetBody();
        
        
        //Head
        {
            b2Body* head = m_Limbs.at(1)->GetLimb()->GetBody();
            //TODO: Use a mouse joint for the enemy, and update it every second with new player information. Set it from the enemy torso to the player. Make and update it in character manager
            //Create the prismatic joint
            b2PrismaticJointDef prismaticJointDef;
            //Initialize(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor, const b2Vec2& axis);
            prismaticJointDef.Initialize(torsoBody, head, head->GetPosition(), b2Vec2(0, 1));
            prismaticJointDef.enableLimit = true;
            prismaticJointDef.lowerTranslation = -0.2f;
            prismaticJointDef.upperTranslation = 0.2f;
            prismaticJointDef.userData = head;
            m_Joints.push_back(aPhysicsWorld->CreateJoint(&prismaticJointDef));
        }
        //Sword
        {
            b2Body* arm = m_Limbs.at(2)->GetLimb()->GetBody();
            
            //Create the revolute joint
            b2RevoluteJointDef revoluteJointDef;
            revoluteJointDef.Initialize(arm, m_Weapon->GetWeapon()->GetBody(), arm->GetPosition());
            revoluteJointDef.userData = arm;
            m_Joints.push_back(aPhysicsWorld->CreateJoint(&revoluteJointDef));
        }
        
        for(int i = 2; i < m_Limbs.size(); i++)
        {
            b2Body* body = m_Limbs.at(i)->GetLimb()->GetBody();
            
            //Create the revolute joint
            b2WeldJointDef jointDef;
            jointDef.Initialize(torsoBody, body, body->GetPosition());
            jointDef.userData = body;
            m_Joints.push_back(aPhysicsWorld->CreateJoint(&jointDef));
        }
    }
    
    

    void Character::Update(double aDelta)
    {
        int temp = m_Health;
        
        if(m_DestroyPlayer == true && m_Destroyed == false)
        {
            if (m_Timer != nullptr)
            {
                m_Timer->Stop();
                SafeDelete(m_Timer);
                m_Timer = nullptr;
                
                StopSwing();
            }
            
            DestroyCharacter();
            m_DestroyPlayer = false;
            m_Destroyed = true;            
        }
        
        //Timer
        if (m_Timer != nullptr)
        {
            m_Timer->Update(aDelta);
            if (m_Timer->IsRunning() == false)
            {
                SafeDelete(m_Timer);
                m_Timer = nullptr;
                
                StopSwing();
            }
        }
        //Destruction Timer
        if (m_DamageTimer != nullptr)
        {
            m_DamageTimer->Update(aDelta);
            if (m_DamageTimer->IsRunning() == false)
            {
                SafeDelete(m_DamageTimer);
                m_DamageTimer = nullptr;
            }
        }

        for (int i = 0; i < m_Limbs.size(); i++)
        {
            m_Limbs.at(i)->Update(aDelta);
        }
        
        m_Weapon->Update(aDelta);
    }
    void Character::Draw()
    {
        for (int i = 2; i < m_Limbs.size(); i++)
        {
            m_Limbs.at(i)->Draw();
        }
        
        m_Weapon->Draw();
        
        m_Limbs.at(0)->Draw();  //This insures the torso is drawn, then the head
        m_Limbs.at(1)->Draw();
    }
    
    

    bool Character::GetIsPlayer()
    {
        return m_IsPlayer;
    }
    
    float Character::GetHealth()
    {
        return m_Health;
    }
    
    float Character::GetDamage()
    {
        float limbStrength = 0;
        
        for(int i = 0; i < m_Limbs.size(); i++)
        {
            limbStrength += m_Limbs.at(i)->GetStat(StatTypeStrength);
        }
        
        return (m_Strength + limbStrength);
    }
    
    Weapon* Character::GetWeapon()
    {
        return m_Weapon;
    }
    
    

    void Character::SwingInDirection(glm::vec2 aOldDirection, glm::vec2 aNewDirection)
    {
        vec2 directionToSwing = vec2((aOldDirection.x + aNewDirection.x)/2, (aOldDirection.y + aNewDirection.y)/2);
        
        if (m_Timer == nullptr && m_Joints.size() > 0)
        {
            float motorSpeed = 6;
            
            if(m_SwingLeft == true)
                motorSpeed *= -1;
            
            m_Timer = new Timer(SECONDS_TO_WAIT_FOR_SWORD_SWING);
            m_Timer->Start();
            
            //Turn on motor
            ((b2RevoluteJoint*)m_Joints.at(1))->EnableMotor(true);
            ((b2RevoluteJoint*)m_Joints.at(1))->SetMotorSpeed(motorSpeed * M_PI);
            ((b2RevoluteJoint*)m_Joints.at(1))->SetMaxMotorTorque(50.0f);
            
            m_SwingLeft = !m_SwingLeft;
        }
    }
    
    void Character::ThrowBomb(glm::vec2 aOldDirection, glm::vec2 aNewDirection)
    {
        vec2 directionToThrow = vec2((aOldDirection.x + aNewDirection.x)/2, (aOldDirection.y + aNewDirection.y)/2);
        
        //TODO: Throw a bomb
    }
    
    bool Character::DealDamage(Limb* limb, float damage)
    {
        if(m_DamageTimer == nullptr)
        {
            m_DamageTimer = new Timer(SECONDS_TO_WAIT_FOR_DAMAGE);
            m_DamageTimer->Start();
            
            m_Health -= damage / m_NumberOfLimbs;
            limb->DealDamage(damage/m_Health);  //TODO: Better formula
            
            if(limb->GetStat(StatTypeHealth) <= 0.0f)
                return true;
        }
        return false;
    }
    
    

    void Character::MoveCharacter(glm::vec2 aNewPosition)
    {
        float limbSpeed = 0;
        
        for (int i = 0; i < m_Limbs.size(); i++)
        {
            limbSpeed += m_Limbs.at(i)->GetStat(StatTypeSpeed);
        }
        
        m_Limbs.at(0)->MoveLimb(aNewPosition, m_Speed + limbSpeed);
    }

    glm::vec2 Character::GetCharacterPosition()
    {
        return m_Limbs.at(0)->GetLimb()->GetLocalPosition();
    }
    
    
    
    std::vector<Limb*> Character::GetLimbs()
    {
        return m_Limbs;
    }
    //And now, Mr Character, you DIE!
    void Character::DestroyCharacter()
    {
        Random* random = new Random();
        float randomNumber = 0;
        float oldRandom = 0;
        
        for(int i = 0; i < m_Joints.size(); i++)
        {
            random->RandomizeSeed();
            
            do{
                randomNumber = random->RandomRange(10, 100);
            }while(oldRandom == randomNumber);
            
            m_World->DestroyJoint(m_Joints.at(i));
            m_Limbs.at(i)->GetLimb()->ApplyTorque(randomNumber);
        }
        
        m_Joints.clear();
        
        SafeDelete(random);
    }
    
    void Character::SetDestroyCharacter(bool aDestroy)
    {
        m_DestroyPlayer = aDestroy;
    }
    
    void Character::SetDestructionSlating(bool aDestruction)
    {
        m_SlatedForDestruction = aDestruction;
    }
    
    bool Character::GetSlatedToDestroy()
    {
        return m_SlatedForDestruction;
    }
    
    
    //Private Functions
    void Character::StopSwing()
    {
        ((b2RevoluteJoint*)m_Joints.at(1))->EnableMotor(false);
    }
    
    Limb* Character::CreateALimb(std::string aType, glm::vec2 aPosition, glm::vec2 aSize, glm::vec3 aStats, float aAngle)
    {
        //TODO: Go through the number of limbs, and VERY SLIGHTLY randomize. 1% chance of two heads, for example
        //TODO: Get the angle based on how many of the current limb there is
        return new Limb(aType, aPosition, aSize, aStats, aAngle);
    }
    
    void Character::RemoveLimb(Limb* limb)
    {
        vector<b2Joint*> jointsToDestroy;
        
        for (b2JointEdge* jointEdge = limb->GetLimb()->GetBody()->GetJointList(); jointEdge; jointEdge=jointEdge->next)
        {
            jointsToDestroy.push_back((b2Joint*)jointEdge->joint);
            
            for( std::vector<b2Joint*>::iterator iter = m_Joints.begin(); iter != m_Joints.end(); ++iter )
            {
                if( *iter == (GameDev2D::b2Joint*)jointEdge->joint )
                {
                    m_Joints.erase( iter );
                    break;
                }
            }
        }
        
        for( std::vector<Limb*>::iterator iter = m_Limbs.begin(); iter != m_Limbs.end(); ++iter )
        {
            if( *iter == limb )
            {
                m_Limbs.erase( iter );
                break;
            }
        }
        
        
        for(int i = 0; i < jointsToDestroy.size(); i++)
        {
            m_World->DestroyJoint(jointsToDestroy.at(i));
        }
    }
}