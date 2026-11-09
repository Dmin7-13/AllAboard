// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UPhysicsHandleComponent;
class UPrimitiveComponent;

UCLASS()
class ALLABOARD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float HoldDistance = 50.f;
	
	void Interact();
	
	void Move(const FInputActionValue& Value);
	
	void OnJumpStart(const FInputActionValue& Value);
	void OnJumpCancelled(const FInputActionValue& Value);
	
	FRotator HeldRotation;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
