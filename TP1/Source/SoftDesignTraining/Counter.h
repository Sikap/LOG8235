// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Counter.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API ACounter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACounter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int m_Collected = 0;
	int m_Deaths = 0;
	FTimerHandle m_TimerHand;
public:	

	UFUNCTION()
		void OnTimerEnd();

	UFUNCTION()
		void IncrementCollect();
	
	UFUNCTION()
		void IncrementDeath();

	// We cannot count less than 1 sec
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = STATS, meta = (ClampMin = 1))
	int m_TimeToCount = 1;
};
