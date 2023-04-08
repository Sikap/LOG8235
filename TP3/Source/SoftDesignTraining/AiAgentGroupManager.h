#pragma once
#include "CoreMinimal.h"
#include "SDTAIController.h"

class SOFTDESIGNTRAINING_API AAiAgentGroupManager 
{
public:
	// Sets default values for this empty's properties
	static AAiAgentGroupManager* GetInstance();
	static void Destroy();

	void RegisterAIAgent(ASDTAIController* aiAgent);
	void UnregisterAIAgent(ASDTAIController* aiAgent);

public:
	//SINGLETON
	AAiAgentGroupManager();
	static AAiAgentGroupManager* m_Instance;

	TArray<ASDTAIController*> m_registeredAgents;
};

