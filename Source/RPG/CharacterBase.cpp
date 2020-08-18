// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	bWasFirstAttackUsed = false;
	bWasSecondAttackUsed = false;
	bWasThirdAttackUsed = false;
	bWasFourthAttackUsed = false;
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
	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::Attack);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &ACharacterBase::EquippedPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::Walk);

	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &ACharacterBase::StartAttack1);
	PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &ACharacterBase::StartAttack2);
	PlayerInputComponent->BindAction("Attack3", IE_Pressed, this, &ACharacterBase::StartAttack3);
	PlayerInputComponent->BindAction("Attack4", IE_Pressed, this, &ACharacterBase::StartAttack4);

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
void ACharacterBase::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterBase::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply); // prevents from taking damage more than amount of Health, no overkill.
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health Left %f"), Health);
	return DamageToApply;
}

void ACharacterBase::StartAttack1()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("FIRST ATTACK"));
		//Weapon->Attack();
		bWasFirstAttackUsed = true;
	}
}
void ACharacterBase::StartAttack2()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("SECOND ATTACK"));
		//Weapon->Attack();
		bWasSecondAttackUsed = true;
	}

}
void ACharacterBase::StartAttack3()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("THIRD ATTACK"));
		//Weapon->Attack();
		bWasThirdAttackUsed = true;
	}
}
void ACharacterBase::StartAttack4()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("FOURTH ATTACK"));
		//Weapon->Attack();
		bWasFourthAttackUsed = true;
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
			Weapon->SetOwner(this);
		}
	}
}



