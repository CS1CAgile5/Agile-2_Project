// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "YSlideDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UYSlideDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYSlideDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FindPressurePlate();
	void FindAudioComponent();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float InitialY;
	float CurrentY;

	UPROPERTY(EditAnywhere)
	float TargetY = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	float LastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float Delay = 2.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = .5f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY()
	UAudioComponent* DoorAudio = nullptr;

	bool OpenSound = false;
	bool CloseSound = true;
};
