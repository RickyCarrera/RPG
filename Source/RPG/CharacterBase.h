// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class AWeaponBase;
class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RPG_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/* Handles the forward and back movement  */
	void MoveForward(float AxisValue);
	/* Handles the right and left movement */
	void MoveRight(float AxisValue);
	/* Handles the looking up and down using an analog controller */
	void LookUpRate(float AxisValue);
	/* Handles the looking right and left using an analog controller */
	void LookRightRate(float AxisValue);
	/* returns the change in health as a percent */
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	/* returns the change in stamina as a percent */
	UFUNCTION(BlueprintPure)
	float GetStaminaPercent() const;
	/* Calls the attack function from weapon class and plays animation montage */
	void Attack();
	/* Will equipped the weapon we are overlapping with when mapped key is pressed */
	void EquippedPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetEquippedWeapon(AWeaponBase* WeaponToSet);

	FORCEINLINE void SetActiveOverlappingWeapon(AWeaponBase * WeaponBase) { ActiveOverlappingWeapon = WeaponBase; }

protected:
	/* Root component for the follow camera */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	/* the camera that the player will view the game through */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* FollowCamera;
	/* Adjustable rotation rate for analog controller for the character looking vertically and horizontally */
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;
	/* Default Character Max health adjustable */
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	/* will be used to update the default (max) health when taking damage */
	float Health;
	/* Default Character Max Stamina adjustable*/
	UPROPERTY(EditAnywhere)
	float MaxStamina = 100;
	/* will be used to update the default (max) stamina when performing actions */
	float Stamina;
	/* Plays the attack montage */
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;
	/* Reference to our Weapon */
	UPROPERTY(BlueprintReadWrite)
	AWeaponBase* Weapon;
	/* The weapon we will overlap with in order to equipped it */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase * ActiveOverlappingWeapon;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> WeaponClass;
};
