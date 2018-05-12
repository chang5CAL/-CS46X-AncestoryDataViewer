// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeBluePrintScript.h"
#include "Components/TextRenderComponent.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h> //ONE of these are unneccessary. I'm not risking it though.
#include <fstream>
#include <stdio.h>/**/
#include <algorithm>
#include <vector>

#include "lineage_view_spawner.generated.h"

UCLASS()
class ADVR_API Alineage_view_spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Alineage_view_spawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<ANodeBluePrintScript> Node;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString fName;

	struct Person {
		FString id;
		FString name;
		FString spouse;
		FString father;
		FString mother;
		std::vector<FString> children;
		float pos; // use to calculate location
		float totalspace; //space need for child nodes
		int largestchildnode; // the spaces that the largest node takes up
		int level; //This is the Y level. It goes up as the range
				   //int treeLevel;
		FVector p_location;
		bool placed = false;
	};

	std::vector<std::vector<float>> levelMaxVal;
	std::vector<Person> parsedData;
	std::vector<Person> lineageData;
	int xoffset;

	int search_by_id(std::vector<Person> p, FString str);
	std::vector<Person> parse(FString fileLocation);
	void lineage_view(FString root);
	void find_ancestors(std::vector<Person> p, FString root, std::vector<Person> &l);
	void setLevels(FString root, int level);
	int setSpace(FString root);
	void setPosition(FString root, float position);
	void placeNodes(FString root, int middle);
	
};
