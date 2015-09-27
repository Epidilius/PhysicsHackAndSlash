//
//  CharacterManager.h
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__CharacterManager__
#define __GameDev2D__CharacterManager__

#include <stdio.h>

using glm::vec2;

//Character Manager creates characters, which in turn are composed of limbs
namespace GameDev2D
{
    enum CharacterTypes
    {
        CharacterTypePlayer = 0,
        CharacterTypeEnemy
    };
    
    enum SpawnPoints
    {
        SpawnPointOne = 0,
        SpawnPointTwo,
        SPawnPointThree,
        SpawnPointFour
    };

    //Local Constants
    //Touch Stuff
    const float SECONDS_TO_WAIT_FOR_SWIPE_EVENT = 0.1f;
    
    //Character destroy
    const float SECONDS_TO_WAIT_FOR_CHARACTER_DESTRUCTION = 3.0f;
    
    const float SECONDS_TO_WAIT_FOR_ENEMY_MOVEMENT = 2.0f;

    const std::string WEAPON_TYPE = "WEAPON";
    const std::string LIMB_TYPE = "LIMB";

    //Spawn locations
    const vec2 PLAYER_SPAWN = vec2(200.0f, 200.0f);
    const vec2 SPAWN_LOCATION_ONE = vec2(320, 320); //TODO: Make more spawn points
    const vec2 SPAWN_LOCATION_TWO = vec2(1200, 1200); //TODO: Make more spawn points
    const vec2 SPAWN_LOCATION_THREE = vec2(-960, -400); //TODO: Make more spawn points
    const vec2 SPAWN_LOCATION_FOUR = vec2(960, -1280); //TODO: Make more spawn points

    const vec2 SPAWN_LOCATION_X_RANGE = vec2(0, 96);   //Three meter range of spawning. Change based on single largest gap between spawn point and a wall
    const vec2 SPAWN_LOCATION_Y_RANGE = vec2(0, 96);

    const unsigned short MAX_NUMBER_OF_CHARACTERS = 4;
    const unsigned short MIN_NUMBER_OF_LIMBS = 4;
    const unsigned short MAX_NUMBER_OF_LIMBS = 10;
    const vec2 MIN_LIMB_RANGE_SIZE = vec2(0.1f, 1.5f);
    const vec2 MAX_LIMB_RANGE_SIZE = vec2(1.6f, 3.0f);
    const vec2 MIN_STAT_RANGE = vec2(0.5f, 2.0f);
    const vec2 MAX_STAT_RANGE = vec2(2.0f, 4.0f);

    //Forward declarations
    class Character;
    class Random;
    class TouchEvent;
    class Timer;
    class Limb;
    class PhysicsWorld;
    
    class CharacterManager
    {
    public:
        CharacterManager(float width, float height, PhysicsWorld* physicsWorld);
        ~CharacterManager();
        
        void Update(double delta);
        void Draw();

        void CreateCharacter(CharacterTypes type = CharacterTypeEnemy);
        void RemoveCharacterAtIndex(int index);
        void RemoveCharacter(Character* character);
        
        int GetIndexOfCharacter(Character* character);
        Character* GetCharacterAtIndex(int index);
        std::vector<Character*> GetCharacters();
        
        void PrepareToDestroyCharacterAtIndex(int index);
        void DestroyCharacter(Character* character);

        //Conveniance method to handle touch events
        void HandleTouchEvent(TouchEvent* touchEvent);

        //b2Contact methods
        void OnContactStart(b2Contact* contact);
        void OnContactEnd(b2Contact* contact);
    private:
        //Private functions
        void MoveEnemies();
        
        //Member Variables
        Random* m_Random;
        std::vector<Character*> m_Characters;
        std::vector<Limb*> m_FloatingLimbs;
        PhysicsWorld* m_PhysicsWorld;
        bool m_DestroyingCharacter;
        unsigned int m_CurrentSpawn;
        
        //Timers
        std::__1::map<Character*, Timer*> m_Timers;
        Timer* m_MoveEnemies;
        
        //Touch Related Stuff
        float m_WidthOfScreen;
        float m_HeightOfScreen;
        Timer* m_SwipeTimer;
        bool m_IsSwiping;
        vec2 m_BeginSwipeLocation;
        unsigned char m_CurrentTouches;
        std::vector<vec2> m_CurrentTouchLocations;
    };
}

#endif /* defined(__GameDev2D__CharacterManager__) */
