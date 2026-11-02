// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "AllAboardPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALLABOARD_API AAllAboardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	virtual void SetupInputComponent() override;
};
