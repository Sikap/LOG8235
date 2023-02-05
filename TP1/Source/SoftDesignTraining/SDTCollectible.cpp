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

    if (isMoveable) {
        switch (state) {
        case ASDTCollectibleState::InitialState: {

            speed = 5.f;
            acceleration = 2.5f;

            FVector positon = GetActorLocation();
            speed += acceleration * deltaTime;

            // detect the wall
            FVector endPosRight = positon + GetActorRightVector() * 400.f;
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::LeftAcceleration;
            }
            else {
                positon += GetActorRightVector() * speed;
                SetActorLocation(positon);

            }
        }
                                               break;
        case ASDTCollectibleState::RightAcceleration:
        {

            acceleration = 2.5f;

            FVector positon = GetActorLocation();
            speed += acceleration * deltaTime;

            // detect the wall
            FVector endPosRight = positon + (GetActorRightVector() * m_raycast) + GetActorRightVector();
            if (SDTUtils::Raycast(GetWorld(), positon, endPosRight)) {
                state = ASDTCollectibleState::LeftAcceleration;
            }
            else {
                positon += GetActorRightVector() * speed;
                SetActorLocation(positon);
                m_raycast += 0.01f;

            }
        }
        break;

        case ASDTCollectibleState::LeftAcceleration:
        {

            acceleration = -2.5f;
            speed += acceleration * deltaTime;

            // detect the wall
            FVector positon = GetActorLocation();
            FVector endPosLeft = positon - (GetActorRightVector() * m_raycast) - GetActorRightVector();
            if (SDTUtils::Raycast(GetWorld(), positon, endPosLeft)) {
                state = ASDTCollectibleState::RightAcceleration;
            }
            else {
                positon += GetActorRightVector() * speed;
                SetActorLocation(positon);
                m_raycast += 0.01f;
            }
        }
        break;
        case ASDTCollectibleState::CoolDown:
            m_raycast = 1850.f;
            SetActorLocation(initialPosition);
            state = ASDTCollectibleState::InitialState;
            break;
        }


    }

}
