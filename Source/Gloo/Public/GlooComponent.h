// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GlooComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLOO_API UGlooComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGlooComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int GlooThreshold;

	int GlooCount;

public:	

	void TakeGlooDamage();
};
