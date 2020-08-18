// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ACharacterBase;
class UBoxComponent;
class USkeletalMeshComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class RPG_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	/* Handles the line tracing of our weapon */
	//void Attack();
	/* Triggers equip event when character overlaps with the weapon */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/* Trigger events after the character is no longer overlapping with the weapon */
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void Equip(ACharacterBase* Character);



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

	/* Collision that will trigger the OnOverlapBegin and OnOverlapEnd */
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	USphereComponent* CollisionSphere;

	/* Collision that will trigger damage to be applied */
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CombatCollisionBox;

	/* Default damage amount, adjustable */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.0f;

private:

	AController * GetOwningController() const;

};
