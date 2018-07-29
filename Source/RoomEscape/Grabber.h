// Lukas Bayard copyright 2018

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"
#define OUT

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//How far the player can reach 
	const float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab whats in reach
	void Grab();

	//Release what has been grabbed
	void Release();

	//Find (assumed) attached physics handle
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	//Return hit fore first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Object being grabbed moves with the player
	void MovePlayerHeldObject();

	//Gets the line trace end of the player
	FVector GetPlayerLineTraceEnd(OUT FVector PlayerViewPointLocation, OUT FRotator PlayerViewPointRotation);

	const void LogHitActor(AActor* Actor);

	FHitResult GetLineTraceToReachDistance(FVector LocationPoint, FVector LineTraceEnd);
};
