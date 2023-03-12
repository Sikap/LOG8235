// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"
#include "SDTCollectible.h"
#include "SDTFleeLocation.h"
#include "SDTPathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
//#include "UnrealMathUtility.h"
#include "SDTUtils.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NavAreas/NavArea_Obstacle.h"

ASDTAIController::ASDTAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USDTPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ASDTAIController::BeginPlay() {
    Super::BeginPlay();
    USDTPathFollowingComponent* ref = Cast<USDTPathFollowingComponent>(GetPathFollowingComponent());
    ref->controllerRef = this;
}

bool ASDTAIController::GoToPlayer()
{
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return false;

    FVector playerLocation;

    // TODO : Check if player is visible
    if (true) {
        playerLocation = playerCharacter->GetActorLocation();
 
    }
    else {
        // Player is not visible, move to last known position
        playerLocation = lastKnownPlayerLocation;
    }

    return MoveToLocation(playerLocation) == EPathFollowingRequestResult::RequestSuccessful;
}

bool ASDTAIController::GoToFleeLocation()
{
    TArray<AActor*> fleeLocations;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), fleeLocations);
    if (fleeLocations.Num() <= 0)
        return false;

    FVector selfLocation = GetPawn()->GetActorLocation();
    float bestDistance = -1;
    FVector bestLocation;

    // Find closest flee location
    for (AActor* fleeLocation : fleeLocations) {
        float distance = FVector::Distance(selfLocation, fleeLocation->GetActorLocation());

        if (bestDistance < 0 || distance < bestDistance) {
            bestDistance = distance;
            bestLocation = fleeLocation->GetActorLocation();
        }
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation NavLocation;

    // Check if the flee location is valid and reachable
    if (NavSystem->GetRandomPointInNavigableRadius(bestLocation, m_FleeRadius, NavLocation)) {
        return MoveToLocation( NavLocation.Location) == EPathFollowingRequestResult::RequestSuccessful;
    }
    return false;
}


void ASDTAIController::GoToBestTarget(float deltaTime)
{
    
    //Move to target depending on current behavior 

    bool isMoveSuccessful = false;
    switch (state) {


        case ASDTAIState::Attacking:
            isMoveSuccessful = GoToPlayer();
            break;

        case ASDTAIState::Fleeing:
            isMoveSuccessful = GoToFleeLocation();
            break;

        case ASDTAIState::ChasingCollectible:
            isMoveSuccessful = MoveToClosestCollectible();
            break;
    
    }

    if (isMoveSuccessful)
        OnMoveToTarget();
    else
        state = ASDTAIState::ChasingCollectible;
}

// collectible
bool ASDTAIController::MoveToClosestCollectible() {
    TArray<AActor*> collectibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), collectibles);

    float minPathLength = INT_MAX;
    AActor* closestCollectible = nullptr;
    for (AActor* collectible : collectibles) {

        if (Cast<ASDTCollectible>(collectible)->IsOnCooldown()) continue;

        UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());

        UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetPawn()->GetActorLocation(), collectible->GetActorLocation());

        float currentPathLength = path->GetPathLength();

        if (currentPathLength <= minPathLength) {
            minPathLength = currentPathLength;
            closestCollectible = collectible;
        }
    }

    return closestCollectible != nullptr && MoveToLocation(closestCollectible->GetActorLocation()) == EPathFollowingRequestResult::RequestSuccessful;
}

void ASDTAIController::OnMoveToTarget()
{
    m_ReachedTarget = false;
}

void ASDTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    m_ReachedTarget = true;
}

void ASDTAIController::ShowNavigationPath()
{
    //Show current navigation path DrawDebugLine and DrawDebugSphere

}

void ASDTAIController::ChooseBehavior(float deltaTime)
{
    UpdatePlayerInteraction(deltaTime);
    ShowPathToClosestCollectible();
}

void ASDTAIController::UpdatePlayerInteraction(float deltaTime)
{
    //finish jump before updating AI state
    if (AtJumpSegment) {
       // Jump(deltaTime);
        return;
    }

    APawn* selfPawn = GetPawn();
    if (!selfPawn)
        return;

    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return;

    FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * m_DetectionCapsuleForwardStartingOffset;
    FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * m_DetectionCapsuleHalfLength * 2;

    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_COLLECTIBLE));
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(m_DetectionCapsuleRadius));

    FHitResult detectionHit;
    GetHightestPriorityDetectionHit(allDetectionHits, detectionHit);

    // check if player is powered up
    bool isPlayerPoweredUp = SDTUtils::IsPlayerPoweredUp(GetWorld());

    // set the AI state based on player detection
    if (isPlayerPoweredUp && detectionHit.bBlockingHit && detectionHit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
    {
        // Player is powered up 
        state = ASDTAIState::Fleeing;
        lastKnownPlayerLocation = playerCharacter->GetActorLocation();
    }
    else if (!isPlayerPoweredUp && detectionHit.bBlockingHit && detectionHit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
    {
        // Player is not powered up and attacking
        state = ASDTAIState::Attacking;
        lastKnownPlayerLocation = playerCharacter->GetActorLocation();
    }
    else
    {
      
        state = ASDTAIState::ChasingCollectible;
    }
    

    DrawDebugCapsule(GetWorld(), detectionStartLocation + m_DetectionCapsuleHalfLength * selfPawn->GetActorForwardVector(), m_DetectionCapsuleHalfLength, m_DetectionCapsuleRadius, selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat(), FColor::Blue);
    if (detectionHit.bBlockingHit && detectionHit.GetComponent()->GetCollisionObjectType() == COLLISION_COLLECTIBLE) {
        if (state != ASDTAIState::ChasingCollectible && m_CanChangeBehavior) {
            AIStateInterrupted();
            state = ASDTAIState::ChasingCollectible;
        }
    }
}

void ASDTAIController::GetHightestPriorityDetectionHit(const TArray<FHitResult>& hits, FHitResult& outDetectionHit)
{
    for (const FHitResult& hit : hits)
    {
        if (UPrimitiveComponent* component = hit.GetComponent())
        {
            if (component->GetCollisionObjectType() == COLLISION_PLAYER)
            {
                //we can't get more important than the player
                outDetectionHit = hit;
                return;
            }
            else if (component->GetCollisionObjectType() == COLLISION_COLLECTIBLE)
            {
                outDetectionHit = hit;
            }
        }
    }
}

void ASDTAIController::AIStateInterrupted()
{
    StopMovement();
    m_ReachedTarget = true;
}

void ASDTAIController::ShowPathToClosestCollectible() {

    TArray<AActor*> Collectibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), Collectibles);

    AActor* ClosestCollectible = nullptr;
    float MinDistance = MAX_FLT;
    for (AActor* Collectible : Collectibles)
    {
        if (Collectible)
        {
            float Distance = FVector::Dist(Collectible->GetActorLocation(), GetPawn()->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestCollectible = Collectible;
            }
        }
    }

    if (ClosestCollectible)
    {

        FindNavPathWithBestCost(GetPawn(), ClosestCollectible->GetActorLocation());
    }
}


UNavigationPath* ASDTAIController::GetPath(FVector EndLocation) {
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    
    TSubclassOf<UNavArea> NavAreaClass = UNavArea_Obstacle::StaticClass();
    NavAreaClass.GetDefaultObject()->DefaultCost = 100.f;

    FVector myLocation = GetPawn()->GetActorLocation();
    return NavSys->FindPathToLocationSynchronously(GetWorld(), myLocation, EndLocation);
}

void ASDTAIController::FindNavPathWithBestCost(AActor* StartActor, FVector EndLocation)
{
    UNavigationPath* NavPath = GetPath(EndLocation);


    // Check if the path was found successfully.
    if (NavPath == nullptr || !NavPath->IsValid())
    {
        return;
    }

    // Display the path by drawing lines between the path points.
    for (int32 i = 0; i < NavPath->PathPoints.Num() - 1; i++)
    {
        FVector Start = NavPath->PathPoints[i];
        FVector End = NavPath->PathPoints[i + 1];
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 3.0f);
    }
}




