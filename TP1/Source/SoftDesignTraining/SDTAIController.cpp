// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"

void ASDTAIController::Tick(float deltaTime)
{
	m_accel = FVector(-50, -50, 0);
	m_speed = m_accel * deltaTime;
	GetPawn()->FaceRotation(m_speed.Rotation());
	GetPawn()->AddMovementInput(m_speed, 1);
	
}




