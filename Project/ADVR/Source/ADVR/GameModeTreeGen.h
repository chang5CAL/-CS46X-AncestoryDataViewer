// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NodeBluePrintScript.h"
#include "GameModeTreeGen.generated.h"

/**
 * 
 */
UCLASS()
class ADVR_API AGameModeTreeGen : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	void CreateNode(FVector location);
	
	public:
		UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<ANodeBluePrintScript> Node;
	
};


