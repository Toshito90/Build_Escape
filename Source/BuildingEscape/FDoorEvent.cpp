// Fill out your copyright notice in the Description page of Project Settings.

#include "FDoorEvent.h"

#define OUT

// Sets default values for this component's properties
UFDoorEvent::UFDoorEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFDoorEvent::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();

	if (!pressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("%s missing pressure plate."), *GetOwner()->GetName())
	}
}


// Called every frame
void UFDoorEvent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > triggerMass) {
		onOpenRequest.Broadcast();
	} else {
		onCloseRequest.Broadcast();
		//lastDoorOpenTime = 1.f;
	}
}

float UFDoorEvent::GetTotalMassOfActorsOnPlate() {
	float totalMass = 0.f;

	TArray<AActor*>overlappingActors;

	if (!pressurePlate) {
		return totalMass;
	}
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	for (const auto& actors : overlappingActors) {
		totalMass += actors->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate."), *actors->GetName())
	}

	return totalMass;
}