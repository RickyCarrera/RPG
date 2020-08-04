// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Stamina = MaxStamina;

	// attachs to weapon to the socket when game starts.
	Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);
	
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Axis Bindings Keyboard
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	// Axis Binds Controller
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpRate);
	PlayerInputComponent->BindAxis("LookRightRate", this, &ACharacterBase::LookRightRate);

	// Action Binding
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::Attack);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &ACharacterBase::EquippedPressed);

}

void ACharacterBase::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ACharacterBase::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	// Using GetDeltaSeconds will make the Pitch (verticle axis) input frame rate independent.
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookRightRate(float AxisValue)
{
	// Using GetDeltaSeconds will make the Yaw (horizontal axis) input frame rate independent.
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

float ACharacterBase::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float ACharacterBase::GetStaminaPercent() const
{
	return Stamina / MaxStamina;
}

void ACharacterBase::SetEquippedWeapon(AWeaponBase * WeaponToSet)
{
	if (Weapon)
	{
		Weapon->Destroy(); // if a weapon is already equipped destroy it and replace with new weapon.
	}
	Weapon = WeaponToSet;
}

void ACharacterBase::Attack()
{
	if (Weapon)
	{
		PlayAnimMontage(AttackMontage);
		Weapon->Attack();
	}
}

void ACharacterBase::EquippedPressed()
{
	if (ActiveOverlappingWeapon)
	{
		AWeaponBase* weapon = Cast<AWeaponBase>(ActiveOverlappingWeapon);
		if (weapon)
		{
			weapon->Equip(this);
			SetActiveOverlappingWeapon(nullptr);
		}
	}
}


