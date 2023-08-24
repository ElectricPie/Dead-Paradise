// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectableComponent.generated.h"

class UUnitPathfinding;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetTargetSignature, FVector, TargetPosition);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADPARADISE_API USelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USelectableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedSignature OnSelectedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnSetTargetSignature OnSetTargetEvent;

	void Select() const;
	void SetTarget(const FVector& TargetPosition) const;
};
