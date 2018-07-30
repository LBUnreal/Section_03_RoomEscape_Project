// Lukas Bayard copyright 2018

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	UPROPERTY(EditAnywhere)
		ATriggerVolume* EnabledPressurePlates[3]; //Pressure plates that need to be on for the door to open

	UPROPERTY(EditAnywhere)
		ATriggerVolume* DisabledPressurePlates[3]; //Pressure plates that need to be off for the door to open

	UPROPERTY(EditAnywhere)
		float TriggerMass = 5.0f;

	AActor* Owner = nullptr; //Owning Door

	void SetUpDoorActors();
	void PollTriggerVolume();
	float GetTotalMassOfActorsOnPlate(ATriggerVolume* _PressurePlate);
	bool IsWithinTotalMassofActorsOnPlate(ATriggerVolume* _PressurePlate);
};
