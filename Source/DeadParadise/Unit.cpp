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

void AUnit::MoveToPoint(const FVector& TargetPoint, float Duration)
{
	FVector StartLocation = GetActorLocation();

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float EndTime = CurrentTime + Duration;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("LerpToPoint"), StartLocation, TargetPoint, CurrentTime, EndTime);
	
	GetWorldTimerManager().SetTimer(MoveTimerHandle, TimerDelegate, FApp::GetDeltaTime(), true);
}

void AUnit::LerpToPoint(FVector& StartLocation, FVector& EndLocation, float StartTime, float EndTime)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float Alpha = FMath::Clamp((CurrentTime - StartTime) / (EndTime / StartTime), 0.f, 1.f);

	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	SetActorLocation(NewLocation);

	if (Alpha >= 1.f)
	{
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	}
}

