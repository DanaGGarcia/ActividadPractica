// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorC.h"

// Sets default values
AActorC::AActorC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

