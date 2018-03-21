// Fill out your copyright notice in the Description page of Project Settings.

#include "twoD_NodeSpawner.h"
#include <string>
#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "Components/LineBatchComponent.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "twoD_Node.h"
#include "PlatformFileManager.h"
#include "FileHelpers.h"
#include "UnrealString.h"
#include "Vector.h"

// Sets default values
AtwoD_NodeSpawner::AtwoD_NodeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AtwoD_NodeSpawner::BeginPlay()
{
	Super::BeginPlay();

	FString file = "C:/Users/liyon/Desktop/cs/cs461/ged/d.ged";

	parsedData = parse(file);
	roots = findRoots(parsedData);

	int id;
	id = search_by_id(parsedData, roots[0]);

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("Root: childs: ") + FString::FromInt(parsedData[id].children.size()));

	int maxLevel = findmaxLevel(parsedData, roots[0]);
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("max level: ") + FString::FromInt(maxLevel));

	float middle = 0;
	xoffset = 120;

	for (int i = 0; i < roots.size(); i++) {
		id = search_by_id(parsedData, roots[i]); // find index of root
		setLevel(roots[i], maxLevel); // set the levels recursively from top to bottom 
		int maxSpace = setSpace(roots[i]); // find the spaces needed for all the nodes, maxSpace is space for root node
		xoffset = 120; // space between nodes
		middle = middle + maxSpace * xoffset / 2; //starting point of the root node
		setPosition(roots[i], 0); // calculate node's offset from parent, use for calculating the location of node
		placeNodes(roots[i], middle); // calculate location of node, place node, and save location
	}
	
}

// Called every frame
void AtwoD_NodeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Parser Functionality
int AtwoD_NodeSpawner::search_by_id(std::vector<Person> p, FString str) {
	int i = 0;
	while (i < p.size()) {
		if (p[i].id.Equals(str, ESearchCase::CaseSensitive)) {
			//cout << "husb  " << i << endl;
			return i;
		}
		i++;
	}
	return -1; //Return an impossible value if the id doesn't exist.
}

std::vector<AtwoD_NodeSpawner::Person> AtwoD_NodeSpawner::parse(FString fileLocation) {
	std::vector<Person> p;
	int i, j, k, z, husb, wife, chil;
	FString str;
	FString str2 = "0 @I";
	FString str3 = "1 NAME";
	FString str4 = "1 HUSB";
	FString str5 = "1 WIFE";
	FString str6 = "1 CHIL";
	//IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile;
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*fileLocation)) {
		//if(1){
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Can't find file"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("File name: ") + fileLocation);
		return p;
	}
	else {
		j = 0;
		k = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("FILE EXIST"));
		FString FileData = "temp";
		FFileHelper::LoadFileToString(FileData, *fileLocation);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FileData);
		TArray<FString> parsed;
		int32 linecount = FileData.ParseIntoArray(parsed, TEXT("\n"), true);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, parsed[0]);


		FString count = FString::FromInt(linecount);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("count: ")+count);
		for (i = 0; i < linecount; i++) {
			if (parsed[i].Contains(str2, ESearchCase::CaseSensitive, ESearchDir::FromStart))
			{
				p.push_back(Person());
				parsed[i].RemoveAt(0, 3, true);
				while (parsed[i].Contains("@")) {
					parsed[i] = parsed[i].LeftChop(1);
				}
				p[j].id = parsed[i];
				j++;
				//parsed[i].RemoveAt(trimloc, parsed[i].Len() - trimloc, true);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, parsed[i]);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("length: ") + FString::FromInt(parsed[i].Len()));
			}

			if (parsed[i].Contains(str3, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 7, true);
				for (z = 0; z < parsed[i].Len(); z++) {
					if (parsed[i][z] == '/') {
						parsed[i].RemoveAt(z, 1, true);
					}
				}
				parsed[i].RemoveAt(parsed[i].Len() - 1, 1, true);
				//p[k].name = parsed[i];

				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("k: ") + FString::FromInt(k));

				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("p size: ") + FString::FromInt(p.size()));
				if (k < p.size()) {
					p[k].name = parsed[i];
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("p name: ") + p[k].name);
					k++;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("name: ") + parsed[i]);

			}

			if (parsed[i].Contains(str4, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 8, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				husb = search_by_id(p, parsed[i]);
			}
			if (parsed[i].Contains(str5, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 8, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				wife = search_by_id(p, parsed[i]);
				p[wife].spouse = p[husb].id;
				p[husb].spouse = p[wife].id;
			}
			if (parsed[i].Contains(str6, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 8, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				chil = search_by_id(p, parsed[i]);
				p[wife].children.push_back(p[chil].id);
				p[husb].children.push_back(p[chil].id);
				p[chil].father = p[husb].id;
				p[chil].mother = p[wife].id;
			}

		}
	}
	i = 0;

	return p;
}


//Sorting algorithm functionality


/*
void ANodeSpawner::levelInsert(int tree, int level, float val) {
//Checks if an inputted value is greater than
//the current value, and if it doesn't exist,
//create all levels until it does.
std::vector<float> f;
while (levelMaxVal.size() < tree) {
//If the outer vector doesn't have a vector, then add it in until it does.
levelMaxVal.push_back(f);
}
while (levelMaxVal[tree].size() < level) {
levelMaxVal[tree].push_back(0);
}
if (levelMaxVal[tree][level - 1] < val) {
levelMaxVal[tree][level - 1] = val;
}
//Remember, if we get 3, we add 0,1,2 as 0,0,0. Therefore 2 is the third value.
}
*/


// use id of root and find the max level which well be the level for the root node. Highest level
int AtwoD_NodeSpawner::findmaxLevel(std::vector<Person> p, FString sid) {
	double maxLev = 0;
	double curLev = 0;
	int id = search_by_id(p, sid);
	if (p[id].children.size() == 0) {
		return 1;
	}
	for (int i = 0; i < p[id].children.size(); i++) {
		curLev = findmaxLevel(p, p[id].children[i]) + 1;

		if (curLev > maxLev) {
			maxLev = curLev;
		}
	}
	return maxLev;
}

// Set the level of nodes recursively from root. child nodes are 1 less than parent nodes.
void AtwoD_NodeSpawner::setLevel(FString sid, int maxLevel) {
	int id = search_by_id(parsedData, sid);
	parsedData[id].level = maxLevel;
	if (!parsedData[id].spouse.IsEmpty()) {
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		parsedData[spouseid].level = maxLevel;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("parsed ID: max level: ") + FString::FromInt(parsedData[id].level));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("p ID: max level: ") + FString::FromInt(p[id].level));
	for (int i = 0; i < parsedData[id].children.size(); i++) {
		setLevel(parsedData[id].children[i], maxLevel - 1);
	}
}

//find and set values are largestchildnode and totalspace
int AtwoD_NodeSpawner::setSpace(FString sid) {
	int id = search_by_id(parsedData, sid);
	int curVal = 0;
	int maxVal = 0;
	if (parsedData[id].children.size() == 0) {
		parsedData[id].totalspace = 5;
		parsedData[id].largestchildnode = 5;
		return 5;
	}
	for (int i = 0; i < parsedData[id].children.size(); i++) {
		curVal = setSpace(parsedData[id].children[i]);
		int cid = search_by_id(parsedData, parsedData[id].children[i]);
		if (curVal > maxVal) {
			if (!parsedData[cid].placed) {
				maxVal = curVal;
			}
		}
	}
	if (parsedData[id].children.size() != 0) {
		parsedData[id].largestchildnode = maxVal;
		parsedData[id].totalspace = maxVal * parsedData[id].children.size();
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("largest: ") + FString::FromInt(parsedData[id].largestchildnode));
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("TotalSpace: ") + FString::FromInt(parsedData[id].totalspace));
	}
	return parsedData[id].totalspace;
}

// Set position of nodes
void AtwoD_NodeSpawner::setPosition(FString sid, float position) {
	int id = search_by_id(parsedData, sid);
	parsedData[id].pos = position - 1;
	if (!parsedData[id].spouse.IsEmpty()) {
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		parsedData[spouseid].pos = position + 1;
	}
	if (parsedData[id].children.size() != 0) {
		float positionOfFirstNode = ((parsedData[id].totalspace / 2) - (parsedData[id].largestchildnode / 2)) * -1;
		for (int i = 0; i < parsedData[id].children.size(); i++) {
			setPosition(parsedData[id].children[i], positionOfFirstNode);
			//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, TEXT("position: ") + FString::FromInt(positionOfFirstNode));
			positionOfFirstNode = positionOfFirstNode + parsedData[id].largestchildnode;
		}
	}
}

void AtwoD_NodeSpawner::placeNodes(FString sid, int middle) {
	int id = search_by_id(parsedData, sid);
	float x = middle;
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("id: position; ") + FString::FromInt(parsedData[id].pos));
	float z = parsedData[id].level * 300;
	FVector location;
	FVector location2;
	location.X = x;
	location.Y = 0.0f;
	location.Z = z;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NULL;
	SpawnParams.Instigator = Instigator;


	AtwoD_Node* newNode = GetWorld()->SpawnActor<AtwoD_Node>(Node, location, FRotator::ZeroRotator, SpawnParams);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[id].name);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);

	parsedData[id].placed = true;
	parsedData[id].p_location = location;

	if (!parsedData[id].spouse.IsEmpty()) {
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		x = middle + 240;
		z = parsedData[spouseid].level * 300;
		location2.X = x;
		location2.Y = 0.0f;
		location2.Z = z;

		AtwoD_Node* newNode = GetWorld()->SpawnActor<AtwoD_Node>(Node, location2, FRotator::ZeroRotator, SpawnParams);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[spouseid].name);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);
		location.Y = -10.0f;
		location2.Y = -10.0f;
		parsedData[spouseid].placed = true;
		parsedData[spouseid].p_location = location2;



		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
	}

	for (int i = 0; i < parsedData[id].children.size(); i++) {
		int cid = search_by_id(parsedData, parsedData[id].children[i]); // get child id

		if (parsedData[cid].placed == false) { // if child has not been created
			float childMiddle = middle + parsedData[cid].pos * 120;  // get the middle position of child and spouse
			location.X = middle + 120;
			location2.X = childMiddle;
			location2.Z = z - 300;

			if (!parsedData[cid].spouse.IsEmpty()) {
				placeNodes(parsedData[id].children[i], childMiddle + 120); // place the child node
				ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
				location.Y = -10.0f;
				location2.Y = -10.0f;
				location2.X = location2.X + 120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
				LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
			}
			else {
				placeNodes(parsedData[id].children[i], childMiddle + 240); // place the child node
				location2.X = location2.X + 240;  // change x to point to the child
				location.Y = -10.0f;
				location2.Y = -10.0f;
				ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher; // create line
				LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
			}
		}
		else {  // if child already created, connect child to parent
			location.X = middle + 120;
			location.Y = -10.0f;
			location2.Y = -10.0f;
			ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher; // create line
			LineBatcher->DrawLine(location, parsedData[cid].p_location, FColor::Red, 0, 10.0f);
		}
	}

}

std::vector<FString> AtwoD_NodeSpawner::findRoots(std::vector<Person> family) {
	//We just store the id.
	bool spouseAdded = false;
	int spouseID;
	for (int i = 0; i<family.size(); ++i) {
		//I can do this since I know the family is stored in a vector so I can just iterate through it.
		if (family[i].father.IsEmpty() && family[i].mother.IsEmpty()) {
			if (!family[i].spouse.IsEmpty()) {
				//If spouse exists
				spouseID = search_by_id(family, family[i].spouse);
				if ((family[spouseID].father.IsEmpty() && family[spouseID].mother.IsEmpty())) { // code deleted: family[i].spouse.IsEmpty() ||
																								//Check if spouse either doesn't exist or doesn't have parents either.
					for (int j = 0; j < roots.size(); j++) {
						if (roots[j] == family[i].spouse) {
							//Will need to come back to solve divorces.
							spouseAdded = true;
						}
					}
					//If either the spouse doesn't exist, or they haven't been added already, add it
					if (!spouseAdded) {
						//If the spouse has been added already, don't add.
						roots.push_back(family[i].id);
					}

					spouseAdded = false;
					//So the fatal flaw is that we assume the roots married once and had children.
					//If W-X and Y-Z both had children, then X-Y and W-Z got together, we'll forget to
					//push one in.
					//Fortunately, GEDCOM files only allow one spouse, so we just need to keep track of
					//divorces.
				}
			}
		}
	}
	return roots;
}