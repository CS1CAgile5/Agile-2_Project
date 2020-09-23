// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

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
	
	//Degbug to make sure the grabber is attached to the pawn can be deleted later
	UE_LOG(LogTemp, Warning, TEXT("Grabber attached"));

	//Physics Handle Setup and check 
	FindPhysicsHandle();

	//Key binding 
	SetUpInput();
}

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle)
	{
		//Debug UE_LOG(LogTemp, Error, TEXT("%s does not have a Physics Handle"), *GetOwner()->GetName());
		return;
	}
}

void UGrabber::SetUpInput()
{
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	
	//Setting Key Binding for grab and release
	if(InputComp)
	{
		//Debug UE_LOG(LogTemp, Warning, TEXT("Input Component Found");
		InputComp->BindAction("Grab_Action", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab_Action", IE_Released, this, &UGrabber::Release);
	}
	else 
	{
		//Debug UE_LOG(LogTemp, Error, TEXT("Input Component NOT Found"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	// Debug for Grabber UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	
	FHitResult HitResult = GetPhysicsBodyReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	//Grabbing Actor
	AActor* ActorHit = HitResult.GetActor();

	// If we hit something then attach the physics handle so that we can grab it.
	if (ActorHit)
	{
		//Checking if the physics handle is attached
		if(!PhysicsHandle) {return;}
		//Grabbing Actor
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release()
{
	//Debug for Release UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	
	//Checking if the physics handle is attached
	if(!PhysicsHandle) {return;}

	//Releasing Actor
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Chekcing if the physics handle is attached
	if(!PhysicsHandle){return;}

	//Grabbing components in reach
	if(PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetPhysicsBodyReach()
{

	//Find player Viewpoint
	FHitResult Hit;

	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(
		FName(""), 
		false, 
		GetOwner()
	);

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//Debug See what was hit 
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
	{
			UE_LOG(LogTemp, Warning, TEXT("%s has fallen"), *ActorHit->GetName());
	}
	
	//Drawing the debug line 
	/* Delete Later
	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		GetPlayerReach(),
		FColor(0, 105, 150),
		false,
		0.f,
		0,
		3.f
	); */

	return Hit;
}

FVector UGrabber::GetWorldPosition()
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	//Find Player location 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	
	return PlayerLocation;
}

FVector UGrabber::GetPlayerReach()
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	//Finding the players reach
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString());
	
	//Calculate Players reach
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector()*Reach;

	return LineTraceEnd;
}