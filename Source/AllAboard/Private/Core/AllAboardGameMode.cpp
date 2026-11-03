// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AllAboardGameMode.h"

#include "Character/BaseCharacter.h"

AAllAboardGameMode::AAllAboardGameMode()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
}