//
//  Character.h
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Character__
#define __GameDev2D__Character__

namespace GameDev2D
{
    enum StatTypes
    {
        StatTypeHealth = 0,
        StatTypeStrength,
        StatTypeSpeed
    };
    
    //Forward Declarations
    class Limb;
    class Weapon;
    class PhysicsWorld;
    class Weapon;
    class Timer;
    
    struct b2Joint;
    
    //Local Constants
    //Body Types
    const std::string RIGHT_ARM_TYPE = "RIGHT_ARM";
    const std::string LEFT_ARM_TYPE = "LEFT_ARM";
    const std::string LEG_TYPE = "LEG";
    const std::string HEAD_TYPE = "HEAD";
    const std::string TORSO_TYPE = "TORSO";
    //Timer
    const float SECONDS_TO_WAIT_FOR_SWORD_SWING = 0.5f;
    const float SECONDS_TO_WAIT_FOR_DAMAGE = 0.5f;
    //Character Stats
    const float DEFAULT_HEALTH = 100.0f;
    const float DEFAULT_STRENGTH = 1.0f;
    const float DEFAULT_SPEED = 1.0f;
    
    class Character
    {
    public:
        Character(int numberOfLimbs = 6, glm::vec2 rangeOfLimbSize = glm::vec2(0.2f, 1.5f),
                  glm::vec2 rangeOfStats = glm::vec2(0.2f, 1.5f), glm::vec2 position = glm::vec2(250.0f,500.0f), bool isPlayer = true);
        ~Character();
        
        void CreateJoints(PhysicsWorld* physicsWorld);
        
        void Update(double delta);
        void Draw();

        bool GetIsPlayer();
        float GetHealth();
        float GetDamage();
        Weapon* GetWeapon();

        void SwingInDirection(glm::vec2 oldDirection, glm::vec2 newDirection);
        void ThrowBomb(glm::vec2 oldDirection, glm::vec2 newDirection);
        bool DealDamage(Limb* limb, float damage);

        void MoveCharacter(glm::vec2 newPosition);
        glm::vec2 GetCharacterPosition();

        void AddLimb(Limb* limb);
        std::vector<Limb*> GetLimbs();
        Limb* GetLimbAtIndex(int index);
        int GetIndexForLimb(Limb* limb);
        
        void DestroyCharacter();
        void SetDestroyCharacter(bool destroy);
        
        void SetDestructionSlating(bool destruction);
        bool GetSlatedToDestroy();

    private:
        //Private functions
        void StopSwing();
        Limb* CreateALimb(std::string type, glm::vec2 position, glm::vec2 size, glm::vec3 stats, float angle);
        void RemoveLimb(Limb* limb);
        
        //Timer
        Timer* m_Timer;
        Timer* m_DamageTimer;
        
        //World for joint destruction
        PhysicsWorld* m_World;
        
        //Collections
        std::vector<Limb*> m_Limbs;
        std::vector<b2Joint*> m_Joints;
        
        //Player info
        bool m_IsPlayer;
        float m_NumberOfLimbs;
        Weapon* m_Weapon;
        bool m_SwingLeft;
        bool m_SlatedForDestruction;
        bool m_DestroyPlayer;
        bool m_Destroyed;
        
        //Stats
        float m_Health;
        float m_Strength;
        float m_Speed;
    };
}
#endif /* defined(__GameDev2D__Character__) */
