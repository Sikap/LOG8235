// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"
#include "SDTCollectible.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SDTUtils.h"


/*
	This controller must react to the environement and has key features
	 - Move toward a point using a speed vector and an acceleration
	 - Wall avoidance
	 - DeathFloor avoidance
	 - Detection and pickup of collectibles
	 - Move towards and away from player
	
	In order to control the behaviour of the movement the movement method
	takes a target point and is responsible for wall and deathtrap avoidance
	
	The target point is defined through detections, depending on the state of the agent 

	The agent has 4 'States' Roaming, goingForAPickup, GoingForPlayer, AwayForPlayer



	Roaming:						GetArandom point in front of the actor to reach, DoDetections for collectibles (if collectible found, change state to GoingForAPickup and ovveride target point)
	GoingForAPickup :				DoDetectionForPlayer (if player found, change state to GoingForPlayer or AwayForPlayer and ovveride target point )
	GoingForPlayer, AwayForPlayer:  DoDetectionForPlayer (if player NOT found, change state to Roaming)

	If the agent reaches the target point, we goto Roaming
	
	We use euleur method for movement 
	

*/



void ASDTAIController::Tick(float deltaTime)
{

	if (!m_initialized) {
		// We must ovveride some values in the charactermovement compoent to use our values
		UCharacterMovementComponent* movCompoent = static_cast<UCharacterMovementComponent*>(GetPawn()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		movCompoent->MaxAcceleration = m_accel;
		movCompoent->MaxWalkSpeed = m_max_speed;
		m_initialized = true;
	}

	// Depending on the state we execute different behaviour
	switch (state) {

		case ASDTAIState::Idle:
			// We check detections and if nothing found we generate random point to go to
			m_targetPoint = runIdleState();
			//DrawDebugSphere(GetWorld(), m_targetPoint, 30, 30, FColor::Red);
			break;
		
		case ASDTAIState::Roaming:
			//We are currently going to a RandomPoint, but we need to check detections and ovveride if collectible or player
			m_targetPoint = MoveTowardsPickup();
			m_targetPoint = PlayerDetection();
			//DrawDebugSphere(GetWorld(), m_targetPoint, 30, 30, FColor::Blue);
			break;

		case ASDTAIState::GoingForPickup:
			// We are currently going for a collectible, but we need to check detections for player
			m_targetPoint = MoveTowardsPickup();
			m_targetPoint = PlayerDetection();
			//DrawDebugSphere(GetWorld(), m_targetPoint, 90, 30, FColor::Purple);
			break;

		case ASDTAIState::Attacking:
			// We are currently going for a player but we need to verify that we can still reach the player (if not go back to idle for next frame)
			m_targetPoint = PlayerDetection();
			//DrawDebugSphere(GetWorld(), m_targetPoint, 30, 30, FColor::Black);
			break;

		case ASDTAIState::Fleeing:
			// We are currently running away from a player but we need to verify that we are still seeing the player (if not go back to idle for next frame)
			m_targetPoint = PlayerDetection();
			//DrawDebugSphere(GetWorld(), m_targetPoint, 30, 30, FColor::Black);
			break;


			//Should never happen 
		default: UE_LOG(LogTemp, Error, TEXT("Undefined state"));
	}

	// The functions called before will change the m_targetPoint variable
	// Function call to move towards target point 
	bool hasArrived = MoveToTargetPoint(deltaTime);
	if (hasArrived) {
		state = ASDTAIState::Idle;
		m_targetPoint = UNDEFINED_TARGET_POINT;
	}
}

FVector ASDTAIController::runIdleState() {
	
	m_targetPoint = MoveTowardsPickup();
	m_targetPoint = PlayerDetection();

	if (m_targetPoint == UNDEFINED_TARGET_POINT || !IsInsideCone(m_targetPoint)) {
		// FindRandomPoint
		FVector2D point = FMath::RandPointInCircle(200);
		// Verify that random point is reachable
		while (!IsInsideCone(GetPawn()->GetActorLocation() + FVector(point, 0))) {
			point = FMath::RandPointInCircle(200);
		}
		m_targetPoint = GetPawn()->GetActorLocation() + FVector(point, 0);
		state = ASDTAIState::Roaming;
	}
	return m_targetPoint;
}

bool ASDTAIController::MoveToTargetPoint(float deltaTime) {

	FVector direction = m_targetPoint - GetPawn()->GetActorLocation();
	direction.Normalize(0.0001);
	
	// Speed is controlled by an acceleration value
	// By defayult the pawn keeps accelerating in the direction it is currently going
	m_speed = m_speed + (m_accel * deltaTime) * direction;

	if (!DeathFloorAvoidance()) {
		WallAvoidance();
	}

	// Pawn has a max speed we must cap it
	if (m_speed.Length() >= m_max_speed) {
		m_speed = m_speed.GetSafeNormal() * m_max_speed;
	}

	// The pawn must face the direction of the movement
	GetPawn()->FaceRotation(m_speed.Rotation());
	// Movement direction is induced by speed vector
	GetPawn()->AddMovementInput(m_speed, 1);
	if (!IsInsideCone(m_targetPoint)) { return true; }
	return FVector::Distance(m_targetPoint, GetPawn()->GetActorLocation()) <= 60;
}

void ASDTAIController::WallAvoidance() {

	FVector startPos = GetPawn()->GetActorLocation();
	FVector endPos = startPos + GetPawn()->GetActorForwardVector() * m_wall_cast;

	FVector endPosRight = startPos + GetPawn()->GetActorForwardVector().RotateAngleAxis(15, FVector::UpVector) * m_wall_cast;
	FVector endPosLeft = startPos + GetPawn()->GetActorForwardVector().RotateAngleAxis(-15, FVector::UpVector) * m_wall_cast;

	// If we hit something (a wall)
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
	//DrawDebugLine(GetWorld(), startPos, endPosLeft, FColor::Green);
	//DrawDebugLine(GetWorld(), startPos, endPosRight, FColor::Blue);

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

	FHitResult hitData;
	TArray<AActor*> actorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectsType;
	objectsType.Add(UEngineTypes::ConvertToObjectType(COLLISION_DEATH_OBJECT));
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), endPos, endPos, 150, objectsType, false, actorsToIgnore, EDrawDebugTrace::None, hitData, false);
	
	
	//DrawDebugSphere(GetWorld(), endPos, 150, 32, FColor::Black);
	// If we hit something (a deathFloor)
	if (hitData.bBlockingHit) {
		FVector deathFloorDir = hitData.Location - GetPawn()->GetActorLocation() ;
		if(GetPawn()->GetActorForwardVector().Dot(deathFloorDir) > 0) {
			// We induce a small rotation offset to the right of the current direction
			float offset = -3;
			m_speed = m_speed.RotateAngleAxis(offset, FVector(0, 0, 1));
			return true;
		}
	}
	return false;
}

FVector ASDTAIController::MoveTowardsPickup() {
	FVector startPos = GetPawn()->GetActorLocation();

	TArray <FHitResult> hitData;
	FCollisionObjectQueryParams objectQueryParams; 
	objectQueryParams.AddObjectTypesToQuery(COLLISION_COLLECTIBLE);
	FCollisionShape collisionShape;     
	collisionShape.SetSphere(m_Collectebale_detection_reduis);
	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;      
	GetWorld()->SweepMultiByObjectType(hitData, startPos, startPos, FQuat::Identity, objectQueryParams, collisionShape, queryParams);
	
	//DrawDebugSphere(GetWorld(), startPos, m_Collectebale_detection_reduis, 32, FColor::Blue);

	Algo::Sort(hitData, [&](FHitResult lhs, FHitResult rhs)
	{		
		return FVector::Distance(lhs.TraceStart, lhs.Location) < FVector::Distance(lhs.TraceStart, rhs.Location);
	});


	for (int i = 0; i < hitData.Num(); ++i) {
		ASDTCollectible* ref = Cast<ASDTCollectible>(hitData[i].GetActor());
		if (ref != nullptr && IsInsideCone(hitData[i].Location) && ref->GetStaticMeshComponent()->IsVisible()) 
		{
			state = ASDTAIState::GoingForPickup;
			return hitData[i].GetActor()->GetActorLocation();
		}
	}

	return m_targetPoint;
}

bool ASDTAIController::IsInsideCone(FVector targetActor) const
{
	APawn* pawn = GetPawn();
	auto pawnForwardVector = pawn->GetActorForwardVector();
	auto dir = targetActor - pawn->GetActorLocation();
	auto value = FVector::DotProduct(dir.GetSafeNormal(), pawnForwardVector.GetSafeNormal());
	auto angle = FMath::Acos(value);
	auto isVisible = FMath::Abs(angle) <= m_vision_angle;
	bool hitWall = SDTUtils::Raycast(GetWorld(), pawn->GetActorLocation(), targetActor);
	return isVisible && !hitWall;
}

FVector ASDTAIController::PlayerDetection() {
	/*
		TODO:
		- Sphere detection for player
		- If player has pickup : run away
		- If player does not have piclup : go towards
		- Would be nice to do like exo 2 (use player speed to influence direction)
			
		Should return the position to go to regardless of runnign away or going forward
		Also Should set the state of the agent using below before returning
		state = ASDTAIState::STATE
	*/
	return m_targetPoint;
}
