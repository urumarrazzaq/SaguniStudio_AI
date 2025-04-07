// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaguniStudio_AIGameMode.h"
#include "SaguniStudio_AICharacter.h"
#include "UObject/ConstructorHelpers.h"

ASaguniStudio_AIGameMode::ASaguniStudio_AIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_Character.BP_Character_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
