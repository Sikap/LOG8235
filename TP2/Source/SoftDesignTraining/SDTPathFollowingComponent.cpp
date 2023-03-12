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


	if (!SDTUtils::HasJumpFlag(segmentStart))
	{
		Super::FollowPathSegment(DeltaTime);
	}
	else {
		Super::FollowPathSegment(DeltaTime);
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
		
	}
	else
	{
		//Handle normal segments
		Cast<UCharacterMovementComponent>(MovementComp)->SetMovementMode(MOVE_NavWalking);
	}
}


