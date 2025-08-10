// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseMoveDirectionTrigger.h"
#include "CollisionSettings/CollisionSettingsFactory.h"
#include "Blob.h"

// Sets default values
AReverseMoveDirectionTrigger::AReverseMoveDirectionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up USphereComponent for this Object.

	AC_SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SM_Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	if (AC_SphereCollision != nullptr && SM_Trigger != nullptr) {
		RootComponent = AC_SphereCollision;
		AC_SphereCollision->InitSphereRadius(32.0f);
		AC_SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		AC_SphereCollision->SetGenerateOverlapEvents(true);
		//AC_SphereCollision->SetCollisionResponseToChannels(CreateCollisionResponseContainer());
		AC_SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AReverseMoveDirectionTrigger::OnOverlapBegin);

		// Set up UStaticMeshComponent for this Object.
		SM_Trigger->SetupAttachment(RootComponent);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

		if (SphereVisualAsset.Succeeded()) {
			UE_LOG(LogTemp, Warning, TEXT("Setting Static Mesh for Sphere to Trigger..."))
			SM_Trigger->SetStaticMesh(SphereVisualAsset.Object);
			SM_Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
			SM_Trigger->SetWorldScale3D(FVector(0.8));
			SM_Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to procure Shape_Sphere Static Mesh..."))
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to create USphereComponent or UStaticMeshComponent!"))
	}

}

void AReverseMoveDirectionTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping with Trigger!!!"));
	BlobRef = Cast<ABlob>(OtherActor);
	if (BlobRef == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast OtherActor as ABlob*..."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Sucessfully cast OtherActor* as ABlob*..."));
	OverlappedComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SM_Trigger->ToggleVisibility(false);
	bool bBlobReverseMovement = BlobRef->GetReverseMovement();
	BlobRef->SetReverseMovement(!bBlobReverseMovement);

	// Set Timer (i.e. countdown) to call a function after some delay.
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AReverseMoveDirectionTrigger::ClearReverseDirectionTriggerCondition, 1.0f, false, DelayTime);
}

// Called when the game starts or when spawned
void AReverseMoveDirectionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AReverseMoveDirectionTrigger::ClearReverseDirectionTriggerCondition()
{
	if (BlobRef != nullptr) {
		bool bBlobReverseMovement = BlobRef->GetReverseMovement();
		BlobRef->SetReverseMovement(!bBlobReverseMovement);
		AC_SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SM_Trigger->ToggleVisibility(true);
	}
}

// Called every frame
void AReverseMoveDirectionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

