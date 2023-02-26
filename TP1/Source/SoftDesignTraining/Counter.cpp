// Fill out your copyright notice in the Description page of Project Settings.


#include "Counter.h"
#include "SDTCollectible.h"
#include "Kismet/GameplayStatics.h"
#include "SoftDesignTrainingCharacter.h"
// Sets default values
ACounter::ACounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACounter::BeginPlay()
{
	// We must set the max fps to 60 because it is the
	// value we used to calibrate all the detections

	FString MyCommandString = "t.MaxFPS 60";
	GetWorld()->Exec(GetWorld(), *MyCommandString);

	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors) {

		ASDTCollectible* a = Cast<ASDTCollectible>(actor);
		a->OnSomeEvent.AddDynamic(this, &ACounter::IncrementCollect);
	}

	TArray<AActor*> FoundAgents;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoftDesignTrainingCharacter::StaticClass(), FoundActors);

	for (AActor* actor : FoundAgents) {

		ASoftDesignTrainingCharacter* a = Cast<ASoftDesignTrainingCharacter>(actor);
		a->deathEvent.AddDynamic(this, &ACounter::IncrementDeath);
	}
	
	

	GetWorld()->GetTimerManager().SetTimer(m_TimerHand, this, &ACounter::OnTimerEnd, m_TimeToCount, false);
}

void ACounter::IncrementCollect()
{
	m_Collected += 1;
	UE_LOG(LogTemp, Warning, TEXT("COLLECTED, %i"), m_Collected);
}
void ACounter::IncrementDeath()
{
	m_Deaths += 1;
	UE_LOG(LogTemp, Warning, TEXT("DEATH, %i"), m_Deaths);

}

void ACounter::OnTimerEnd() {

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors) {

		ASDTCollectible* a = Cast<ASDTCollectible>(actor);
		a->OnSomeEvent.RemoveDynamic(this, &ACounter::IncrementCollect);
	}

	TArray<AActor*> FoundAgents;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoftDesignTrainingCharacter::StaticClass(), FoundActors);

	for (AActor* actor : FoundAgents) {

		ASoftDesignTrainingCharacter* a = Cast<ASoftDesignTrainingCharacter>(actor);
		a->deathEvent.RemoveDynamic(this, &ACounter::IncrementDeath);
	}
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("Resultat de l'agent: pickup-morts,  %i - %i"), m_Collected, m_Deaths));
	UE_LOG(LogTemp, Warning, TEXT("Resultat de l'agent: pickup-morts,  %i - %i"), m_Collected, m_Deaths);

}


