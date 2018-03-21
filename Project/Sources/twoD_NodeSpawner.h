// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "twoD_Node.h"
#include "Components/TextRenderComponent.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "twoD_NodeSpawner.generated.h"

UCLASS()
class ADVR_API AtwoD_NodeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AtwoD_NodeSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AtwoD_Node> Node;

	struct Person {
		FString id;
		FString name;
		FString spouse;
		FString father;
		FString mother;
		std::vector<FString> children;
		float pos; // use to calculate location
		int totalspace; //space need for child nodes
		int largestchildnode; // the spaces that the largest node takes up
		int level; //This is the Y level. It goes up as the range
				   //int treeLevel;
		FVector p_location;
		bool placed = false;
	};

	std::vector<FString> roots;
	std::vector<std::vector<float>> levelMaxVal;
	std::vector<Person> parsedData;
	int xoffset;

	int search_by_id(std::vector<Person> p, FString str);
	std::vector<Person> parse(FString fileLocation);


	//Sorting algorithm functionality
	//void levelInsert(int tree, int level, float val);
	std::vector<FString> findRoots(std::vector<Person> family);
	int findmaxLevel(std::vector<Person> family, FString root);
	void setLevel(FString root, int maxLevel);
	int setSpace(FString root);
	void setPosition(FString sid, float position);
	void placeNodes(FString sid, int middle);
	
};
