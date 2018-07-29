// Lukas Bayard copyright 2018

#include "OpenDoor.h"
#include "Grabber.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	SetUpDoorActors();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PollTriggerVolume();
}

void UOpenDoor::SetUpDoorActors()
{
	Owner = GetOwner();
}

void UOpenDoor::PollTriggerVolume()
{
	//If the ActorThatOpens is in the Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) 
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	if (!PressurePlate)
	{
		return TotalMass;
	}

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their Masses
	for (const auto* OActor : OverlappingActors)
	{
		//Log the actors name
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *(OActor->GetName()));

		UPrimitiveComponent* PhysicsComponent = OActor->FindComponentByClass<UPrimitiveComponent>();
		TotalMass += PhysicsComponent->CalculateMass();
	}

	return TotalMass;
}

