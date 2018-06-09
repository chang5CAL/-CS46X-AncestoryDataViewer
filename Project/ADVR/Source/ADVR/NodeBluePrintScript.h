// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeBluePrintScript.generated.h"

UCLASS()
class ADVR_API ANodeBluePrintScript : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANodeBluePrintScript();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
