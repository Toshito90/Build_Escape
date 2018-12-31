// Fill out your copyright notice in the Description page of Project Settings.

#include "ReporterPos.h"

// Sets default values for this component's properties
UReporterPos::UReporterPos()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReporterPos::BeginPlay()
{
	Super::BeginPlay();

	FString objName = GetOwner()->GetName();
	FString objPos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("This %s is at position %s"), *objName, *objPos);
}


// Called every frame
void UReporterPos::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

