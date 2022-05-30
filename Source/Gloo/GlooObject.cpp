// Fill out your copyright notice in the Description page of Project Settings.


#include "GlooObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AGlooObject::AGlooObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetBoxExtent(FVector(28));
	BoxCollision->SetSimulatePhysics(true);
	BoxCollision->SetMassOverrideInKg(NAME_None, 10.f, true);
	RootComponent = BoxCollision;

	// Players can walk on it
	BoxCollision->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Increase, 50.f));
	BoxCollision->CanCharacterStepUpOn = ECB_Yes;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(BoxCollision);

}





