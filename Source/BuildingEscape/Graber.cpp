// Fill out your copyright notice in the Description page of Project Settings.

#include "Graber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGraber::UGraber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if the physics handle is attached
	// move the object that we're holding

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor::Red,
		false,
		0.f,
		0.f,
		1.0f
	);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//TODO Line-trace (AKA Ray Cast)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit - %s"), *ActorHit->GetFullName());
	}
}

void UGraber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGraber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Released, this, &UGraber::Release);
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGraber::Grab);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

void UGraber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("You grabbed smth!"));
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();
	/// If we hit something then attach a physics handle
	// TODO attach physics handle
}

void UGraber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("You released smth!"));
}

const FHitResult UGraber::GetFirstPhysicsBodyInReach()
{
	return FHitResult();
}
