// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
	reach = 100.f;

	///Look for Attached Physics Handler
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Handle Component!"), *GetOwner()->GetName())
	}

	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!inputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component!"), *GetOwner()->GetName())
	} else {
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"))
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FVector pViewPointLocation;
	FRotator pViewPointRotation;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT pViewPointLocation,
		OUT pViewPointRotation
	);

	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Postion: %s"), 
		*pViewPointLocation.ToString(), 
		*pViewPointRotation.ToString()*/
	//)

	//Draw a Red Trace in the World to visualize
	FVector LineTraceEnd = pViewPointLocation + pViewPointRotation.Vector() * reach;
	DrawDebugLine(GetWorld(), 
		pViewPointLocation, 
		LineTraceEnd, 
		FColor(255, 0, 0, 255), 
		false, 
		0.f, 
		0.f, 
		10.f);

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
}

