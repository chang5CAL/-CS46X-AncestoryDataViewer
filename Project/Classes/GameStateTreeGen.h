// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/TextRenderComponent.h"
#include "LevelEditor.h"
#include "Editor.h"
#include "Engine.h"
#include "Components/ShapeComponent.h"
#include "NodeBluePrintScript.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h> //ONE of these are unneccessary. I'm not risking it though.
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "GameStateTreeGen.generated.h"
/**
 * 
 */
UCLASS()
class ADVR_API AGameStateTreeGen : public AGameModeBase
{

	GENERATED_BODY()

	TSubclassOf<class UNodeBluePrintScript> Node;
	public:
		struct Person {
			std::string id;
			std::string name;
			std::string spouse;
			std::string father;
			std::string mother;
			std::vector<std::string> children;
			float pos = 0;
			float range = 0;
			float level = 0; //This is the Y level. It goes up as the range
			int treeLevel;
			bool placed = false;
		};

		AGameStateTreeGen();
	private:
		std::vector<std::string> roots;
		std::vector<std::vector<float>> levelMaxVal;
		std::vector<Person> parsedData;


		//Parser Functionality
		int search_by_id(std::vector<Person> p, std::string str);
		std::vector<Person> parse(std::string fileLocation);

		//Sorting algorithm functionality
		void levelInsert(int tree, int level, float val);
		std::vector<std::string> findRoots(std::vector<Person> family);
		float spacingChildren(int tree, std::string sid, std::vector<Person> p, int level);
		void place(int tree, std::string sid, std::vector<Person> p, float minPos);

		//Generate
		void placeNodes(Person p, std::vector<Person> family, Person parent);
		void placeSpouse(Person p, std::vector<Person> family);
	
};

// Fill out your copyright notice in the Description page of Project Settings.





