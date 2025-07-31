#include "AI_HFSM.h"
#include "Pawn.h"
#include "AI_State_Seek.h"
#include "AI_State_Wander.h"
#include "AI_State_Attack.h"
#include "AI_State_Reload.h"
#include "AI_State_Shoot.h"
#include <iostream>
AI_HSFM::AI_HSFM() {
    // Initialize the AI state machine to the idle state
}

void AI_HSFM::InitializeStates(Pawn& _agent)
{
    // Initialize the states for the AI state machine using polymorphic pointers
    // Seek State
    seekState = std::make_unique<AI_State_Seek>();
    // Wander State
    wanderState = std::make_unique<AI_State_Wander>();
    // Attack State
    attackState = std::make_unique<AI_State_Attack>();
    // Reload State
    reloadState = std::make_unique<AI_State_Reload>(); 
    // Shoot State
    shootState = std::make_unique<AI_State_Shoot>();

    // Add new states to the state machine as needed

    // Set the initial state to seek
    currentAIStatePtr = wanderState.get();  // Set the current state pointer to the seek state
    currentAIStatePtr->OnEnter(_agent);  // Call the OnEnter method of the initial state
    std::cout << "Initial state: " << currentAIStatePtr->GetStateName() << std::endl;
}

void AI_HSFM::Update(Pawn& _agent, Pawn& _target)
{
    // Update logic based on the current state
    if(currentAIStatePtr == nullptr) {
        return; // If no current state, do nothing
    }
    // The update will return the next state ID based on the current state logic
    e_AI_StateID nextStateID = currentAIStatePtr->OnUpdate(_agent, _target);  // Call OnUpdate for the current state

    if (nextStateID != e_AI_StateID::None) {
        TransitionToState(_agent, nextStateID);
    }
}

void AI_HSFM::TransitionToState(Pawn& _agent, e_AI_StateID _nextStateID)  {
    if(currentAIStatePtr != nullptr && _nextStateID == currentAIStatePtr->GetStateID()) {
        std::cout << "Already in the desired state, no transition needed." << std::endl;
        return; // No transition needed if already in the desired state
    }

    if(currentAIStatePtr != nullptr) {
       currentAIStatePtr->OnExit(_agent);  // Call OnExit for the current state
    }

    AI_State* newStatePtr = nullptr;
    switch (_nextStateID) {
        case e_AI_StateID::None:
            // No state change, do nothing
            break;
        case e_AI_StateID::Seek:
            newStatePtr = seekState.get();
            break;
        case e_AI_StateID::Wander:
            newStatePtr = wanderState.get();
            break;
        case e_AI_StateID::Attack:
            newStatePtr = attackState.get();
            break;
        case e_AI_StateID::Reload:
            newStatePtr = reloadState.get();
            break;
        case e_AI_StateID::Shoot:
            newStatePtr = shootState.get();
            break;
        // Add more cases for additional states as needed
        default:
            break; // Handle other states as needed
    }

    // Change to the new state
    currentAIStatePtr = newStatePtr;
    std::cout << "Transitioning to: " << currentAIStatePtr->GetStateName() << std::endl;
    currentAIStatePtr->OnEnter(_agent);  // Call the OnEnter method of the new state


}