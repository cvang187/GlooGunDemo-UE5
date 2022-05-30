// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlooProjectile.generated.h"

class AGlooObject;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AGlooProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	//UStaticMeshComponent* CollisionComp;
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AGlooProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AGlooObject> GlooClass;

private:

	void CreateGlooObject(AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const FHitResult& Hit) const;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float CollisionOffset = 45;

	UFUNCTION(BlueprintCallable)
	void SpawnGlooDecal(const FHitResult& Hit) const;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	UMaterial* Decal;
	
	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalDepth;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalFadeDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalStartDelay = .25f;

	void ApplyGlooEffectToCharacter(const ACharacter* OtherCharacter) const;

	
protected:
	

};

