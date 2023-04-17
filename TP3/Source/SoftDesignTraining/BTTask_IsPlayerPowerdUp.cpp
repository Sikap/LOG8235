// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsPlayerPowerdUp.h"
#include "SDTUtils.h"
EBTNodeResult::Type UBTTask_IsPlayerPowerdUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (SDTUtils::IsPlayerPoweredUp(GetWorld())) {
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}