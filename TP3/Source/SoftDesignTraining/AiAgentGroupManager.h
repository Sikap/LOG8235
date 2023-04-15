#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "SDTAIController.h"

class SOFTDESIGNTRAINING_API AAiAgentGroupManager 
{
public:
	// Sets default values for this empty's properties
	
    AAiAgentGroupManager();
    ~AAiAgentGroupManager();
	
	static AAiAgentGroupManager* GetInstance();
	static void Destroy();

	bool RegisterAIAgent(ASDTAIController* aiAgent);
	bool UnregisterAIAgent(ASDTAIController* aiAgent);
	
	void AssignGroupPositions(UWorld* world);
    FVector GetAgentAssignedPosition(ASDTAIController* agent);

public:
	//SINGLETON
	static AAiAgentGroupManager* m_Instance;

	TArray<ASDTAIController*> m_registeredAgents;
	TMap<ASDTAIController*, FVector> AgentAssignedPositions;
};

