// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TryGetCollectibleLoc.h"
#include "SDTCollectible.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviourTreeAiController.h"
#include "AiAgentGroupManager.h"
#include "Engine/GameEngine.h"

UBTService_TryGetCollectibleLoc::UBTService_TryGetCollectibleLoc() {
	bCreateNodeInstance = true;
}

void UBTService_TryGetCollectibleLoc::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    float closestSqrCollectibleDistance = 18446744073709551610.f;
    AActor* closestCollectible = nullptr;

    TArray<AActor*> foundCollectibles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), foundCollectibles);
    ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner());

   
    for (int i=0; i < foundCollectibles.Num(); i++)
    {
        //int index = FMath::RandRange(0, foundCollectibles.Num() - 1);

        ASDTCollectible* collectibleActor = Cast<ASDTCollectible>(foundCollectibles[i]);
        float dist = FVector::Dist(foundCollectibles[i]->GetActorLocation(), aiController->GetPawn()->GetActorLocation());
        if (!collectibleActor->IsOnCooldown() && dist < closestSqrCollectibleDistance)
        {
            closestCollectible = foundCollectibles[i];
            closestSqrCollectibleDistance = dist;
        }
      
    }

    if (closestCollectible) {

            aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetCollectibleLocationKeyID(), closestCollectible->GetActorLocation());  
            AAiAgentGroupManager* aiAgentGroupManager = AAiAgentGroupManager::GetInstance();
            aiAgentGroupManager->UnregisterAIAgent(aiController);
    }
    else {
        aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetCollectibleLocationKeyID(), aiController->m_invalidLocation);
    }
     
}