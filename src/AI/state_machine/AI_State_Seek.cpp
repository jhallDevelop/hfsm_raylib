#include "AI_State_Seek.h"
#include <iostream>
#include "raylib.h"

// Constants for the state
const char* SEEK_STATE_NAME = "Seek";

AI_State_Seek::AI_State_Seek() : AI_State(e_AI_StateID::Seek, SEEK_STATE_NAME)
{
}


void AI_State_Seek::OnEnter(Pawn &_agent)
{
}

e_AI_StateID AI_State_Seek::OnUpdate(Pawn& _agent, Actor& _target)
{
    // If the target escapes our range, switch to wander state
    // We actually should be adding the radius of the agent to the distance check, but this is close enough for now
    float distanceToTarget = Vector2Distance(_target.GetPosition(), _agent.GetPosition());
    if (distanceToTarget >= maxSeekDistance){
        // Transition to wander state
        return e_AI_StateID::Wander; // Switch to Wander state
    } else if (distanceToTarget <= minSeekDistance) {
        // If we are close enough to the target, we stop seeking
        return e_AI_StateID::Attack; // Stop seeking if we are close enough to the target
    }   
    // 1. Calculate the desired velocity vector.
    // It MUST be (target - agent) to point towards the target.
    Vector2 desired = Vector2Subtract(_target.GetPosition(), _agent.GetPosition());

    // 2. Normalize the desired vector and scale it to the agent's max speed.
    desired = Vector2Normalize(desired);
    desired = Vector2Scale(desired, _agent.GetSpeed());

    // 3. Calculate the steering force required to correct the course.
    Vector2 steer = Vector2Subtract(desired, _agent.GetVelocity());

    // 4. Limit the steering force to the agent's maximum turning force.
    if (Vector2LengthSqr(steer) > (_agent.GetMaxForce() * _agent.GetMaxForce())) {
        steer = Vector2Normalize(steer);
        steer = Vector2Scale(steer, _agent.GetMaxForce());
    }

    // 5. Apply the force.
    // Draw a line from the agent to the target for debugging
    DrawLineEx(_agent.GetPosition(), _target.GetPosition(), 2.0f, RED);

    _agent.ApplyForce(steer);

    // 6. Return "None" to stay in this state.
    return e_AI_StateID::None;
}

void AI_State_Seek::OnExit(Pawn &_agent)
{
}
