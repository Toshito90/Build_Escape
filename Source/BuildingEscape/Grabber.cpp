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
	//Setup Query Parameters
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	
	//Line Tracing (AKA Raycast) out to reach distance
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameters
	);


	/*AActor* hitActor = hitResult.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(hitActor->GetName()))
	}*/

	return hitResult;
}

FVector UGrabber::GetReachLineStart() {
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT pViewPointLocation,
		OUT pViewPointRotation
	);

	//Draw a Red Trace in the World to visualize
	//LineTraceEnd = pViewPointLocation + pViewPointRotation.Vector() * reach;


	/*if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(LineTraceEnd);
	}*/

	return pViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT pViewPointLocation,
		OUT pViewPointRotation
	);

	//Draw a Red Trace in the World to visualize
	LineTraceEnd = pViewPointLocation + pViewPointRotation.Vector() * reach;

	return LineTraceEnd;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetReachLineEnd();
	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

