// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Engine/GameEngine.h"

// Sets default values
AFlyingPawn::AFlyingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	RootMesh->SetVisibility(false);
	RootComponent = RootMesh;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	PlaneMesh->SetupAttachment(RootComponent);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = true;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->bEnableCameraRotationLag = true;	// Do not allow camera to lag
	SpringArm->CameraRotationLagSpeed = 10.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;


	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	MaxHorizontalTurnSpeed = 100.f;
	CurrentHorizontalSpeed = 0.f;

	MaxVerticalAngle = 30.f;
	CurrentVerticalAngle = 0.f;

	// Inverse of how long it takes to get to max turning speed. Total time = 1 second / HorizontalTurnAcceloration
	HorizontalTurnAcceloration = 10.f;
	VerticalTurAcceloration = 2.5f;
}

// Called when the game starts or when spawned
void AFlyingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingPawn::Tick(float DeltaTime)
{
	const FVector LocalMove = FVector(1000.f * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);
}

// Called to bind functionality to input
void AFlyingPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("MoveUp", this, &AFlyingPawn::VerticalInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFlyingPawn::HoriontalInput);
}

void AFlyingPawn::VerticalInput(float Input) {
	FRotator CurrentRotation = GetActorRotation();
	if (Input != 0) {
		CurrentRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, -1 * Input * MaxVerticalAngle, GetWorld()->GetDeltaSeconds() * VerticalTurAcceloration);
	}
	else {
		CurrentRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, 0.f, GetWorld()->GetDeltaSeconds() * VerticalTurAcceloration);
	}

	// Force object to not roll. Handled by the mesh instead
	CurrentRotation.Roll = 0;
	SetActorRotation(CurrentRotation);
}

void AFlyingPawn::HoriontalInput(float Input) {
	if (Input != 0) {
		CurrentHorizontalSpeed = FMath::Lerp(CurrentHorizontalSpeed, Input * MaxHorizontalTurnSpeed, GetWorld()->GetDeltaSeconds() * HorizontalTurnAcceloration);
	}
	else {
		CurrentHorizontalSpeed = FMath::Lerp(CurrentHorizontalSpeed, 0.f, GetWorld()->GetDeltaSeconds() * 10);
	}

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, CurrentHorizontalSpeed * GetWorld()->GetDeltaSeconds(), 0);

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}