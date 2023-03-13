// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTPathFollowingComponent.h"
#include "SoftDesignTraining.h"
#include "SDTUtils.h"
#include "SDTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"

USDTPathFollowingComponent::USDTPathFollowingComponent(const FObjectInitializer& ObjectInitializer)
{

}

void USDTPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	const TArray<FNavPathPoint>& points = Path->GetPathPoints();
	const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];
	const FNavPathPoint& segmentEnd = points[MoveSegmentStartIndex + 1];

	if (!SDTUtils::HasJumpFlag(segmentStart))
	{
		Super::FollowPathSegment(DeltaTime);
	}
	else {

		if ((currentJumpT / timeToTravel) > 1) {
			Super::FollowPathSegment(DeltaTime);
			return;
		}
		

		currentJumpT += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("Current progress value %f and total jump time: %f , current jump time: %f, pawn jump speed is : %f  "), currentJumpT / timeToTravel, timeToTravel, currentJumpT, controllerRef->JumpSpeed);
		FVector direction = Path->GetSegmentDirection(MoveSegmentStartIndex + 1);
		//										   m (normalized to cm)       s                m/s   
		FVector dir = segmentStart.Location + (direction * currentJumpT * controllerRef->JumpSpeed);
		float jumpCurveVal = controllerRef->JumpCurve->GetFloatValue(currentJumpT / timeToTravel);
		float playerHeight = basePlayerLocation.Z + controllerRef->JumpApexHeight * jumpCurveVal;

		AActor* pawn = controllerRef->GetPawn();
		pawn->SetActorLocation(FVector(dir.X, dir.Y, playerHeight));
		pawn->SetActorRotation(FVector(dir.X, dir.Y, 0).Rotation());

		
	}
}

void USDTPathFollowingComponent::SetMoveSegment(int32 segmentStartIndex)
{
	Super::SetMoveSegment(segmentStartIndex);

	const TArray<FNavPathPoint>& points = Path->GetPathPoints();

	const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];
	const FNavPathPoint& segmentEnd = points[MoveSegmentStartIndex + 1];
	


	if (SDTUtils::HasJumpFlag(segmentStart) && FNavMeshNodeFlags(segmentStart.Flags).IsNavLink())
	{
		controllerRef->AtJumpSegment = true;
		float distanceToTravel = FVector::Distance(segmentStart.Location, segmentEnd.Location);
		timeToTravel = distanceToTravel / (controllerRef->JumpSpeed);
		currentJumpT = 0;
		basePlayerLocation = controllerRef->GetPawn()->GetActorLocation();
	}
	else
	{
		//Handle normal segments
		currentJumpT = 0;
		//timeToTravel = 0;
		controllerRef->AtJumpSegment = false;
		Cast<UCharacterMovementComponent>(MovementComp)->SetMovementMode(MOVE_NavWalking);
	}
}


