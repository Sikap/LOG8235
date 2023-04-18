// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentTimeLogger.h"
#include "BehaviourTreeAiController.h"
#include <Kismet/GameplayStatics.h>
#include "SoftDesignTrainingCharacter.h"

// Sets default values
AAgentTimeLogger::AAgentTimeLogger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAgentTimeLogger::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> foundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoftDesignTrainingCharacter::StaticClass(), foundCharacters);
	int i = 0;
	for (AActor* a : foundCharacters) {
		ABehaviourTreeAiController* controller = Cast<ABehaviourTreeAiController>(Cast<ASoftDesignTrainingCharacter>(a)->GetController());
		if (controller) {
			controller->m_debugKey = i;
			controller->m_loggerRef = this;
			AgentMap.Add(i, FString(TEXT("  %i    |                          |                          |                          |                          |"), i));
			i++;
		}
	}
	
}

// Called every frame
void AAgentTimeLogger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString base = FString(	TEXT("AgentID |     Last total time      |  LastPlayerDetectionTime |   LastFleeLocationTime   |    LastCollectibleTime   |"));
							
	for (const TPair<int, FString>& pair : AgentMap) { 
		base += "\n" +	pair.Value;
	}
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, base);
}

