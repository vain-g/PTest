// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "PTest.h"
#include "PTestCharacter.h"

//////////////////////////////////////////////////////////////////////////
// PTCharacter

APTestCharacter::APTestCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	WalkSpeed = 300;
	RunSpeed = 750;
	CanRun = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void APTestCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APTestCharacter::FirePress);
	InputComponent->BindAction("Fire", IE_Released, this, &APTestCharacter::FireRelease);

	InputComponent->BindAction("Aim", IE_Pressed, this, &APTestCharacter::AimPress);
	InputComponent->BindAction("Aim", IE_Released, this, &APTestCharacter::AimRelease);

	InputComponent->BindAction("Run", IE_Pressed, this, &APTestCharacter::RunPress);
	InputComponent->BindAction("Run", IE_Released, this, &APTestCharacter::RunRelease);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &APTestCharacter::CrouchPress);
	InputComponent->BindAction("Crouch", IE_Released, this, &APTestCharacter::CroouchRelease);

	InputComponent->BindAxis("MoveForward", this, &APTestCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APTestCharacter::TurnAxis);
	InputComponent->BindAxis("TurnRate", this, &APTestCharacter::TurnAtRate);

	InputComponent->BindAxis("LookUp", this, &APTestCharacter::LookUpAxis);
	InputComponent->BindAxis("LookUpRate", this, &APTestCharacter::LookUpAtRate);

}


void APTestCharacter::Tick(float deltaTime)
{

}



void APTestCharacter::AimPress()
{
	IsIronsight = true;
	CharacterMovement->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	CanRun = false;
}

void APTestCharacter::AimRelease()
{
	IsIronsight = false;
	CharacterMovement->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	CanRun = true;
}


void APTestCharacter::FirePress()
{

}

void APTestCharacter::FireRelease()
{

}


void APTestCharacter::RunPress()
{
	if (CanRun)
	{
		CharacterMovement->MaxWalkSpeed = RunSpeed;
	}
}

void APTestCharacter::RunRelease()
{
	CharacterMovement->MaxWalkSpeed = WalkSpeed;
}

void APTestCharacter::CrouchPress()
{
	//CharacterMovement->crou
}

void APTestCharacter::CroouchRelease()
{

}



void APTestCharacter::TurnAxis(float Rate)
{
	APawn::AddControllerYawInput(Rate);
}

void APTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void APTestCharacter::LookUpAxis(float Rate)
{
	APawn::AddControllerPitchInput(Rate);
}

void APTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void APTestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APTestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
