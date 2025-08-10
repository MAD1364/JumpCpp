// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseMovementInputModifier.h"
#include "Blob.h"
#include "EnhancedPlayerInput.h"

FInputActionValue UReverseMovementInputModifier::ModifyRaw_Implementation(
	const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue,
	float DeltaTim) {
	APlayerController* PlayerController = Cast<APlayerController>(PlayerInput->GetOuterAPlayerController());
	bool bReverseDirection = false;
	if (PlayerController != nullptr) {
		ABlob* Blob = Cast<ABlob>(PlayerController->GetCharacter());
		if (Blob != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Blob Character is accessible from UInputModifier assigned to one of its InputActions!"));
			bReverseDirection = Blob->GetReverseMovement();
		}
	}
	return CurrentValue * (bReverseDirection ? - 1 : 1);
}