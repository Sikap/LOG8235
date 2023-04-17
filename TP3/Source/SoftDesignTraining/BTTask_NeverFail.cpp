// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NeverFail.h"

#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"


EBTNodeResult::Type UBTTask_NeverFail::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{     
    return EBTNodeResult::Succeeded;
}