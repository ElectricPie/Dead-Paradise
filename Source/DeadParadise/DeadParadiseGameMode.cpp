// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadParadiseGameMode.h"
#include "DeadParadisePlayerController.h"
#include "DeadParadiseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeadParadiseGameMode::ADeadParadiseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADeadParadisePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}