// Fill out your copyright notice in the Description page of Project Settings.

#include "Blob.h"
#include "Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BC_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Collision"));
	SM_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));

	if (BC_Collision != nullptr && SM_Wall != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Instantiated both UBoxComponent and UStaticMeshComponent for Wall."));

		RootComponent = BC_Collision;
		SM_Wall->SetupAttachment(SM_Wall);
		BC_Collision->OnComponentBeginOverlap.AddDynamic(this, &AWall::OnOverlapBegin);
		BC_Collision->SetGenerateOverlapEvents(true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate either UBoxComponent or UStaticMeshCompnent for Wall."));
	}

}

void AWall::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with Wall!!!"));
	ABlob* blobRef = Cast<ABlob>(OtherActor);

	if (blobRef != nullptr) {
		blobRef->SetCanWallJump(true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast OtherActor as ABlob!"));
	}
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

