// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"
#include "Internationalization/Text.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Input/NegateFloatInputModifier.h"
#include "Input/ReverseMovementInputModifier.h"

void InitializeJumpInputAction(UInputAction* JumpInputAction, UInputMappingContext* InputMappingContext) {
	UE_LOG(LogTemp, Warning, TEXT("Created JumpInputAction: %s"), JumpInputAction->GetDesc().GetCharArray().GetData());
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	JumpInputAction->Triggers.Add(DownTrigger);
	JumpInputAction->ActionDescription = FText::FromString("Jump");
	JumpInputAction->ValueType = EInputActionValueType::Axis1D;
	// The default is UInputTriggerDown with an actuation threshold > 0. Adding, simply for the practice.
	if (!JumpInputAction->Triggers.IsEmpty()) {
		JumpInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	FEnhancedActionKeyMapping& jump_mapping = InputMappingContext->MapKey(JumpInputAction, EKeys::SpaceBar);
}

void InitializeHorizontalMovementInputAction(UInputAction* HorizontalMovementInputAction, UInputMappingContext* InputMappingContext) {
	UE_LOG(LogTemp, Warning, TEXT("Initializing MoveHorizontalInputAction..."));
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	HorizontalMovementInputAction->Triggers.Add(DownTrigger);
	HorizontalMovementInputAction->ActionDescription = FText::FromString("Move Right");
	HorizontalMovementInputAction->ValueType = EInputActionValueType::Axis1D;
	if (!HorizontalMovementInputAction->Triggers.IsEmpty()) {
		HorizontalMovementInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	// ReverseMovementInputModifier is applied, regardless of direction, so as to enable reversal of movement in that direction.
	UReverseMovementInputModifier* ReverseMovementInputModifier = NewObject<UReverseMovementInputModifier>();
	HorizontalMovementInputAction->Modifiers.Add(ReverseMovementInputModifier);
	// Map this input Action to the keys that can trigger it
	FEnhancedActionKeyMapping& left_horizontal_movement = InputMappingContext->MapKey(HorizontalMovementInputAction, EKeys::A);
	FEnhancedActionKeyMapping& right_horizontal_movement = InputMappingContext->MapKey(HorizontalMovementInputAction, EKeys::D);
}

void InitializeDashActivateInputAction(UInputAction* DashActivateInputAction, UInputMappingContext* InputMappingContext) {
	UE_LOG(LogTemp, Warning, TEXT("Initializing Dash Activate..."))
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	DashActivateInputAction->Triggers.Add(DownTrigger);
	DashActivateInputAction->ValueType = EInputActionValueType::Boolean;
	if (!DashActivateInputAction->Triggers.IsEmpty()) {
		DashActivateInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	FEnhancedActionKeyMapping& dash_activate_left = InputMappingContext->MapKey(DashActivateInputAction, EKeys::A);
	FEnhancedActionKeyMapping& dash_activate_right = InputMappingContext->MapKey(DashActivateInputAction, EKeys::D);
}

void InitializeDashInputAction(UInputAction* DashInputAction, UInputAction* DashActivateInputAction, UInputMappingContext* InputMappingContext) {
	UE_LOG(LogTemp, Warning, TEXT("Initialize Dash..."))
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	DashInputAction->Triggers.Add(DownTrigger);
	DashInputAction->ValueType = EInputActionValueType::Axis1D;
	UInputTriggerChordAction* ChordedAction = NewObject<UInputTriggerChordAction>();
	ChordedAction->ChordAction = DashActivateInputAction;
	if (!DashInputAction->Triggers.IsEmpty()) {
		DashInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	FEnhancedActionKeyMapping& dash = InputMappingContext->MapKey(DashInputAction, EKeys::RightShift);
}

// Sets default values
ABlob::ABlob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Blob Constructed..."));

	UE_LOG(LogTemp, Warning, TEXT("Using C++ defined UInputMappingContext"));
	// Initialize Blob's UInputActions
	InputMappingContext = UObject::CreateDefaultSubobject<UInputMappingContext>(FName("InputMappingContext"));
	JumpInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("JumpInputAction"));
	MoveInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("Move Horizontally UInputAction"));
	DashActivateInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashActivateRightInputAction"));
	DashInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashRightInputAction"));
	if (JumpInputAction) {
		InitializeJumpInputAction(JumpInputAction.Get(), InputMappingContext.Get());
	}
	if (DashActivateInputAction) {
		InitializeDashActivateInputAction(DashActivateInputAction.Get(), InputMappingContext.Get());
	}
	if (DashInputAction) {
		InitializeDashInputAction(DashInputAction.Get(), DashActivateInputAction.Get(), InputMappingContext.Get());
	}
	if (MoveInputAction) {
		InitializeHorizontalMovementInputAction(MoveInputAction.Get(), InputMappingContext.Get());
	}
}

// Called when the game starts or when spawned
void ABlob::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Logging from C++..."));

	APlayerController* controller = Cast<APlayerController>(GetController());
	if (ULocalPlayer* LocalPlayer = controller->GetLocalPlayer()) {
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if (InputMappingContext != nullptr && !InputMappingContext.IsNull()) {
				UE_LOG(LogTemp, Warning, TEXT("Adding InputMappingContext by default..."));
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
			}
		}
	}
}

// Called every frame
void ABlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABlob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind Input Action for Jumping
	Input->BindAction(JumpInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleJumpInputActionInstance);
	// Bind Input Action for Horizontal Movement
	Input->BindAction(MoveInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleHorizontalMovementInputActionInstance);
	// Bind Input Action for Dash to the Right
	Input->BindAction(DashInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashInputActionInstance);
	Input->BindAction(DashActivateInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashActivateInputActionInstance);
}

void ABlob::HandleJumpInputActionInstance(const FInputActionInstance& Instance) {
	this->Jump();
}

void SetNewActorLocation(ABlob* blob, float distance) {
	if (blob == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Blob is inaccessible!"))
		return;
	}
	FVector ActorLocation = blob->GetActorLocation();
	// How far from the Box's Position, a trace will be computed, to look for overlaps
	FVector BoxExtent = FVector(0.0, 40.0, 0.0);
	float NewYPosition = distance + ActorLocation.Y;
	FVector NewActorLocation(ActorLocation.X, NewYPosition, ActorLocation.Z);
	// Types of Objects to search for collisions with.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	// Actor Class to filter on (search only for AActors of this type, that issue collision events via the channels specified above).
	UClass* ActorFilter = AStaticMeshActor::StaticClass();
	// Actor types to ignore.
	TArray<AActor*> IgnoreActors;
	// Actors that overlap with the box extended.
	TArray<AActor*> OutputActors;
	if (!UKismetSystemLibrary::BoxOverlapActors(
		blob->GetWorld(),
		NewActorLocation,
		BoxExtent,
		ObjectTypes,
		ActorFilter,
		IgnoreActors,
		OutputActors)
		) {
		blob->SetActorLocation(NewActorLocation);
	}
}

APlayerController* GetAPlayerController(ABlob* blob) {
	return Cast<APlayerController>(blob->GetController());
}

UEnhancedInputLocalPlayerSubsystem* GetUEnhancedInputLocalPlayerSubsystem(ABlob* blob) {
	if (APlayerController* PlayerController = GetAPlayerController(blob)) {
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				return Subsystem;
			}
		}
	}
	return nullptr;
}

void ABlob::HandleHorizontalMovementInputActionInstance(const FInputActionInstance& Instance) {
	float input_value = 0.0f;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetUEnhancedInputLocalPlayerSubsystem(this);
	if (Subsystem != nullptr) {
		TArray<FKey> horizontal_movement_keys = Subsystem->QueryKeysMappedToAction(this->MoveInputAction.Get());
		APlayerController* controller = GetAPlayerController(this);
		for (FKey input_key : horizontal_movement_keys) {
			if (input_key == EKeys::A) {
				input_value += -1 * controller->GetInputAnalogKeyState(input_key);
			}
			else if (input_key == EKeys::D) {
				input_value += controller->GetInputAnalogKeyState(input_key);
			}
		}
	}
	else {
		input_value = Instance.GetValue().Get<float>();
	}

	if (this->GetReverseMovement()) {
		input_value *= -1.0f;
	}
	

	SetNewActorLocation(this, input_value * HorizontalMovement);
}

void ABlob::HandleDashActivateInputActionInstance(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Activate Triggerred!"))
}



void ABlob::HandleDashInputActionInstance(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Triggerred!"))
	float input_value = Instance.GetValue().Get<float>(),
		distance = 0.0;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetUEnhancedInputLocalPlayerSubsystem(this);
	if (Subsystem != nullptr) {
		UEnhancedPlayerInput* PlayerInput = Subsystem->GetPlayerInput();
		FInputActionValue DashActivateInputActionValue = PlayerInput->GetActionValue(DashActivateInputAction.Get());
		APlayerController* controller = GetAPlayerController(this);

		bool dash_activate_actuation = DashActivateInputActionValue.Get<bool>();
		float direction = 1.0f;

		if (dash_activate_actuation) {
			TArray<FKey> dash_activate_keys = Subsystem->QueryKeysMappedToAction(DashActivateInputAction.Get());
			bool left = false,
				right = false;
			for (FKey input_key : dash_activate_keys) {
				if (input_key == EKeys::A && controller->GetInputAnalogKeyState(input_key) != 0.0f) {
					left = true;
				}
				else if (input_key == EKeys::D && controller->GetInputAnalogKeyState(input_key) != 0.0f) {
					right = true;
				}
			}

			// Dashing in both directions should cancel one another out.
			if (left && right) {
				input_value = 0.0f;
			}
			else if (left) {
				// only move left, if the left key has actually been depressed, thereby yielding a non-zero analog key state for this input.
				direction *= -1;
			}
			// default direction of 1.0 addresses Dash to the right being activated.

			input_value *= direction;
		}
		else {
			// Should not dash, if dash has not been activated.
			input_value = 0.0f;
		}
	}

	distance = input_value * HorizontalMovement * DashMultiplier * (this->GetReverseMovement() ? -1.0f : 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("Dash InputActionInstance (value, distance): (%.3f, %.3f)"), input_value, distance)

	SetNewActorLocation(this, distance);
}

