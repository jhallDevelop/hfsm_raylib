#pragma once
#include <memory>
#include "AI_State.h"
#include "Pawn.h"

class AI_HSFM {
public:
    AI_HSFM();
    ~AI_HSFM() = default;

    void InitializeStates(Pawn& _agent);
    void Update(Pawn& _agent, Actor& _target);  // Update the state machine
    
    const AI_State* GetCurrentState() const { return currentAIStatePtr; }  // Get the current state pointer
    void TransitionToState(Pawn& _agent, e_AI_StateID _nextStateID) ;  // Change to a new state
private:
    AI_State* currentAIStatePtr;  // Pointer to the current AI state object

    // Unique pointers to AI states
    std::unique_ptr<AI_State> seekState;  // Pointer to the seek state
    std::unique_ptr<AI_State> wanderState;  // Pointer to the wander state
    std::unique_ptr<AI_State> attackState;  // Pointer to the attack state
    std::unique_ptr<AI_State> reloadState;  // Pointer to the reload state
    std::unique_ptr<AI_State> shootState;   // Pointer to the shoot state
    std::unique_ptr<AI_State> pathfindState; // Pointer to the pathfinding state
    // Add more states as needed

    // Transition to a new state
    // needs protecting as transitions handled in the FSM update function
    
};