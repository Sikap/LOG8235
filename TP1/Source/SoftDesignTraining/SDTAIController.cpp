// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SDTUtils.h"

void ASDTAIController::Tick(float deltaTime)
{

	if (!m_initialized) {
		// We must ovveride some values in the charactermovement compoent to use our values
		UCharacterMovementComponent* movCompoent = static_cast<UCharacterMovementComponent*>(GetPawn()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		movCompoent->MaxAcceleration = m_accel;
		movCompoent->MaxWalkSpeed = m_max_speed;
		m_initialized = true;
	}


	
	if (!DeathFloorAvoidance())
		WallAvoidance();
	
	// Speed is controlled by an acceleration value
	// By defayult the pawn keeps accelerating in the direction it is currently going
	// If the pawns speed is null we give the direction forward
	m_speed = m_speed + (m_accel * deltaTime) * m_speed.GetSafeNormal(0.0001, GetPawn()->GetActorForwardVector());


	// Pawn has a max speed we must cap it
	if (m_speed.Length() >= m_max_speed) {
		m_speed = m_speed.GetSafeNormal() * m_max_speed;
	}

	// The pawn must face the direction of the movement
	GetPawn()->FaceRotation(m_speed.Rotation());

	// Movement direction is induced by speed vector
	GetPawn()->AddMovementInput(m_speed, 1);
	
}

void ASDTAIController::WallAvoidance() {

	FVector startPos = GetPawn()->GetActorLocation();
	FVector endPos = startPos + GetPawn()->GetActorForwardVector() * m_wall_cast;

	FVector endPosRight = startPos + GetPawn()->GetActorForwardVector().RotateAngleAxis(15, FVector::UpVector) * m_wall_cast;
	FVector endPosLeft = startPos + GetPawn()->GetActorForwardVector().RotateAngleAxis(-15, FVector::UpVector) * m_wall_cast;

	// If we hit something (a wall)
	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
	DrawDebugLine(GetWorld(), startPos, endPosLeft, FColor::Green);
	DrawDebugLine(GetWorld(), startPos, endPosRight, FColor::Blue);

	float offset = 0;
	if (SDTUtils::Raycast(GetWorld(), startPos, endPos)) {
		offset = -3;
	}
	if (SDTUtils::Raycast(GetWorld(), startPos, endPosLeft)) {
		offset = 3;
	}
	if (SDTUtils::Raycast(GetWorld(), startPos, endPosRight)) {
		offset = -3;
	}

	// We induce a small rotation offset to the right of the current direction
	m_speed = m_speed.RotateAngleAxis(offset, FVector(0, 0, 1));
}

bool ASDTAIController::DeathFloorAvoidance() {

	FVector startPos = GetPawn()->GetActorLocation();
	FVector endPos = startPos + GetPawn()->GetActorForwardVector() * m_side_cast;
	FVector endPosDown = startPos + GetPawn()->GetActorForwardVector().RotateAngleAxis(10, GetPawn()->GetActorRightVector()) * m_side_cast;


	DrawDebugSphere(GetWorld(), endPos, 100, 32, FColor::Black);

	FHitResult hitData;
	TArray<AActor*> actorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectsType;
	objectsType.Add(UEngineTypes::ConvertToObjectType(COLLISION_DEATH_OBJECT));
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), endPos, endPos, 100, objectsType, false, actorsToIgnore, EDrawDebugTrace::None, hitData, false);
	// If we hit something (a deathFloor)
	if (hitData.bBlockingHit) {
		FVector deathFloorDir = hitData.Location - GetPawn()->GetActorLocation() ;
		DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), hitData.Location, FColor::Orange);
		if(GetPawn()->GetActorForwardVector().Dot(deathFloorDir) > 0) {
			// We induce a small rotation offset to the right of the current direction
			float offset = -3;
			m_speed = m_speed.RotateAngleAxis(offset, FVector(0, 0, 1));
			return true;
		}
	}

	
	return false;
}

