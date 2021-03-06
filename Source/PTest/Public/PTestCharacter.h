// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "GameFramework/Character.h"
#include "PTestCharacter.generated.h"

UCLASS(config=Game)
class APTestCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		bool CanRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		bool IsIronsight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control)
		bool IsCrouch;



protected:

	void FirePress();
	void FireRelease();

	void AimPress();
	void AimRelease();

	void RunPress();
	void RunRelease();

	void CrouchPress();
	void CroouchRelease();



	void MoveForward(float Value);

	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAxis(float Rate);
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAxis(float Rate);
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float deltaTime) override;
};

