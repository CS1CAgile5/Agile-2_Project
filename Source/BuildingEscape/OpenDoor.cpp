// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//Intialize Yaw variables 
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	//Finds pressure plate attached
	FindPressurePlate();

	//Finds Audio Component Attached
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	//Protect from a null pointer with the pressure plates 
	if(!PressurePlate)
	{
		//Debug UE_LOG(LogTemp, Error, TEXT("Actor %s does NOT have a pressure plate."), *GetOwner()->GetName());
		return;
	}
}

void UOpenDoor:: FindAudioComponent()
{
	//Finds Audio Component based on class
	DoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();
	
	//Protect from null pointer
	if(!DoorAudio) 
	{
		//Debug
		UE_LOG(LogTemp, Error, TEXT("%s does not have an Audio Component"), *GetOwner()->GetName());
		return;
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//Opening and Closing doors based on Mass and delay
	if(TotalMassOfActors() > MassToOpenDoor)
	{	
		OpenDoor(DeltaTime);
		LastOpened = GetWorld()->GetTimeSeconds();
	}
	else 
	{
		if(GetWorld()->GetTimeSeconds() - LastOpened > Delay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//Open Door
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, OpenSpeed *DeltaTime);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	//Play Open Door sound 
	CloseSound = false;
	if(!DoorAudio) {return;}
	if (!OpenSound)
	{
		DoorAudio->Play();
		OpenSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//Close Door 
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, CloseSpeed *DeltaTime);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	//Play Close Door sound
	OpenSound = false;
	if(!DoorAudio) {return;}
	if (!CloseSound)
	{
		DoorAudio->Play();
		CloseSound = true;
	}
}

float UOpenDoor::TotalMassOfActors()
{
	float TotalWeight = 0.f;

	//Finds all actors on the pressure plate
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Calculates Sum of All Actors on the pressure plate
	if(!PressurePlate) {return 0.f;}
	for(AActor* Actor : OverlappingActors)
	{
		TotalWeight += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalWeight;
}
