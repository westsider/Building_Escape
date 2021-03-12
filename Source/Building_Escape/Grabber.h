// Copyright Perilous Orbit 2021

#pragma once

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 150.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhisicsHandle();
	void SetUpInputComponent();

	FHitResult GetFirstPhisicsBodyInReach() const;
	FVector	GetPlayersReach() const;
	FVector GetPlayersWorldPos() const;
};
