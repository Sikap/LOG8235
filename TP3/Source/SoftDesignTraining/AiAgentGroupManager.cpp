#include "AiAgentGroupManager.h"

AAiAgentGroupManager* AAiAgentGroupManager::m_Instance;

AAiAgentGroupManager::AAiAgentGroupManager()
{
}

AAiAgentGroupManager* AAiAgentGroupManager::GetInstance()
{
    if (!m_Instance)
    {
        m_Instance = new AAiAgentGroupManager();
    }

    return m_Instance;
}

void AAiAgentGroupManager::Destroy()
{
    delete m_Instance;
    m_Instance = nullptr;
}

bool AAiAgentGroupManager::RegisterAIAgent(ASDTAIController* aiAgent)
{
    if (!m_registeredAgents.Contains(aiAgent))
    {
        m_registeredAgents.Add(aiAgent);
        return true;
    }
    return false;
}

bool AAiAgentGroupManager::UnregisterAIAgent(ASDTAIController* aiAgent){

    if (m_registeredAgents.Contains(aiAgent))
    {
        m_registeredAgents.Remove(aiAgent);
        return true;
    }
    return false;
}

void AAiAgentGroupManager::AssignGroupPositions()
{
    int32 numAgents = m_registeredAgents.Num();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter || numAgents <= 0) return;

    FVector playerLocation = playerCharacter->GetActorLocation();

    float angleIncrement = 360.0f / numAgents;
    for (int32 i = 0; i < numAgents; ++i)
    {
        float angle = i * angleIncrement;
        FVector offset = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), 0.0f) * 300.0f;
        AgentAssignedPositions.Add(m_registeredAgents[i], playerLocation + offset);
    }
}

void AAiAgentGroupManager::AssignGroupPositions()
{
    int32 numAgents = m_registeredAgents.Num();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter || numAgents <= 0) return;

    FVector playerLocation = playerCharacter->GetActorLocation();

    float angleIncrement = 360.0f / numAgents;
    for (int32 i = 0; i < numAgents; ++i)
    {
        float angle = i * angleIncrement;
        FVector offset = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), 0.0f) * 300.0f;
        AgentAssignedPositions.Add(m_registeredAgents[i], playerLocation + offset);
    }
}