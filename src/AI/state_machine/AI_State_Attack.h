#pragma once
#include "AI_State.h"

class AI_State_Attack : public AI_State {
public:
    AI_State_Attack();
    ~AI_State_Attack() override = default;

    // Override the AI_State methods
    void OnEnter(Pawn &_agent) override;   // Called when entering the seek state
    e_AI_StateID OnUpdate(Pawn& _agent, Pawn& _target) override;  // Called every frame while in the seek state
    void OnExit(Pawn &_agent) override;    // Called when exiting the seek state
private:
    const float maxAttackDistance = 20.0f;
    
    
    
};