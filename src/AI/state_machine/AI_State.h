#pragma once
#include "Pawn.h"

enum class e_AI_StateID {
    None,
    Wander,
    Seek, 
    Attack,
    Reload,
    Shoot, 
    Pathfind
    // Add more states as needed
};

class AI_State {
public:
    AI_State(e_AI_StateID id, const char* name) : stateName(name), stateID(id) {};
    virtual ~AI_State() = default;
    virtual void OnEnter(Pawn &_agent) = 0;   // Called when entering the state
    virtual e_AI_StateID OnUpdate(Pawn& _agent, Actor& _target) = 0;  // Called every frame while in the state
    virtual void OnExit(Pawn &_agent) = 0;    // Called when exiting the state

    // Getters/Setters
    const char* GetStateName() const { return stateName; }  // Get the state name for debugging
    e_AI_StateID GetStateID() const { return stateID; }  // Get the state ID
    
protected:
    const char* stateName;  // Name of the state for debugging
    const e_AI_StateID stateID;  // Unique ID for the state
};