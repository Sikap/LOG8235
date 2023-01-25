// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTCollectible.h"
#include "SoftDesignTraining.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SDTUtils.h"

ASDTCollectible::ASDTCollectible()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASDTCollectible::BeginPlay()
{
    Super::BeginPlay();
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
    SetActorLocation(initialPosition);
}

bool ASDTCollectible::IsOnCooldown()
{
    return m_CollectCooldownTimer.IsValid();
}


void ASDTCollectible::Tick(float deltaTime)
{
    //Super::Tick(deltaTime);
    if (isMoveable && !IsOnCooldown()) {
        // Etat initiale
        FVector Location = GetActorLocation();
		//m_currentVelocity +=  m_accel * deltaTime;
		Location += GetActorRightVector() * m_initialSpeed * deltaTime;
		SetActorLocation(Location);
		
    }
    
}

