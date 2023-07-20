// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AUnit::MoveToPoint(const FVector& TargetPosition, float Duration, UObject* CallbackObject, FName CallbackFunction)
{
	OnMoveFinishedDelegate.AddUFunction(CallbackObject, CallbackFunction);
	
	FVector StartLocation = GetActorLocation();

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float EndTime = CurrentTime + Duration;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("LerpToPoint"), StartLocation, TargetPosition, CurrentTime, EndTime);
	
	GetWorldTimerManager().SetTimer(MoveTimerHandle, TimerDelegate, FApp::GetDeltaTime(), true);
}


void AUnit::LerpToPoint(const FVector& StartLocation, const FVector& EndLocation, const float StartTime, const float EndTime)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float Alpha = FMath::Clamp((CurrentTime - StartTime) / (EndTime / StartTime), 0.f, 1.f);

	const FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	SetActorLocation(NewLocation);

	// Stop the timer and inform any listens that the unit has stopped moving
	if (Alpha >= 1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit At Point"));

		GetWorldTimerManager().ClearTimer(MoveTimerHandle);

		// Stores the delegates so they can be cleared ready for the next move
		const FOnMoveFinishedSignature OnFinishedDelegate = OnMoveFinishedDelegate;
		OnMoveFinishedDelegate.Clear();
		OnFinishedDelegate.Broadcast(false);
	}
}

