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
	bool bDoorResult = false;

	//If any pressure plates are enabled
	if (EnabledPressurePlates)
	{
		bool bEnabledPlates[] = { true, true, true };
		bool bDisabledPlates[] = { true, true, true };

		for (int index = 0; index < 3; index++)
		{
			if (EnabledPressurePlates[index])
			{
				bEnabledPlates[index] = IsWithinTotalMassofActorsOnPlate(EnabledPressurePlates[index]);
			}

			if (DisabledPressurePlates[index])
			{
				bDisabledPlates[index] = !IsWithinTotalMassofActorsOnPlate(DisabledPressurePlates[index]);
			}

		}

		//If the ActorThatOpens is in the Volume
		if (bEnabledPlates[0] && bEnabledPlates[1] && bEnabledPlates[2] &&
			bDisabledPlates[0] && bDisabledPlates[1] && bDisabledPlates[2])
		{
			OnOpen.Broadcast();
		}
		else
		{
			OnClose.Broadcast();
		}
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate(ATriggerVolume* _PressurePlate)
{
	float TotalMass = 0.0f;

	if (!_PressurePlate)
	{
		return TotalMass;
	}

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	_PressurePlate->GetOverlappingActors(OUT OverlappingActors);

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

bool UOpenDoor::IsWithinTotalMassofActorsOnPlate(ATriggerVolume * _PressurePlate)
{
	float TotalMass = GetTotalMassOfActorsOnPlate(_PressurePlate);

	bool Result = (TotalMass > 0 && TotalMass <= TriggerMass);

	UE_LOG(LogTemp, Warning, 
		TEXT("Pressure plate: %s, result: %s"), 
		*(_PressurePlate->GetFName().ToString()), 
		Result ? TEXT("True") :TEXT("False"));

	return (Result);
}

