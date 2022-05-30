// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlooProjectile.h"

#include "GlooObject.h"
#include "Components/DecalComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Public/GlooComponent.h"

AGlooProjectile::AGlooProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.5f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetGenerateOverlapEvents(false);
	CollisionComp->OnComponentHit.AddDynamic(this, &AGlooProjectile::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGlooProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	// if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	// {
	// 	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	//
	// 	Destroy();
	// }
	
	if (GlooClass && !OtherActor->IsA(GlooClass))
	{
		if (Cast<ACharacter>(OtherActor))
		{
			// Apply Gloo Effect to Character
		}
		else
		{
			CreateGlooObject(OtherActor, OtherComp, Hit);
			
			if (Decal != nullptr)
			{
				SpawnGlooDecal(Hit);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Decal is Null...."))
			}
			
			const ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
			if (OtherCharacter)
			{
				ApplyGlooEffectToCharacter(OtherCharacter);
			}
		}
	}

	Destroy();
}

void AGlooProjectile::CreateGlooObject(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
	const FVector SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * CollisionOffset);

	AActor* Gloo = GetWorld()->SpawnActor<AActor>(
		GlooClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (Gloo && OtherActor && OtherComp->IsSimulatingPhysics())
	{
		const FAttachmentTransformRules AttachRules =
			FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
		
		Gloo->AttachToActor(OtherActor, AttachRules);

		OtherComp->AddImpulseAtLocation(-GetVelocity(), Hit.Location);
	}
	
}

void AGlooProjectile::SpawnGlooDecal(const FHitResult& Hit) const
{
	const FVector Location = Hit.ImpactPoint;
	FRotator RandomRollRotation = Hit.ImpactNormal.Rotation();
	RandomRollRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

	const float RandomDecalSize = DecalSize * FMath::FRandRange(0.75f, 1.15f);
	const FVector DecalBounds = FVector(DecalDepth, RandomDecalSize, RandomDecalSize);

	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached
	(
		Decal, DecalBounds, Hit.GetComponent(), NAME_None, Location,
		RandomRollRotation, EAttachLocation::KeepWorldPosition
	);
	
	DecalComponent->SetFadeIn(DecalStartDelay, DecalFadeDuration);
}

void AGlooProjectile::ApplyGlooEffectToCharacter(const ACharacter* OtherCharacter) const
{
	if (OtherCharacter == nullptr)
	{
		return;
	}

	
	UGlooComponent* StatusComponent = Cast<UGlooComponent>(
		OtherCharacter->GetComponentByClass(UGlooComponent::StaticClass())
	);

	if (StatusComponent)
	{
		StatusComponent->TakeGlooDamage();
	}
}

