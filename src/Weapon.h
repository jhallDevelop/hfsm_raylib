#pragma once
/* ===============
Derived class for player-controlled pawns.
=================*/
#include "Actor.h"

class Weapon : public Actor {

public:
    Weapon();
    ~Weapon() override;

    void CreateWeapon(int _ammoutCount, float _fireRate, float _reloadTime);   // Weapon factory method
    void Update() override;
    void Render() override;
    void Destroy() override;

    // Gameplay attributes
    bool Fire(); // Method to fire the weapon
    void StartReload(); // Method to reload the weapon
    bool IsReloading() const;

    // Setters and Getters
    int GetCurrentAmmoCount() const; 
    void SetCurrentAmmoCount(int _count); 
    int GetMaxAmmoCount() const; 
    void SetMaxAmmoCount(int _count); 


    float GetCurrentFireRate() const;
    void SetCurrentFireRate(float _fireRate);
    float GetMaxFireRate() const;
    void SetMaxFireRate(float _fireRate);


    float GetCurrentReloadTime() const;
    void SetCurrentReloadTime(float _reloadTime);
    float GetMaxReloadTime() const;
    void SetMaxReloadTime(float _reloadTime);

private:
    bool isReloading; // Flag to check if the weapon is reloading

    // Pawn specific attributes for Physics
    int currentAmmoCount; // Current ammo count
    int maxAmmoCount; // Maximum ammo count

    float currentFireRateTime; // Timer for the fire rate
    float maxFireRate; // Time between shots

    float currentReloadtime; // Time to reload
    float maxReloadTime; // Time to reload
};