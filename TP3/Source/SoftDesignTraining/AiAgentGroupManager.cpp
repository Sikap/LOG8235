#include "AiAgentGroupManager.h"

AAiAgentGroupManager* AAiAgentGroupManager::m_Instance;

AAiAgentGroupManager::AAiAgentGroupManager()
{
}

AAiAgentGroupManager::~AAiAgentGroupManager()
{
    m_registeredAgents.Empty();
    AgentAssignedPositions.Empty();
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

bool AAiAgentGroupManager::RegisterAIAgent(ABehaviourTreeAiController* aiAgent)
{
    if (!m_registeredAgents.Contains(aiAgent))
    {
        m_registeredAgents.Add(aiAgent);
        return true;
    }
    return false;
}

bool AAiAgentGroupManager::HasAIAgent(ABehaviourTreeAiController* aiAgent) {

   return m_registeredAgents.Contains(aiAgent);
}

bool AAiAgentGroupManager::UnregisterAIAgent(ABehaviourTreeAiController* aiAgent){

    if (m_registeredAgents.Contains(aiAgent))
    {
        m_registeredAgents.Remove(aiAgent);
        return true;
    }
    return false;
}


void AAiAgentGroupManager::AssignGroupPositions(UWorld* world)
{
    int32 numAgents = m_registeredAgents.Num();
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(world, 0);
    if (playerCharacter == nullptr || numAgents <= 0) { return;  }
    FVector playerLocation = playerCharacter->GetActorLocation();

    float angleIncrement = 360.0f / numAgents;
    AgentAssignedPositions.Empty();
    for (int32 i = 0; i < numAgents; ++i)
    {
        float angle = i * angleIncrement;
        FVector offset = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), 0.0f) * 300.0f;
        AgentAssignedPositions.Add(m_registeredAgents[i], playerLocation + offset);
    }
}

FVector AAiAgentGroupManager::GetAgentAssignedPosition(ABehaviourTreeAiController* agent)
{
    FVector* assignedPosition = AgentAssignedPositions.Find(agent);
    if (assignedPosition)
    {
        return *assignedPosition;
    }

    return FVector::ZeroVector;
}