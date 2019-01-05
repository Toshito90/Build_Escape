// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	reach = 100.f;

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))

	auto hitResult = GetPhysicsBodyInReach();

	auto componentToGrab = hitResult.GetComponent();

	auto actorHit = hitResult.GetActor();

	if (actorHit) {
		physicsHandle->GrabComponent(componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"))

		physicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	///Look for Attached Physics Handler
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Handle Component!"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!inputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component!"), *GetOwner()->GetName())
	}
	else {
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

const FHitResult UGrabber::GetPhysicsBodyInReach() {
	FVector pViewPointLocation;
	FRotator pViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT pViewPointLocation,
		OUT pViewPointRotation
	);

	//Draw a Red Trace in the World to visualize
	FVector LineTraceEnd = pViewPointLocation + pViewPointRotation.Vector() * reach;

	//Setup Query Parameters
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	//Line Tracing (AKA Raycast) out to reach distance
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		pViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameters
	);


	AActor* hitActor = hit.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(hitActor->GetName()))
	}

	return hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector pViewPointLocation;
	FRotator pViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT pViewPointLocation,
		OUT pViewPointRotation
	);

	//Draw a Red Trace in the World to visualize
	FVector LineTraceEnd = pViewPointLocation + pViewPointRotation.Vector() * reach;


	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

