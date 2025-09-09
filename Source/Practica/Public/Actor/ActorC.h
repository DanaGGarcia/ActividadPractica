// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorC.generated.h"

UCLASS()
class PRACTICA_API AActorC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = 0, ToolTip = "Salud del personaje"))
	float Salud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = 0, ClampMax = 5000, UIMin = 0, UIMax = 5000, ToolTip = "Escudo del personaje"))
	int Escudo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
