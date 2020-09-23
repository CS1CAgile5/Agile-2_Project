// Fill out your copyright notice in the Description page of Project Settings.


#include "YSlideDoor.h"

// Sets default values for this component's properties
UYSlideDoor::UYSlideDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UYSlideDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//Intialize Y variables 
	InitialY = GetOwner()->GetActorLocation().Y;
	CurrentY = InitialY;
	TargetY += InitialY;

	//Finds pressure plate attached
	FindPressurePlate();

	//Finds Audio Component Attached
	FindAudioComponent();
}

void UYSlideDoor::FindPressurePlate()
{
	//Protect from a null pointer with the pressure plates 
	if(!PressurePlate)
	{
		//Debug UE_LOG(LogTemp, Error, TEXT("Actor %s does NOT have a pressure plate."), *GetOwner()->GetName());
		return;
	}
}

void UYSlideDoor:: FindAudioComponent()
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
void UYSlideDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UYSlideDoor::OpenDoor(float DeltaTime)
{
	//Open Door
	CurrentY = FMath::Lerp(CurrentY, TargetY, OpenSpeed *DeltaTime);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Y = CurrentY;
	GetOwner()->SetActorLocation(DoorLocation);

	//Play Open Door sound 
	CloseSound = false;
	if(!DoorAudio) {return;}
	if (!OpenSound)
	{
		DoorAudio->Play();
		OpenSound = true;
	}
}

void UYSlideDoor::CloseDoor(float DeltaTime)
{
	//Close Door 
	CurrentY = FMath::Lerp(CurrentY, InitialY, CloseSpeed *DeltaTime);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Y = CurrentY;
	GetOwner()->SetActorLocation(DoorLocation);

	//Play Close Door sound
	OpenSound = false;
	if(!DoorAudio) {return;}
	if (!CloseSound)
	{
		DoorAudio->Play();
		CloseSound = true;
	}
}

float UYSlideDoor::TotalMassOfActors()
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

