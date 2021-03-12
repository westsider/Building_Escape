// Copyright Perilous Orbit 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"


//#define OUT

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

	FindPhisicsHandle();
	SetUpInputComponent();
}

void UGrabber::FindPhisicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// phisics is found
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle fount %s"), *GetOwner()->GetName());
	}

}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Input component found!"));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!"));

	FHitResult HitResult =  GetFirstPhisicsBodyInReach();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, 
		PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}

	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released!"));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *PlayerViewPointLocation.ToString() );

	// draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhisicsBodyInReach() const
{
	// get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *PlayerViewPointLocation.ToString() );

	// draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	// ray-cast (reach)
	FHitResult  Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	// see what were hitting
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace has hit %s"), *(ActorHit->GetName()));
	}
	return Hit;
}

