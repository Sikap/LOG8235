// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAnimNotify_JumpStart.h"
#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "SoftDesignTrainingCharacter.h"
#include "BehaviourTreeAiController.h"
void USDTAnimNotify_JumpStart::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    if (AActor* owner = MeshComp->GetOwner())
    {
        if (ASoftDesignTrainingCharacter* character = Cast<ASoftDesignTrainingCharacter>(owner))
        {
            if (ASDTAIController* controller = Cast<ASDTAIController>(character->GetController()))
            {
                controller->InAir = true;
            }

            if (ABehaviourTreeAiController* controller2 = Cast<ABehaviourTreeAiController>(character->GetController()))
            {
                controller2->InAir = true;
            }
        }
    }
}
