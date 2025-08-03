#pragma once
#include "AI_State.h"

class AI_State_Seek : public AI_State {
public:
    AI_State_Seek();
    ~AI_State_Seek() override = default;

    // Override the AI_State methods
    void OnEnter(Pawn &_agent) override;   // Called when entering the seek state
    e_AI_StateID OnUpdate(Pawn& _agent, Pawn& _target) override;  // Called every frame while in the seek state
    void OnExit(Pawn &_agent) override;    // Called when exiting the seek state
private:
    // The distance at which the agent considers it has "arrived" at the target.
    const float maxSeekDistance = 80.0f; 
    const float minSeekDistance = 20.0f; // Minimum distance to the target to stop seeking
};