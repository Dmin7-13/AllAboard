// Fill out your copyright notice in the Description page of Project Settings.


#include "AllAboard/Public/Core/AllAboardGameMode.h"

#include "AllAboard/Public/Character/BaseCharacter.h"

AAllAboardGameMode::AAllAboardGameMode()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
}