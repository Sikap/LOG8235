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
    GetWorld()->GetTimerManager().SetTimer(m_CollectCooldownTimer, this, &ASDTCollectible::OnCooldownDone, m_CollectCooldownDuration, false);

    GetStaticMeshComponent()->SetVisibility(false);
}

void ASDTCollectible::OnCooldownDone()
{
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
    // Super::Tick(deltaTime);

   /*  // move the actor right
    FVector position = GetActorLocation();
    position += GetActorRightVector() * speed * deltaTime;
    SetActorLocation(position);

    // detect the wall
    FVector startPos = position;
    FVector endPosRight = startPos + GetActorRightVector() * m_wall_cast;
    FVector endPosLeft = startPos - GetActorRightVector() * m_wall_cast;  */
    if (isMoveable) {


        switch (state) {
        case ASDTCollectibleState::InitialState:
            SetActorLocation(initialPosition);
            speed = 5.f;
            acceleration = 0;
            state = ASDTCollectibleState::RightAcceleration;
            break;
        case ASDTCollectibleState::RightAcceleration:
        {
            FVector positon = GetActorLocation();
            speed += acceleration * deltaTime;

            acceleration = 2.5f;

            // detect the wall
            FVector endPosRight = positon + GetActorRightVector() * 1250.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::LeftAcceleration;
            }
            else {
                positon += GetActorRightVector() * speed;
                SetActorLocation(positon);
            }
        }
        break;

        case ASDTCollectibleState::LeftAcceleration:
        {
            acceleration = -2.5f;
            FVector positon = GetActorLocation();
            speed += acceleration * deltaTime;



            // detect the wall
            FVector endPosLeft = positon + GetActorRightVector() * -1900.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosLeft)) {
                state = ASDTCollectibleState::RightAcceleration;
            }
            else {
                positon += GetActorRightVector() * speed;
                SetActorLocation(positon);
            }
        }
        break;
        case ASDTCollectibleState::CoolDown:
            state = ASDTCollectibleState::InitialState;
            break;
        }

    }







}
