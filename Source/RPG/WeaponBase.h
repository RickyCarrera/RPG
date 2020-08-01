// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

UCLASS()
class RPG_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/* Will be the parent to which everything will be attach to */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

};
