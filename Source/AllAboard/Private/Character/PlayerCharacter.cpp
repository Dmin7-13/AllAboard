// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->bUsePawnControlRotation = false;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandle->LinearStiffness = 1500.f;
	PhysicsHandle->LinearDamping = 200.f;
	PhysicsHandle->AngularStiffness = 500.f;
	PhysicsHandle->AngularDamping = 100.f;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		
		// Move Action
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		
		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::OnJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::OnJumpCancelled);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Interact()
{
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
		return;
	}
	
	const FVector Start = GetActorLocation();
	const FVector Direction = GetActorRightVector();
	const FVector End = Start + Direction * InteractionDistance;
	
	constexpr float Radius = 45.f;
	
	const FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	TArray<FHitResult> Hits;
	
	if (!GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		Shape,
		CollisionParams
	))
	{
		return;
	}
	
	for (const FHitResult& Hit : Hits)
	{
		UPrimitiveComponent* HitComponent = Hit.GetComponent();
	
		if (!HitComponent || !HitComponent->IsSimulatingPhysics())
		{
			continue;
		}
		
		HeldRotation = HitComponent->GetComponentRotation();
	
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			Hit.ImpactPoint,
			HeldRotation
		);
		
		break;
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!GetController()) return;

	const float MovementDirection = Value.Get<float>();
	
	AddMovementInput(GetActorRightVector(), MovementDirection);
}

void APlayerCharacter::OnJumpStart(const FInputActionValue& Value)
{
	if (!CanJump()) return;
	
	Jump();
}

void APlayerCharacter::OnJumpCancelled(const FInputActionValue& Value)
{
	StopJumping();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const UPrimitiveComponent* HeldComponent = PhysicsHandle->GetGrabbedComponent();
	
	if (!HeldComponent)
	{
		return;
	}
	
	FVector Direction = GetActorRightVector();
	Direction.Y = 5.f;
	Direction.Normalize();
	
	const FVector CurrentLocation = HeldComponent->GetComponentLocation();
	FVector TargetLocation = GetActorLocation() + Direction * HoldDistance;
	
	TargetLocation.Z = CurrentLocation.Z;
	
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, HeldRotation);
}

