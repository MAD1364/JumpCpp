// Fill out your copyright notice in the Description page of Project Settings.


#include "NegateFloatInputModifier.h"
#include "Blob.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/Character.h"

FInputActionValue UNegateFloatInputModifier::ModifyRaw_Implementation(
	const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue,
	float DeltaTime) {
	return CurrentValue.Get<float>() * -1;
}