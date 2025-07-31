#include "AI_State_Attack.h"
#include <iostream>
#include "raylib.h"

// Constants for the state
const char* ATTACK_STATE_NAME = "Attack";

AI_State_Attack::AI_State_Attack() : AI_State(e_AI_StateID::Attack, ATTACK_STATE_NAME)
{
}

void AI_State_Attack::OnEnter(Pawn &_agent)
{
}

e_AI_StateID AI_State_Attack::OnUpdate(Pawn &_agent, Pawn &_target)
{
    // If the target escapes our range, switch to Seek state
    float distanceToTarget = Vector2Distance(_target.GetPosition(), _agent.GetPosition());
    if (distanceToTarget >= maxAttackDistance){
        // Transition to Seek state
        return e_AI_StateID::Seek; 
    }

    // Stop moving
    _agent.SetVelocity({0, 0}); // Stop the agent's movement whilst we shoot and reload

    Weapon* agentWeapon = _agent.GetWeapon();
    
    // Tell the weapon to update its internal timers.
    agentWeapon->Update();

    // Ask the weapon to fire. The Fire() method now contains all the logic
    //    to check for ammo and fire rate. It returns true if a shot was fired.
    if (agentWeapon->Fire()) {
        // The shot was successful. Transition to the Shoot state to play the effect.
        return e_AI_StateID::Shoot;
    }
    
    // Check if the weapon is out of ammo.
    if (agentWeapon->GetCurrentAmmoCount() <= 0) {
        // It's out of ammo, so we need to reload.
        
        return e_AI_StateID::Reload;
    }

    // If we are here, it means we have ammo but the fire rate timer is not ready.
    //    So, we do nothing and wait.
    return e_AI_StateID::None;

}

void AI_State_Attack::OnExit(Pawn &_agent)
{
     // Reload the fire rate time
     
     
    
}

