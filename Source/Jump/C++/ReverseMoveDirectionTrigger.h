// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blob.h"
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ReverseMoveDirectionTrigger.generated.h"

UCLASS()
class JUMP_API AReverseMoveDirectionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReverseMoveDirectionTrigger();

	// Function Delegated to, when the relevant UPrimitiveComponent is overlapped.
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the delegating object's timer delay is up
	void ClearReverseDirectionTriggerCondition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//UPROPERTY(EditAnywhere, Category="Trigger Static Mesh")
	UStaticMeshComponent* SM_Trigger;
	//UPROPERTY(EditAnywhere, Category = "Trigger Collision")
	USphereComponent* AC_SphereCollision;
	ABlob* BlobRef;
	float DelayTime = 4.0f;

};
