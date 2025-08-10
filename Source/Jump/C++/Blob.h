// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blob.generated.h"

// Because ACharacter is an APawn and APawn is Blueprintable, so is ACharacter and therefore, ABlob is as well.
UCLASS()
class JUMP_API ABlob : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when handling Jump Input
	void HandleJumpInputActionInstance(const FInputActionInstance& Instance);
	// Called when handling Horizontal Movement Input
	void HandleHorizontalMovementInputActionInstance(const FInputActionInstance& Instance);
	// Callback for handling Dash Activate Input
	void HandleDashActivateInputActionInstance(const FInputActionInstance& Instance);
	// Callback for handling Dashing Input
	void HandleDashInputActionInstance(const FInputActionInstance& Instance);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns whether the Blob's movement should be reversed.
	bool GetReverseMovement() const { return ReverseMovement; }
	// Set whether the Blob's movement should be reversed.
	void SetReverseMovement(bool bReverseMovement) { ReverseMovement = bReverseMovement; }

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	TSoftObjectPtr<UInputAction> JumpInputAction;
	TSoftObjectPtr<UInputAction> MoveInputAction;
	// The following two UInputActions constitute the Chorded Action for Dash InputAction.
	TSoftObjectPtr<UInputAction> DashActivateRightInputAction;
	TSoftObjectPtr<UInputAction> DashActivateLeftInputAction;
	TSoftObjectPtr<UInputAction> DashInputAction;
	float JumpZVelocity = 100.0f;
	float HorizontalMovement = 5.0f;
	float DashMultiplier = 10.0f;
	bool ReverseMovement = false;
};
