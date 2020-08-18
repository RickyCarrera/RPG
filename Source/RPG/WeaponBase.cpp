// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(RootComp);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComp);

	CombatCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Collision Box"));
	CombatCollisionBox->SetupAttachment(RootComp);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	WeaponMesh->SetupAttachment(RootComp);


}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnOverlapEnd);

	CombatCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::CombatOverlapBegin);
	CombatCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::CombatOverlapEnd);
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Attack();
}

/*
void AWeaponBase::Attack()
{
	AController* OwnerController = GetOwningController();
	if (!OwnerController)
	{
		return;
	}

	FVector StartLocation = WeaponMesh->GetSocketLocation("BaseSocket");
	FVector EndLocation = WeaponMesh->GetSocketLocation("TipSocket");
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	bool TraceSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	if (TraceSuccess)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, Damage, OwnerController, this, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Something was hit!"));
		}
		DrawDebugSphere(GetWorld(), EndLocation, 10.f, 10, FColor::Red, false, -1.f, 1, 5.f);
	}
}
*/

void AWeaponBase::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingWeapon(this);
		}
	}
}

void AWeaponBase::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingWeapon(nullptr);
		}
	}
}

void AWeaponBase::CombatOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AController * OwningPawn = GetOwningController();
	ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
	if (OtherActor!=Main)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, OwningPawn, this, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("Something was hit!"));
	}
}

void AWeaponBase::CombatOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}


void AWeaponBase::Equip(ACharacterBase * Character)
{
	if (Character)
	{
		GetOwningController();
		//WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		WeaponMesh->SetSimulatePhysics(false);
		// Attach Weapon to the socket on the character.
		const USkeletalMeshSocket* SocketToAttach = Character->GetMesh()->GetSocketByName("WeaponSocket");
		if (SocketToAttach)
		{
			SocketToAttach->AttachActor(this, Character->GetMesh());
			Character->SetEquippedWeapon(this);
			Character->SetActiveOverlappingWeapon(nullptr);
		}
	}
}

AController *AWeaponBase::GetOwningController() const
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!OwningPawn)
	{
		return nullptr;
	}
	return OwningPawn->GetController();
}