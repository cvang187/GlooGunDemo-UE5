// Fill out your copyright notice in the Description page of Project Settings.


#include "Gloo/Public/GlooComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UGlooComponent::UGlooComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGlooComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
	
	GlooCount = 0;
}


void UGlooComponent::TakeGlooDamage()
{
	if (GlooCount >= GlooThreshold) return;

	GlooCount++;

	if (MovementComponent)
	{
		const float CurrentMaxWalkSpeed = MovementComponent->MaxWalkSpeed;

		MovementComponent->MaxWalkSpeed  = CurrentMaxWalkSpeed * 0.9f;

		if (GlooCount >= GlooThreshold)
		{
			MovementComponent->DisableMovement();
		}
	}
}

