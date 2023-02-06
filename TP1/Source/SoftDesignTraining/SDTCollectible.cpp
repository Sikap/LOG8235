// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTCollectible.h"
#include "SoftDesignTraining.h"
#include "SDTUtils.h"

ASDTCollectible::ASDTCollectible()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASDTCollectible::BeginPlay()
{
    Super::BeginPlay();
    initialPosition = GetActorLocation();
}

void ASDTCollectible::Collect()
{
    state = ASDTCollectibleState::CoolDown;
    GetWorld()->GetTimerManager().SetTimer(m_CollectCooldownTimer, this, &ASDTCollectible::OnCooldownDone, m_CollectCooldownDuration, false);
    GetStaticMeshComponent()->SetVisibility(false);
    OnSomeEvent.Broadcast();
}

void ASDTCollectible::OnCooldownDone()
{
    SetActorLocation(initialPosition);
    GetWorld()->GetTimerManager().ClearTimer(m_CollectCooldownTimer);
    GetStaticMeshComponent()->SetVisibility(true);
    state = ASDTCollectibleState::InitialState;
}

bool ASDTCollectible::IsOnCooldown()
{
    return m_CollectCooldownTimer.IsValid();
}

void ASDTCollectible::Tick(float deltaTime)
{

    if (isMoveable) {
        switch (state) {


        case ASDTCollectibleState::InitialState: {
            // In the initial state the collectible has a speed of 5m/s 
            // In this state it moves in the direction of the right forward vector of the collectible
            // Units are usualy in cm so we multiply by 100
            speed = 5.0f;
            acceleration = 2.5f;

            FVector positon = GetActorLocation();
            FVector direction = GetActorRightVector();
            direction.Normalize(0.001f);
            // speed += acceleration * deltaTime;

            // We are travelling at 500 cm/s and need 2 seconds to reach  0 velocity 
            // We must then verify the wall distance at 400 cm (4 m) 
            FVector endPosRight = positon + direction * 400.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::LeftAcceleration;
            }
            else {
               
                positon +=  direction * speed;
                SetActorLocation(positon);

            }
        }
                                               break;
        case ASDTCollectibleState::RightAcceleration:
        {

            acceleration = 2.5f;
            speed += acceleration * deltaTime;

            // We must cap the speed of the collectible to -500 cm/s
            if (speed >= 5.0f) {
                speed = 5.0f;
            }

            FVector positon = GetActorLocation();
            FVector direction = GetActorRightVector();
            direction.Normalize(0.001f);

            // detect the wall
            // FVector endPosRight = positon + (GetActorRightVector() * m_raycast) + GetActorRightVector();
            FVector endPosRight = positon + GetActorRightVector() * 400.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::LeftAcceleration;
            }
            else {
                positon += direction * speed;
                SetActorLocation(positon);
                m_raycast += 0.01f;

            }
        }
        break;

        case ASDTCollectibleState::LeftAcceleration:
        {

            acceleration = -2.5f;
            speed += acceleration * deltaTime;

            // We must cap the speed of the collectible to -500 cm/s
            if (speed <= -5.0f) {
                speed = -5.0f;
            }

            FVector direction = GetActorRightVector();
            direction.Normalize(0.001f);
            FVector positon = GetActorLocation();
            // FVector endPosLeft = positon - (GetActorRightVector() * m_raycast) - GetActorRightVector();
            //if (SDTUtils::Raycast(GetWorld(), positon, endPosLeft)) {
            // We are travelling at 500 cm/s and need 2 seconds to reach  0 velocity 
            // We must then verify the wall distance at 400 cm (10 m) 
            FVector endPosRight = positon + direction * -400.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::RightAcceleration;
            }
            else {
                positon += direction * speed;
                SetActorLocation(positon);
                m_raycast += 0.01f;
            }
        }
        break;

        case ASDTCollectibleState::CoolDown:
            //m_raycast = 1850.f;
            //SetActorLocation(initialPosition);
            // state = ASDTCollectibleState::InitialState;
            break;
        }


    }

}
