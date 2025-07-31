#pragma once
#include "AI_State.h"


class AI_State_Wander : public AI_State {
public:
    AI_State_Wander();
    ~AI_State_Wander() override = default;
    void OnEnter(Pawn &_agent) override;   // Called when entering the wander state
    e_AI_StateID OnUpdate(Pawn& _agent, Pawn& _target) override;  // Called every frame while in the wander state
    void OnExit(Pawn &_agent) override;    // Called when exiting the wander state
private:
    const float minDistanceToTarget = 80.0f; // Minimum distance to the target to stop wandering
    const float maxJitter = 0.5f; // How much the angle can change per frame.
    const float wanderRadius = 50.0f; // Radius of the wander circle.
    // The current angle for the wander target.
    float wanderAngle = 0.0f;
};