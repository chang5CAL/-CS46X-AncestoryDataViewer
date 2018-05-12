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
#include "Runtime/Core/Public/Misc/FileHelper.h"
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("In Game Mode"));



	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("We are in TreeGen"));
	FString file = "C:/Users/liyon/Desktop/cs/cs461/ged/s.ged";
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*file)) {
		//if(1){
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Can't find file! File name: ") + file);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("We are parsing"));
		parsedData = parse(file);
		roots = findRoots(parsedData);



		/* Debugging messages
		if (!parsedData.empty()) {
		int i;
		for (i = 0; i < parsedData.size(); i++) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("ParseData[0].id: ") + parsedData[i].id);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("ParseData[0].name: ") + parsedData[i].name);
		}
		}
		else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Empty ParseData"));
		}
		if (!roots.empty()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("roots[0]: ") + roots[0]);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("roots[0]: ") + roots[1]);
		}
		else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Empty Root"));
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("roots: ") + FString::FromInt(roots.size()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Root[1]: ") + n);
		*/

		int id;

		id = search_by_id(parsedData, roots[0]);


		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("max level: ") + FString::FromInt(maxLevel));
		//int maxLevel = 8;
		float middle = 0;
		xoffset = 120;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("roots: ") + FString::FromInt(roots.size()));
		for (int i = 0; i < roots.size(); i++) {
			id = search_by_id(parsedData, roots[i]); // find index of root
			int maxLevel = findmaxLevel(parsedData, roots[i]);
			setLevel(roots[i], maxLevel); // set the levels recursively from top to bottom 
			int maxSpace = setSpace(roots[i]); // find the spaces needed for all the nodes, maxSpace is space for root node
											   //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("roots max space: ") + FString::FromInt(maxSpace));
			xoffset = 120; // space between nodes
			middle = middle + maxSpace * xoffset / 2; //starting point of the root node
													  //GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("roots name: ") + parsedData[id].name);
													  //GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("roots total space: ") + FString::FromInt(parsedData[id].totalspace));
													  //GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("roots level: ") + FString::FromInt(parsedData[id].level));
													  //GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("roots middle: ") + FString::FromInt(middle));
			setPosition(roots[i], 0); // calculate node's offset from parent, use for calculating the location of node
			placeNodes(roots[i], middle); // calculate location of node, place node, and save location
			middle = middle + maxSpace * xoffset / 2;
		}
		//find_common_ancestor(parsedData[20].id, parsedData[9].id);
		find_common_ancestor(parsedData[0].id, parsedData[9].id);
		find_common_ancestor(parsedData[0].id, parsedData[1].id);
	}
}

// Called every frame
void AtwoD_NodeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
static ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
{
return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : NULL);
}
*/




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
	int i, j, k, z, husb, wife, chil, have_father, have_mother;
	FString str;
	FString str2 = "0 @I";
	FString str3 = "1 NAME";
	FString str4 = "1 HUSB";
	FString str5 = "1 WIFE";
	FString str6 = "1 CHIL";
	FString str7 = "0 @F";
	have_father = 0;
	have_mother = 0;
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
				have_father = 1;
			}
			if (parsed[i].Contains(str5, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 8, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				wife = search_by_id(p, parsed[i]);
				have_mother = 1;
				if (have_father == 1) {
					p[wife].spouse = p[husb].id;
					p[husb].spouse = p[wife].id;
				}
			}
			if (parsed[i].Contains(str6, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 8, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				chil = search_by_id(p, parsed[i]);
				if (have_father == 1 && have_mother == 1) {
					p[wife].children.push_back(p[chil].id);
					p[husb].children.push_back(p[chil].id);
					p[chil].father = p[husb].id;
					p[chil].mother = p[wife].id;
				}
				else if (have_father == 1) {
					p[husb].children.push_back(p[chil].id);
					p[chil].father = p[husb].id;
				}
				else if (have_mother == 1) {
					p[wife].children.push_back(p[chil].id);
					p[chil].mother = p[wife].id;
				}
			}
			if (parsed[i].Contains(str7, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				have_father = 0;
				have_mother = 0;
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
	/*if (p[id].level >= 0) {
	return p[id].level;
	}*/
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
		if (!parsedData[id].spouse.IsEmpty()) {
			parsedData[id].totalspace = 5;
			parsedData[id].largestchildnode = 5;
			return 5;
		}
		else {
			parsedData[id].totalspace = 3;
			parsedData[id].largestchildnode = 3;
			return 3;
		}
	}
	for (int i = 0; i < parsedData[id].children.size(); i++) {
		curVal = setSpace(parsedData[id].children[i]);
		int cid = search_by_id(parsedData, parsedData[id].children[i]);
		if (!parsedData[cid].placed) {
			maxVal += curVal;
		}
		/*
		if (curVal > maxVal) {
		if (!parsedData[cid].placed) {
		maxVal = curVal;
		}
		}*/
	}
	if (parsedData[id].children.size() != 0) {
		if (!parsedData[id].spouse.IsEmpty() && maxVal == 0) {
			maxVal = 5;
		}
		else if (maxVal == 0) {
			maxVal = 3;
		}
		parsedData[id].largestchildnode = maxVal;
		parsedData[id].totalspace = maxVal;
	}
	return parsedData[id].totalspace;
}

// Set position of nodes
void AtwoD_NodeSpawner::setPosition(FString sid, float position) {
	int id = search_by_id(parsedData, sid);
	parsedData[id].pos = position;
	if (!parsedData[id].spouse.IsEmpty()) {
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		parsedData[id].pos = position;
		parsedData[spouseid].pos = position;
	}
	if (parsedData[id].children.size() != 0) {
		int child_id = search_by_id(parsedData, parsedData[id].children[0]);
		int child_id2;
		//float positionOfFirstNode = ((parsedData[id].totalspace / 2) - (parsedData[id].largestchildnode / 2)) * -1; modify algorithm
		float child1_halfSpace = (parsedData[child_id].totalspace / 2);
		float child2_halfSpace = (parsedData[child_id].totalspace / 2);
		float positionOfFirstNode = ((parsedData[id].totalspace / 2) - (parsedData[child_id].totalspace / 2)) * -1;
		for (int i = 0; i < parsedData[id].children.size(); i++) {
			setPosition(parsedData[id].children[i], positionOfFirstNode);
			//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, TEXT("position: ") + FString::FromInt(positionOfFirstNode));
			if (i + 1 < parsedData[id].children.size()) {
				child_id2 = search_by_id(parsedData, parsedData[id].children[i + 1]);
				child1_halfSpace = child2_halfSpace;
				child2_halfSpace = (parsedData[child_id2].totalspace / 2);
				positionOfFirstNode = positionOfFirstNode + child1_halfSpace + child2_halfSpace;
			}
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
	location.X = x - 120;
	location.Y = 0.0f;
	location.Z = z;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NULL;
	SpawnParams.Instigator = Instigator;


	AtwoD_Node* newNode = GetWorld()->SpawnActor<AtwoD_Node>(Node, location, FRotator::ZeroRotator, SpawnParams);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[id].name + "\n" + parsedData[id].id);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);

	parsedData[id].placed = true;
	parsedData[id].p_location = location;

	if (!parsedData[id].spouse.IsEmpty()) {
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		x = middle + 120;
		z = parsedData[spouseid].level * 300;
		location2.X = x;
		location2.Y = 0.0f;
		location2.Z = z;

		AtwoD_Node* newNode = GetWorld()->SpawnActor<AtwoD_Node>(Node, location2, FRotator::ZeroRotator, SpawnParams);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[spouseid].name + "\n" + parsedData[spouseid].id);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);

		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);

		parsedData[spouseid].placed = true;
		parsedData[spouseid].p_location = location2;
	}

	for (int i = 0; i < parsedData[id].children.size(); i++) {
		int cid = search_by_id(parsedData, parsedData[id].children[i]); // get child id

		if (parsedData[cid].placed == false) { // if child has not been created
			float childMiddle = middle + parsedData[cid].pos * 120;  // get the middle position of child and spouse
			if (!parsedData[id].spouse.IsEmpty()) {
				location.X = middle;// +120;	// middle between the parents
			}
			else {
				location.X = middle - 120;
			}
			location2.X = childMiddle;
			location2.Z = z - 300;

			if (!parsedData[cid].spouse.IsEmpty()) {
				placeNodes(parsedData[id].children[i], childMiddle + 120); // place the child node
				ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
				location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
				LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
			}
			else {
				placeNodes(parsedData[id].children[i], childMiddle + 240); // place the child node
				location2.X = location2.X + 120;  // change x to point to the child
				ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher; // create line
				LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
			}
		}
		else {  // if child already created, connect child to parent
			if (!parsedData[id].spouse.IsEmpty()) {
				location.X = middle;// +120;	// middle between the parents
			}
			else {
				location.X = middle - 120;
			}
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
			else {
				// If spouse doesn't exist
				roots.push_back(family[i].id);
			}
		}
	}
	return roots;
}


void AtwoD_NodeSpawner::find_ancestors(std::vector<Person> p, FString root, std::vector<FString> &ancestor_array) {
	//int i;
	Person p_father, p_spouse, p_mother;
	int id, spouse_id, p_id;
	int search_father = 0;
	int search_father_in_law = 0;
	id = search_by_id(parsedData, root);

	if (!p[id].father.IsEmpty()) { // current person's parent
		p_id = search_by_id(parsedData, p[id].father);
		p_father = p[p_id];
		ancestor_array.push_back(p_father.id);
		if (!p_father.father.IsEmpty() || !p_father.mother.IsEmpty()) {
			find_ancestors(p, p_father.id, ancestor_array);
			search_father = 1;
		}
	}
	if (!p[id].mother.IsEmpty()) {
		p_id = search_by_id(parsedData, p[id].mother);
		p_mother = p[p_id];
		ancestor_array.push_back(p_mother.id);
		if (!p_mother.father.IsEmpty() || !p_mother.mother.IsEmpty() && search_father == 0) {
			find_ancestors(p, p_mother.id, ancestor_array);
		}
	}
	if (!p[id].spouse.IsEmpty()) {
		spouse_id = search_by_id(parsedData, p[id].spouse);
		p_spouse = p[spouse_id];
		//l.push_back(p_spouse);
		if (!p_spouse.father.IsEmpty()) { // spouse's parent
			p_id = search_by_id(parsedData, p[spouse_id].father);
			p_father = p[p_id];
			ancestor_array.push_back(p_father.id);
			if (!p_father.father.IsEmpty() || !p_father.mother.IsEmpty()) {
				find_ancestors(p, p_father.id, ancestor_array);
				search_father_in_law = 1;
			}
		}
		if (!p_spouse.mother.IsEmpty()) {
			p_id = search_by_id(parsedData, p[spouse_id].mother);
			p_mother = p[p_id];
			ancestor_array.push_back(p_mother.id);
			if (!p_mother.father.IsEmpty() || !p_mother.mother.IsEmpty() && search_father_in_law == 0) {
				find_ancestors(p, p_mother.id, ancestor_array);
			}
		}
	}
}

void AtwoD_NodeSpawner::find_common_ancestor(FString p1, FString p2) {
	std::vector<FString> ancestor_array1, ancestor_array2;
	int i, j, id1, id2, id;
	Person common_ancestor;
	find_ancestors(parsedData, p1, ancestor_array1);
	find_ancestors(parsedData, p2, ancestor_array2);
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person1 ancestor size: ") + FString::FromInt(ancestor_array1.size()));
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person2 ancestor size: ") + FString::FromInt(ancestor_array2.size()));
	/*
	for (i = 0; i < ancestor_array1.size(); i++) {
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person1 ancestor: ") + ancestor_array1[i]);;
	}
	for (i = 0; i < ancestor_array2.size(); i++) {
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person2 ancestor: ") + ancestor_array2[i]);;
	}
	*/
	for (i = 0; i < ancestor_array1.size(); i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person1 ancestor: ") + ancestor_array1[i]);
		for (j = 0; j < ancestor_array2.size(); j++) {
			//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person2 ancestor: ") + ancestor_array2[j]);
			if (ancestor_array1[i].Contains(ancestor_array2[j], ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("common ancestor id: ") + ancestor_array2[j]);
				id = search_by_id(parsedData, ancestor_array1[i]);
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("common ancestor name: ") + parsedData[id].name);
				common_ancestor = parsedData[id];
				j = ancestor_array2.size();
				i = ancestor_array1.size();
			}
		}
	}
	id1 = search_by_id(parsedData, p1);
	id2 = search_by_id(parsedData, p2);
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("Person2 spouse: ") + parsedData[id2].spouse);
	if (common_ancestor.id.IsEmpty()) {
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("These people don't have a common ancestor"));
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("Person2: ") + parsedData[id2].name);
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("Person1: ") + parsedData[id1].name);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("Common Ancestor: ") + common_ancestor.name);
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("Person2: ") + parsedData[id2].name);
		GEngine->AddOnScreenDebugMessage(-1, 400.f, FColor::Black, TEXT("Person1: ") + parsedData[id1].name);
	}

}