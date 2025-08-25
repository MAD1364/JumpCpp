// Fill out your copyright notice in the Description page of Project Settings.

#include "Blob.h"
#include "Ground.h"

// Sets default values
AGround::AGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BC_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SM_Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Static Mesh"));

	if (BC_Collision != nullptr && SM_Ground != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Successfully instantiated Ground's Box Collision and Static Mesh..."));
		RootComponent = BC_Collision;
		// Have the Box Collision be the parent of the static mesh, facilitating transform operations involving both.
		SM_Ground->SetupAttachment(BC_Collision);
		BC_Collision->OnComponentBeginOverlap.AddDynamic(this, &AGround::OnOverlapBegin);
		BC_Collision->SetGenerateOverlapEvents(true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("At least one of BC_Collision or SM_Ground failed to initialize!"));
	}
}

void AGround::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlapping with Ground!"));

	ABlob* blobRef = Cast<ABlob>(OtherActor);

	if (blobRef == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast OtherActor as ABlob*..."));
	}

	blobRef->SetCanWallJump(false);
	blobRef->SetIsOnGround(true);
}

// Called when the game starts or when spawned
void AGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

