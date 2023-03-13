// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTBaseAIController.h"
#include "SDTAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ASDTAIState : uint8 {
    Attacking UMETA(DisplayName = "Attacking"),
    Fleeing UMETA(DisplayName = "Fleeing"),
    ChasingCollectible UMETA(DisplayName = "ChasingCollectible"),
    Idle UMETA(DisplayName = "Idle")
};

UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public ASDTBaseAIController
{
	GENERATED_BODY()

public:
    ASDTAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    
    UFUNCTION(BlueprintCallable)
    void SetAtJumpSegment(bool b);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleHalfLength = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleRadius = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleForwardStartingOffset = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    UCurveFloat* JumpCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float JumpApexHeight = 300.f;

    //// cm/s
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    //float JumpSpeed = 100.f; 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool AtJumpSegment = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool InAir = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool Landing = false;

    // Added member variable for last known player location
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    FVector lastKnownPlayerLocation;
  
    // Added member variable for AI state
    // Default behaviour is to chase collectibles
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    ASDTAIState state = ASDTAIState::ChasingCollectible;
      
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    float m_FleeRadius = 600;

public:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override; 
    bool MoveToClosestCollectible();
    virtual void BeginPlay() override;
    void AIStateInterrupted();
   
protected:
    void OnMoveToTarget();
    void GetHightestPriorityDetectionHit(const TArray<FHitResult>& hits, FHitResult& outDetectionHit);
    void UpdatePlayerInteraction(float deltaTime);
    bool GoToPlayer();
    bool GoToFleeLocation();
    

private:
   
    
    virtual void GoToBestTarget(float deltaTime) override;
    virtual void ChooseBehavior(float deltaTime) override;
    virtual void ShowNavigationPath() override;

    bool m_CanChangeBehavior = false;
    AActor* collectibleRef = nullptr;
    AActor* fleeLocationRef = nullptr;
    FVector oldLastPlayerLocation = FVector(-1,-1,-1);
};