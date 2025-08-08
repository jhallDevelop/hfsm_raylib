#include "AI_State_Pathfind.h"
#include <iostream>
#include "raylib.h"

// Constants for the state
const char* PATHFIND_STATE_NAME = "Pathfind";

AI_State_Pathfind::AI_State_Pathfind() : AI_State(e_AI_StateID::Pathfind, PATHFIND_STATE_NAME)
{
}


void AI_State_Pathfind::OnEnter(Pawn &_agent)
{
}

e_AI_StateID AI_State_Pathfind::OnUpdate(Pawn& _agent, Actor& _target)
{
    // Add a check to see if we are very close to the target to avoid jittering.
    float distanceToTarget = Vector2Distance(_agent.GetPosition(), _target.GetPosition());
    float stoppingRadius = 5.0f; // An arbitrary small radius. Tune this value.

    if (distanceToTarget < stoppingRadius) {
        // We are at the waypoint. Stop moving and wait for the next one.
        _agent.SetVelocity({0, 0}); // Assuming your Pawn has a way to set velocity directly.
        // You might need a way to signal that you've reached the waypoint so a new one can be targeted.
        return e_AI_StateID::None; // Stay in this state, but stop moving for this frame.
    }

    // --- 1. Calculate the vector towards the target ---
    Vector2 toTarget = Vector2Subtract(_target.GetPosition(), _agent.GetPosition());

    // --- 2. Determine the dominant axis and create a strictly cardinal desired velocity ---
    Vector2 desired = {0, 0};

    // Compare the absolute horizontal distance to the absolute vertical distance
    if (fabsf(toTarget.x) > fabsf(toTarget.y))
    {
        // Horizontal movement is dominant. Move purely left or right.
        // Use copysign to get the direction (1.0f or -1.0f) from toTarget.x
        desired.x = copysignf(1.0f, toTarget.x) * _agent.GetSpeed();
        desired.y = 0;
    }
    else
    {
        // Vertical movement is dominant. Move purely up or down.
        desired.x = 0;
        desired.y = copysignf(1.0f, toTarget.y) * _agent.GetSpeed();
    }
    
    // --- The rest of your steering logic remains the same! ---

    // 3. Calculate the steering force required to correct the course.
    Vector2 steer = Vector2Subtract(desired, _agent.GetVelocity());

    // 4. Limit the steering force to the agent's maximum turning force.
    if (Vector2LengthSqr(steer) > (_agent.GetMaxForce() * _agent.GetMaxForce())) {
        steer = Vector2Normalize(steer);
        steer = Vector2Scale(steer, _agent.GetMaxForce());
    }

    // 5. Apply the force.
    // The debug line now shows the final target, not the immediate path.
    DrawLineEx(_agent.GetPosition(), _target.GetPosition(), 2.0f, RED);

    _agent.ApplyForce(steer);

    // 6. Return "None" to stay in this state.
    return e_AI_StateID::None;
}

void AI_State_Pathfind::OnExit(Pawn &_agent)
{
}
