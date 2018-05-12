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

#include "NodeSpawner.generated.h"

UCLASS()
class ADVR_API ANodeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateNode(FVector location);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ANodeBluePrintScript> Node;
	

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
	//float spacingChildren(int tree, FString sid, std::vector<Person> p, int level);
	//void place(int tree, FString sid, std::vector<Person> p, float minPos);

	//Generate
	//void placeNodes(Person p, std::vector<Person> family, Person parent);
	//void placeSpouse(Person p, std::vector<Person> family);
	//static ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground);
	void find_ancestors(std::vector<Person> p, FString root, std::vector<FString> &l);
	void find_common_ancestor(FString p1, FString p2);
};
