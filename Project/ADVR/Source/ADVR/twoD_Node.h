// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "twoD_Node.generated.h"

UCLASS()
class ADVR_API AtwoD_Node : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AtwoD_Node();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};