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
    if (FMath::IsWithin(FVector::Distance(oldLastPlayerLocation, lastKnownPlayerLocation), -1, 25)) { return false; }
    oldLastPlayerLocation = lastKnownPlayerLocation;

    return MoveToLocation(lastKnownPlayerLocation) == EPathFollowingRequestResult::RequestSuccessful;
}

bool ASDTAIController::GoToFleeLocation()
{
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return false;

    if (fleeLocationRef != nullptr)
    {
        // If we are already running towards a flee location we do not need to find a new one to go to
        return false;
    }

    TArray<AActor*> fleeLocations;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTFleeLocation::StaticClass(), fleeLocations);

    if (fleeLocations.Num() <= 0)
        return false;

    FVector selfLocation = GetPawn()->GetActorLocation();

    float minPathLength = INT_MAX;


    // Find closest flee location that is reachable and in the right orientation
    for (AActor* fleeLocation : fleeLocations) {


        FVector dir = selfLocation - playerCharacter->GetActorLocation();
        FVector dirToFlee = fleeLocation->GetActorLocation() - selfLocation;

        auto value = FVector::DotProduct(dirToFlee.GetSafeNormal(), dir.GetSafeNormal());

        auto angle = FMath::Acos(value);
        auto isVisible = FMath::Abs(angle) <= PI;
 

        if (isVisible){
            UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
            UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetPawn()->GetActorLocation(), fleeLocation->GetActorLocation());

            float currentPathLength = path->GetPathLength();

            if (currentPathLength <= minPathLength) {
                minPathLength = currentPathLength;
                fleeLocationRef = fleeLocation;
            }
        }
    }

    if (fleeLocationRef == nullptr) {
        fleeLocationRef = fleeLocations[0];
    }

    // Check if the flee location is valid and reachable todo 
    return MoveToLocation(fleeLocationRef->GetActorLocation()) == EPathFollowingRequestResult::RequestSuccessful;
    
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

    if (isMoveSuccessful) {
        OnMoveToTarget();
    }
    if (m_ReachedTarget) {
        state = ASDTAIState::ChasingCollectible;
        fleeLocationRef = nullptr;
        oldLastPlayerLocation = FVector(-1, -1, -1);
        collectibleRef = nullptr;
    }
}

// Collectible
bool ASDTAIController::MoveToClosestCollectible() {


    if (!(collectibleRef == nullptr || Cast<ASDTCollectible>(collectibleRef)->IsOnCooldown())) 
    {
        // If we are already running towards an active collectible we do not need to find a new one to go to
        return false;
    }

    TArray<AActor*> collectibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), collectibles);

    float minPathLength = INT_MAX;

    
    for (AActor* collectible : collectibles) {

        if (Cast<ASDTCollectible>(collectible)->IsOnCooldown()) continue;

        UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
        UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetPawn()->GetActorLocation(), collectible->GetActorLocation());

        float currentPathLength = path->GetPathLength();

        if (currentPathLength <= minPathLength) {
            minPathLength = currentPathLength;
            collectibleRef = collectible;
        }
    }
    
    return collectibleRef != nullptr && MoveToLocation(collectibleRef->GetActorLocation()) == EPathFollowingRequestResult::RequestSuccessful;
}

void ASDTAIController::OnMoveToTarget()
{
  //  UE_LOG(LogTemp, Warning, TEXT(" SAETTING CURRENT STATUS: FALSE"));
    m_ReachedTarget = false;
}

void ASDTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    m_ReachedTarget = true;
  //  UE_LOG(LogTemp, Warning, TEXT(" SAETTING CURRENT STATUS: TRUE"));
}

void ASDTAIController::ShowNavigationPath()
{
    FNavPathSharedPtr Path = GetPathFollowingComponent()->GetPath();
    // Check if the path was found successfully.
    if (Path == nullptr || !Path->IsValid())
    {
        return;
    }

    //Show current navigation path DrawDebugLine and DrawDebugSphere
    // Display the path by drawing lines between the path points.

    TArray<FNavPathPoint> PathPoints = Path->GetPathPoints();
    for (int32 i = 0; i < PathPoints.Num() - 1; i++)
    {

        FVector Start = PathPoints[i];
        FVector End = PathPoints[i + 1];
      
        DrawDebugSphere(GetWorld(), End, 50, 18, FColor::Blue, false, -1.0f, 0, 2.0f);
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 3.0f);
    }
}

void ASDTAIController::ChooseBehavior(float deltaTime)
{
    // Regardless of current state we must check player interaction
    UpdatePlayerInteraction(deltaTime);

    // Will create the move to querys depending on state and if it needs to be updated
    GoToBestTarget(deltaTime);

}

void ASDTAIController::UpdatePlayerInteraction(float deltaTime)
{
    // check if player is powered up
    bool isPlayerPoweredUp = SDTUtils::IsPlayerPoweredUp(GetWorld());

    // Finish jump before updating AI state 
   
    if (AtJumpSegment) {
        return;
    }

    FVector detectionEndLocation = GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * m_DetectionCapsuleHalfLength * 2;
    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, GetPawn()->GetActorLocation(), detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(m_DetectionCapsuleRadius));

    FHitResult detectionHit;
    GetHightestPriorityDetectionHit(allDetectionHits, detectionHit);

    // set the AI state based on player detection
    if (detectionHit.bBlockingHit && detectionHit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
    {
        // TODO : Check if player is visible
        if (!SDTUtils::Raycast(GetWorld(), GetPawn()->GetActorLocation(), detectionHit.GetActor()->GetActorLocation())) {

            // By default when we see a player we want to attack it
            state = ASDTAIState::Attacking;

            // update position
            lastKnownPlayerLocation = detectionHit.GetActor()->GetActorLocation();
        }
        
        // Player is powered and in the detection zone (but not sight) so we flee instead
        if (isPlayerPoweredUp) {
            state = ASDTAIState::Fleeing;
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
                // We Are looking for player collisions
                outDetectionHit = hit;
                return;
            }
        }
    }
}

void ASDTAIController::AIStateInterrupted() {
    
    //UE_LOG(LogTemp, Warning, TEXT(" SAETTING CURRENT STATUS: TRUE"));
    m_ReachedTarget = true;
    state = ASDTAIState::ChasingCollectible;
    fleeLocationRef = nullptr;
    oldLastPlayerLocation = FVector(-1, -1, -1);
    collectibleRef = nullptr;
    
}
