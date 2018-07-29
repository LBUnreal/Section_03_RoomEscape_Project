// Lukas Bayard copyright 2018

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	MovePlayerHeldObject();
}



void UGrabber::Grab()
{
	if (!PhysicsHandle)
	{
		return;
	}

	//Line Trace and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent(); //Gets the mesh in this case
	AActor* ActorHit = HitResult.GetActor();

	//If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, //No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(), //Grab at white anchor point
			ComponentToGrab->GetOwner()->GetActorRotation()); //Allow Rotation
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		return;
	}

	PhysicsHandle->ReleaseComponent();
}

///Find or look for UPhysicsHandleComponent
void  UGrabber::FindPhysicsHandleComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Physics Handle Component for: %s"), *(GetOwner()->GetName()));
	}
}

///Look for attached input component
void UGrabber::SetupInputComponent()
{

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Pawn Input Component for: %s"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEnd = GetPlayerLineTraceEnd(PlayerViewPointLocation, PlayerViewPointRotation);
	FHitResult ActorHit = GetLineTraceToReachDistance(PlayerViewPointLocation, LineTraceEnd);

	LogHitActor(ActorHit.GetActor());
	return ActorHit;
}

void UGrabber::MovePlayerHeldObject()
{
	if (!PhysicsHandle) 
	{
		return; 
	}

	//if the phyics handle is attached then we should move the object we are holding
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		FVector LineTraceEnd = GetPlayerLineTraceEnd(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		//Move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FVector UGrabber::GetPlayerLineTraceEnd(OUT FVector PlayerViewPointLocation, OUT FRotator PlayerViewPointRotation)
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return (PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach);

}

//Log what we hit
const void UGrabber::LogHitActor(AActor * Actor)
{
	if (Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(Actor->GetName()));
	}
}

FHitResult UGrabber::GetLineTraceToReachDistance(FVector PointLocation, FVector LineTraceEnd)
{
	///Set up Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA ray-cast) to reach distance
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PointLocation, // Line Trace start
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);


	return LineTraceHit;
}
