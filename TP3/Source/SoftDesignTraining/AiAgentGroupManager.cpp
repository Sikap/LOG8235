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

void AAiAgentGroupManager::RegisterAIAgent(ASDTAIController* aiAgent)
{
    m_registeredAgents.Add(aiAgent);
}

void AAiAgentGroupManager::UnregisterAIAgent(ASDTAIController* aiAgent){

    m_registeredAgents.Remove(aiAgent);
}

