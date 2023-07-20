// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveFinishedSignature, bool /* bReachedTarget */);

class UCapsuleComponent;

UCLASS(meta=(ScriptName="RtsUnit"))
class DEADPARADISE_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Moves the target the given point over the given duration. Calls the callback function when moving is done
	 * @param TargetPosition The position to move to
	 * @param Duration How long it will take to move to the TargetPosition
	 * @param CallbackObject Object for callback bind
	 * @param CallbackFunction Callback function address
	 */
	void MoveToPoint(const FVector& TargetPosition, float Duration, UObject* CallbackObject, FName CallbackFunction);
	
private:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	FOnMoveFinishedSignature OnMoveFinishedDelegate;
	FTimerHandle MoveTimerHandle;

	UFUNCTION()
	void LerpToPoint(FVector& StartLocation, FVector& EndLocation, float StartTime, float EndTime);
};
