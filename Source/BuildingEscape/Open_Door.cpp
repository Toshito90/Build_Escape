// Fill out your copyright notice in the Description page of Project Settings.

#include "Open_Door.h"

// Sets default values for this component's properties
UOpen_Door::UOpen_Door()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpen_Door::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}


// Called every frame
void UOpen_Door::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate && pressurePlate->IsOverlappingActor(actorThatOpens)) {
		OpenDoor();

		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime > doorCloseDelay) {
		CloseDoor();
		//lastDoorOpenTime = 1.f;
	}
}


void UOpen_Door::OpenDoor()
{
	owner->SetActorRotation(FRotator(0.f, openAngle, 0.f));
}


void UOpen_Door::CloseDoor()
{
	owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}
