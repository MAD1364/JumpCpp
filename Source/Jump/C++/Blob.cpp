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

void InitializeHorizontalMovementInputAction(UInputAction* HorizontalMovementInputAction, UInputMappingContext* InputMappingContext, bool Negate) {
	UE_LOG(LogTemp, Warning, TEXT("Initializing Move%sInputAction..."), Negate ? "Left" : "Right");
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	HorizontalMovementInputAction->Triggers.Add(DownTrigger);
	HorizontalMovementInputAction->ActionDescription = FText::FromString("Move Right");
	HorizontalMovementInputAction->ValueType = EInputActionValueType::Axis1D;
	if (!HorizontalMovementInputAction->Triggers.IsEmpty()) {
		HorizontalMovementInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	if (Negate) {
		UNegateFloatInputModifier* NegateFloatInputModifier = NewObject<UNegateFloatInputModifier>();
		HorizontalMovementInputAction->Modifiers.Add(NegateFloatInputModifier);
	}
	// ReverseMovementInputModifier is applied, regardless of direction, so as to enable reversal movement in that direction.
	UReverseMovementInputModifier* ReverseMovementInputModifier = NewObject<UReverseMovementInputModifier>();
	HorizontalMovementInputAction->Modifiers.Add(ReverseMovementInputModifier);
	// Map this input Action to the keys that can trigger it
	FEnhancedActionKeyMapping& horizontal_movement = InputMappingContext->MapKey(HorizontalMovementInputAction, Negate ? EKeys::A : EKeys::D);
}

void InitializeDashActivateInputAction(UInputAction* DashActivateInputAction, UInputMappingContext* InputMappingContext, bool Negate) {
	UE_LOG(LogTemp, Warning, TEXT("Initializing Dash Activate %s..."), Negate ? "Left" : "Right")
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	DashActivateInputAction->Triggers.Add(DownTrigger);
	DashActivateInputAction->ValueType = EInputActionValueType::Axis1D;
	if (!DashActivateInputAction->Triggers.IsEmpty()) {
		DashActivateInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	if (Negate) {
		UNegateFloatInputModifier* NegateFloatInputModifier = NewObject<UNegateFloatInputModifier>();
		DashActivateInputAction->Modifiers.Add(NegateFloatInputModifier);
	}
	FEnhancedActionKeyMapping& dash_activate = InputMappingContext->MapKey(DashActivateInputAction, Negate ? EKeys::A : EKeys::D);
}

void InitializeDashInputAction(UInputAction* DashInputAction, UInputAction* DashActivateInputAction, UInputMappingContext* InputMappingContext, bool Negate) {
	UE_LOG(LogTemp, Warning, TEXT("Initialize Dash..."), Negate ? "Left" : "Right")
	UInputTriggerDown* DownTrigger = NewObject<UInputTriggerDown>();
	DashInputAction->Triggers.Add(DownTrigger);
	DashInputAction->ValueType = EInputActionValueType::Axis1D;
	UInputTriggerChordAction* ChordedAction = NewObject<UInputTriggerChordAction>();
	ChordedAction->ChordAction = DashActivateInputAction;
	if (!DashInputAction->Triggers.IsEmpty()) {
		DashInputAction->Triggers[0].Get()->ActuationThreshold = 0.5;
	}
	if (Negate) {
		UNegateFloatInputModifier* NegateFloatInputModifier = NewObject<UNegateFloatInputModifier>();
		DashInputAction->Modifiers.Add(NegateFloatInputModifier);
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
	MoveRightInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("MoveRightInputAction"));
	MoveLeftInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("MoveLeftInputAction"));
	DashActivateRightInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashActivateRightInputAction"));
	DashActivateLeftInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashActivateLeftInputAction"));
	DashRightInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashRightInputAction"));
	DashLeftInputAction = UObject::CreateDefaultSubobject<UInputAction>(FName("DashLefttInputAction"));
	if (JumpInputAction) {
		InitializeJumpInputAction(JumpInputAction.Get(), InputMappingContext.Get());
	}
	if (DashActivateRightInputAction) {
		InitializeDashActivateInputAction(DashActivateRightInputAction.Get(), InputMappingContext.Get(), false);
	}
	if (DashActivateLeftInputAction) {
		InitializeDashActivateInputAction(DashActivateLeftInputAction.Get(), InputMappingContext.Get(), true);
	}
	if (DashRightInputAction) {
		InitializeDashInputAction(DashRightInputAction.Get(), DashActivateRightInputAction.Get(), InputMappingContext.Get(), false);
	}
	/*if (DashLeftInputAction) {
		InitializeDashInputAction(DashLeftInputAction.Get(), DashActivateLeftInputAction.Get(), InputMappingContext.Get(), true);
	}*/
	if (MoveRightInputAction) {
		InitializeHorizontalMovementInputAction(MoveRightInputAction.Get(), InputMappingContext.Get(), false);
	}
	if (MoveLeftInputAction) {
		InitializeHorizontalMovementInputAction(MoveLeftInputAction.Get(), InputMappingContext.Get(), true);
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
	// Bind Input Action for Horizontal Movement to the Right
	Input->BindAction(MoveRightInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleHorizontalMovementInputActionInstance);
	// Bind Input Action for Horizontal Movement to the Left
	Input->BindAction(MoveLeftInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleHorizontalMovementInputActionInstance);
	// Bind Input Action for Dash to the Right
	Input->BindAction(DashRightInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashInputActionInstance);
	// Bind Input Action for Dash to the Left
	//Input->BindAction(DashLeftInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashInputActionInstance);
	Input->BindAction(DashActivateRightInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashActivateInputActionInstance);
	Input->BindAction(DashActivateLeftInputAction.Get(), ETriggerEvent::Triggered, this, &ABlob::HandleDashActivateInputActionInstance);
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

void ABlob::HandleHorizontalMovementInputActionInstance(const FInputActionInstance& Instance) {
	float FloatValue = Instance.GetValue().Get<float>(),
		distance = FloatValue * HorizontalMovement;

	SetNewActorLocation(this, distance);
}

void ABlob::HandleDashActivateInputActionInstance(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Activate Triggerred!"))
}



void ABlob::HandleDashInputActionInstance(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Triggerred!"))
	float FloatValue = Instance.GetValue().Get<float>(),
		distance = FloatValue * HorizontalMovement * DashMultiplier * (this->GetReverseMovement() ? -1.0f : 1.0f);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				UEnhancedPlayerInput* PlayerInput = Subsystem->GetPlayerInput();
				FInputActionValue DashActivateRightInputActionValue = PlayerInput->GetActionValue(DashActivateRightInputAction.Get());
				FInputActionValue DashActivateLeftInputActionValue = PlayerInput->GetActionValue(DashActivateLeftInputAction.Get());

				float right = DashActivateRightInputActionValue.Get<float>(),
					left = DashActivateLeftInputActionValue.Get<float>();

				if (left != 0) {
					distance *= -1;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Dash InputActionInstance (value, distance): (%.3f, %.3f)"), FloatValue, distance)

	SetNewActorLocation(this, distance);
}

