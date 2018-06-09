// Fill out your copyright notice in the Description page of Project Settings.

#include "lineage_view_spawner.h"
#include <string>
#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "Components/LineBatchComponent.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "NodeBluePrintScript.h"
#include "PlatformFileManager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "UnrealString.h"
#include "Vector.h"


// Sets default values
Alineage_view_spawner::Alineage_view_spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Alineage_view_spawner::BeginPlay()
{
	Super::BeginPlay();

	FString file = fName;

	FString pLineage = personLineage;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("We are parsing"));
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*file)) {
		//if(1){
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Can't find file! File name: ") + file);
	}
	else {
		parsedData = parse(file);

		int id;

		id = search_by_id(parsedData, pLineage);

		lineage_view(parsedData[id].id);
	}
}

// Called every frame
void Alineage_view_spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Parser Functionality
int Alineage_view_spawner::search_by_id(std::vector<Person> p, FString str) {
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

std::vector<Alineage_view_spawner::Person> Alineage_view_spawner::parse(FString fileLocation) {
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
				parsed[i].RemoveAt(0, 4, true);
				while (parsed[i].Contains("@")) {
					parsed[i] = parsed[i].LeftChop(1);
				}
				while (parsed[i].Mid(0, 1).Equals(TEXT("0"))) {
					parsed[i] = parsed[i].RightChop(1);
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
				parsed[i].RemoveAt(0, 9, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				while (parsed[i].Mid(0, 1).Equals(TEXT("0"))) {
					parsed[i] = parsed[i].RightChop(1);
				}
				husb = search_by_id(p, parsed[i]);
				have_father = 1;
			}
			if (parsed[i].Contains(str5, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 9, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				while (parsed[i].Mid(0, 1).Equals(TEXT("0"))) {
					parsed[i] = parsed[i].RightChop(1);
				}
				wife = search_by_id(p, parsed[i]);
				have_mother = 1;
				if (have_father == 1) {
					p[wife].spouse = p[husb].id;
					p[husb].spouse = p[wife].id;
				}
			}
			if (parsed[i].Contains(str6, ESearchCase::CaseSensitive, ESearchDir::FromStart)) {
				parsed[i].RemoveAt(0, 9, true);
				parsed[i].RemoveAt(parsed[i].Len() - 2, 2, true);
				while (parsed[i].Mid(0, 1).Equals(TEXT("0"))) {
					parsed[i] = parsed[i].RightChop(1);
				}
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

void Alineage_view_spawner::find_ancestors(std::vector<Person> p, FString root, std::vector<Person> l) {
	//int i;
	Person p_mother, p_father, p_spouse;
	int id;

	id = search_by_id(parsedData, root);


	if (!p[id].father.IsEmpty()) { // current person's parent
		id = search_by_id(parsedData, p[id].father);
		p_father = p[id];
		l.push_back(p_father);
		if (!p_father.father.IsEmpty() || !p_father.mother.IsEmpty()) {
			find_ancestors(p, p_father.id, l);
		}
	}
	if (!p[id].mother.IsEmpty()) {
		id = search_by_id(parsedData, p[id].mother);
		p_mother = p[id];
		l.push_back(p_mother);
		if (!p_mother.father.IsEmpty() || !p_mother.mother.IsEmpty()) {
			find_ancestors(p, p_mother.id, l);
		}
	}
	if (!p[id].spouse.IsEmpty()) {
		id = search_by_id(parsedData, p[id].spouse);
		p_spouse = p[id];
		//l.push_back(p_spouse);
		if (!p_spouse.father.IsEmpty()) { // spouse's parent
			id = search_by_id(parsedData, p[id].father);
			p_father = p[id];
			l.push_back(p_father);
			if (!p_father.father.IsEmpty() || !p_father.mother.IsEmpty()) {
				find_ancestors(p, p_father.id, l);
			}
		}
		if (!p_spouse.mother.IsEmpty()) {
			id = search_by_id(parsedData, p[id].mother);
			p_mother = p[id];
			l.push_back(p_mother);
			if (!p_mother.father.IsEmpty() || !p_mother.mother.IsEmpty()) {
				find_ancestors(p, p_mother.id, l);
			}
		}
	}
}

void Alineage_view_spawner::setLevels(FString root, int level) {
	int spouseid;
	int id = search_by_id(parsedData, root);
	parsedData[id].level = level;
	if (!parsedData[id].father.IsEmpty()) {
		setLevels(parsedData[id].father, level + 1);
	}
	else {
		if (!parsedData[id].mother.IsEmpty()) {
			setLevels(parsedData[id].mother, level + 1);
		}
	}
	if (!parsedData[id].spouse.IsEmpty()) {
		spouseid = search_by_id(parsedData, parsedData[id].spouse);
		parsedData[spouseid].level = level;
		if (!parsedData[spouseid].father.IsEmpty()) {
			setLevels(parsedData[spouseid].father, level + 1);
		}
		else {
			if (!parsedData[spouseid].mother.IsEmpty()) {
				setLevels(parsedData[spouseid].mother, level + 1);
			}
		}
	}
}


int Alineage_view_spawner::setSpace(FString root) {
	int id = search_by_id(parsedData, root);
	int curSpace = 0;
	int totalSpace = 0;
	if (!parsedData[id].spouse.IsEmpty()) {
		int spouse_id = search_by_id(parsedData, parsedData[id].spouse);
		if (parsedData[spouse_id].father.IsEmpty() && parsedData[spouse_id].mother.IsEmpty()) { //spouse = no parents
			totalSpace = totalSpace + 0; // total space needed plus 0 because spouse has no parents
			if (parsedData[id].father.IsEmpty() && parsedData[id].mother.IsEmpty()) { // if current person and their spouse doesn't have parent return 5
				parsedData[id].totalspace = 5;
				parsedData[spouse_id].totalspace = 5;
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
				return 5;
			}
			else {
				if (!parsedData[id].father.IsEmpty()) { // spouse = no parent, current person = has parent
					curSpace = setSpace(parsedData[id].father);
					totalSpace = totalSpace + curSpace;
					parsedData[id].totalspace = totalSpace;
					parsedData[spouse_id].totalspace = totalSpace;
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
					return totalSpace;
				}
				else {
					curSpace = setSpace(parsedData[id].mother);
					totalSpace = totalSpace + curSpace;
					parsedData[id].totalspace = totalSpace;
					parsedData[spouse_id].totalspace = totalSpace;
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
					return totalSpace;
				}
			}
		}
		else { //spouse has parent(s)
			if (!parsedData[spouse_id].father.IsEmpty()) {
				curSpace = setSpace(parsedData[spouse_id].father);
				totalSpace = totalSpace + curSpace;
			}
			else {
				curSpace = setSpace(parsedData[spouse_id].mother);
				totalSpace = totalSpace + curSpace;
			}
			if (parsedData[id].father.IsEmpty() && parsedData[id].mother.IsEmpty()) { // spouse have parent, current person = no parent
				parsedData[id].totalspace = totalSpace;
				parsedData[spouse_id].totalspace = totalSpace;
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
				return totalSpace;
			}
			else {
				if (!parsedData[id].father.IsEmpty()) {
					curSpace = setSpace(parsedData[id].father); // spouse have parents, current person have parents
					totalSpace = totalSpace + curSpace;
					parsedData[id].totalspace = totalSpace;
					parsedData[spouse_id].totalspace = totalSpace;
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
					return totalSpace;
				}
				else {
					curSpace = setSpace(parsedData[id].mother);
					totalSpace = totalSpace + curSpace;
					parsedData[id].totalspace = totalSpace;
					parsedData[spouse_id].totalspace = totalSpace;
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
					//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
					return totalSpace;
				}
			}
		}
	}
	else { // no spouse
		if (parsedData[id].father.IsEmpty() && parsedData[id].mother.IsEmpty()) { // no spouse, current person = no parent
			parsedData[id].totalspace = 3;
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
			return 3;
		}
		else {//current person have parents
			if (!parsedData[id].father.IsEmpty()) {
				curSpace = setSpace(parsedData[id].father);
				totalSpace = totalSpace + curSpace;
				parsedData[id].totalspace = totalSpace;
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
				return totalSpace;
			}
			else {
				curSpace = setSpace(parsedData[id].mother);
				totalSpace = totalSpace + curSpace;
				parsedData[id].totalspace = totalSpace;
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
				return totalSpace;
			}
		}
	}


}

void Alineage_view_spawner::setPosition(FString root, float position) {
	int id = search_by_id(parsedData, root);
	int father_id, mother_id;
	float temp_position;
	parsedData[id].pos = position;
	/*if (!parsedData[id].spouse.IsEmpty()) {
	int spouseid = search_by_id(parsedData, parsedData[id].spouse);
	parsedData[spouseid].pos = position;
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("individual1: id: ") + parsedData[id].id);
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse1: id: ") + parsedData[spouseid].id);
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level: ") + FString::FromInt(parsedData[id].level));
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space: ") + FString::FromInt(parsedData[id].totalspace));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("position: ") + FString::FromInt(parsedData[id].pos));
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse level: ") + FString::FromInt(parsedData[spouseid].level));
	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse space: ") + FString::FromInt(parsedData[spouseid].totalspace));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse position: ") + FString::FromInt(parsedData[spouseid].pos));
	if (!parsedData[id].father.IsEmpty()) { // current individual have parent
	if (!parsedData[spouseid].father.IsEmpty()) {// spouse have parent
	setPosition(parsedData[id].father, position - 2);
	setPosition(parsedData[spouseid].father, position + 2);
	}
	else { // spouse don't have parent
	setPosition(parsedData[id].father, position);
	}
	}
	else { // individual don't have parent
	if (!parsedData[spouseid].father.IsEmpty()) { // spouse have parent
	setPosition(parsedData[spouseid].father, position);
	}
	}
	}
	else { // no spouse = parent
	if (!parsedData[id].father.IsEmpty()) {
	setPosition(parsedData[id].father, position);
	}
	else if (!parsedData[id].mother.IsEmpty()) {
	setPosition(parsedData[id].mother, position);
	}
	}*/

	if (!parsedData[id].spouse.IsEmpty()) { // has spouse
		int spouseid = search_by_id(parsedData, parsedData[id].spouse);
		parsedData[id].pos = position - 1;
		parsedData[spouseid].pos = position + 1;
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("individual1: id: ") + parsedData[id].id);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse1: id: ") + parsedData[spouseid].id);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level: ") + FString::FromInt(parsedData[id].level));
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space: ") + FString::FromInt(parsedData[id].totalspace));
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("position: ") + FString::FromInt(parsedData[id].pos));
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse level: ") + FString::FromInt(parsedData[spouseid].level));
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse space: ") + FString::FromInt(parsedData[spouseid].totalspace));
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("spouse position: ") + FString::FromInt(parsedData[spouseid].pos));
		if (!parsedData[id].father.IsEmpty()) { // current person has parent
			father_id = search_by_id(parsedData, parsedData[id].father);
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level1: ") + FString::FromInt(parsedData[father_id].level));
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space1: ") + FString::FromInt(parsedData[id].totalspace));
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("father_space1: ") + FString::FromInt(parsedData[father_id].totalspace));
			temp_position = ((parsedData[id].totalspace / 2) - (parsedData[father_id].totalspace / 2)) * -1;
			//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("temp_position1: ") + FString::FromInt(temp_position));
			setPosition(parsedData[id].father, temp_position);	// set for parent
			temp_position = temp_position + (parsedData[father_id].totalspace / 2);
			if (!parsedData[spouseid].father.IsEmpty()) {
				father_id = search_by_id(parsedData, parsedData[spouseid].father);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level2: ") + FString::FromInt(parsedData[father_id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space2: ") + FString::FromInt(parsedData[father_id].totalspace));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("father_space2: ") + FString::FromInt(parsedData[father_id].totalspace));
				temp_position = temp_position + (parsedData[father_id].totalspace / 2);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("temp_position2: ") + FString::FromInt(temp_position));
				setPosition(parsedData[spouseid].father, temp_position);
			}
			else if (!parsedData[spouseid].mother.IsEmpty()) {
				mother_id = search_by_id(parsedData, parsedData[spouseid].mother);
				temp_position = temp_position + (parsedData[mother_id].totalspace / 2);
				setPosition(parsedData[spouseid].mother, temp_position);
			}
		}
		else if (!parsedData[id].mother.IsEmpty()) {
			mother_id = search_by_id(parsedData, parsedData[id].mother);
			temp_position = ((parsedData[id].totalspace / 2) - (parsedData[mother_id].totalspace / 2)) * -1;
			setPosition(parsedData[id].mother, temp_position);
			temp_position = temp_position + (parsedData[mother_id].totalspace / 2);
			if (!parsedData[spouseid].father.IsEmpty()) {
				father_id = search_by_id(parsedData, parsedData[spouseid].father);
				temp_position = temp_position + (parsedData[father_id].totalspace / 2);
				setPosition(parsedData[spouseid].father, temp_position);
			}
			else if (!parsedData[spouseid].mother.IsEmpty()) {
				mother_id = search_by_id(parsedData, parsedData[spouseid].mother);
				temp_position = temp_position + (parsedData[mother_id].totalspace / 2);
				setPosition(parsedData[spouseid].mother, temp_position);
			}
		}
		else {
			if (!parsedData[spouseid].father.IsEmpty()) {
				father_id = search_by_id(parsedData, parsedData[spouseid].father);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("level2: ") + FString::FromInt(parsedData[father_id].level));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("id: ") + parsedData[id].id);
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("space2: ") + FString::FromInt(parsedData[father_id].totalspace));
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("father_space2: ") + FString::FromInt(parsedData[father_id].totalspace));
				temp_position = ((parsedData[id].totalspace / 2) - (parsedData[father_id].totalspace / 2)) * -1;
				//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("temp_position2: ") + FString::FromInt(temp_position));
				setPosition(parsedData[spouseid].father, temp_position);
			}
			else if (!parsedData[spouseid].mother.IsEmpty()) {
				mother_id = search_by_id(parsedData, parsedData[spouseid].mother);
				temp_position = ((parsedData[id].totalspace / 2) - (parsedData[mother_id].totalspace / 2)) * -1;
				setPosition(parsedData[spouseid].mother, temp_position);
			}
		}
	}
	else { // no spouse, parent is right on top of current person
		if (!parsedData[id].father.IsEmpty()) {
			setPosition(parsedData[id].father, position);
		}
		else if (!parsedData[id].mother.IsEmpty()) {
			setPosition(parsedData[id].mother, position);
		}
	}
}

void Alineage_view_spawner::placeNodes(FString root, int middle) {
	int id = search_by_id(parsedData, root);
	float x = middle;
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Black, TEXT("id: position; ") + FString::FromInt(parsedData[id].pos));
	float z = parsedData[id].level * 300;
	FVector location;
	FVector location2;
	location.X = x - 120;
	location.Y = 0.0f;
	location.Z = z;
	int spouseid;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NULL;
	SpawnParams.Instigator = Instigator;


	ANodeBluePrintScript* newNode = GetWorld()->SpawnActor<ANodeBluePrintScript>(Node, location, FRotator::ZeroRotator, SpawnParams);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[id].name);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
	newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);

	parsedData[id].placed = true;
	parsedData[id].p_location = location;

	if (!parsedData[id].spouse.IsEmpty()) {
		spouseid = search_by_id(parsedData, parsedData[id].spouse);
		x = middle + 120;
		z = parsedData[spouseid].level * 300;
		location2.X = x;
		location2.Y = 0.0f;
		location2.Z = z;

		ANodeBluePrintScript* newNode = GetWorld()->SpawnActor<ANodeBluePrintScript>(Node, location2, FRotator::ZeroRotator, SpawnParams);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetText(parsedData[spouseid].name);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetHorizontalAlignment(EHTA_Center);
		newNode->FindComponentByClass<UTextRenderComponent>()->SetVerticalAlignment(EVRTA_TextCenter);

		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);

		parsedData[spouseid].placed = true;
		parsedData[spouseid].p_location = location2;
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("middle: ") + FString::FromInt(middle));
	}
	if (!parsedData[id].father.IsEmpty()) {
		int parent_id = search_by_id(parsedData, parsedData[id].father);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent1: id: ") + parsedData[parent_id].id);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent1: position: ") + FString::FromInt(parsedData[parent_id].pos));
		int parent_middle = middle + parsedData[parent_id].pos * 120;
		location.X = middle - 120;// +120;	// middle between the parents
		location2.X = parent_middle;
		location2.Z = z + 300;
		placeNodes(parsedData[id].father, parent_middle); // place the child node
		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		if (!parsedData[id].mother.IsEmpty()) {
			location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
		}
		else {
			location2.X = location2.X - 120;
		}
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent1: middle: ") + FString::FromInt(parent_middle));
	}
	else if (!parsedData[id].mother.IsEmpty()) {
		int parent_id = search_by_id(parsedData, parsedData[id].mother);
		int parent_middle = middle + parsedData[parent_id].pos * 120;
		location.X = middle - 120;// +120;	// middle between the parents
		location2.X = parent_middle;
		location2.Z = z + 300;
		placeNodes(parsedData[id].mother, parent_middle); // place the child node
		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		if (!parsedData[id].father.IsEmpty()) {
			location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
		}
		else {
			location2.X = location2.X - 120;
		}
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
	}
	if (!parsedData[spouseid].father.IsEmpty()) {
		int parent_id = search_by_id(parsedData, parsedData[spouseid].father);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent2: id: ") + parsedData[parent_id].id);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent2: position: ") + FString::FromInt(parsedData[parent_id].pos));
		int parent_middle = middle + parsedData[parent_id].pos * 120;
		location.X = middle + 120;// +120;	// middle between the parents
		location2.X = parent_middle;
		location2.Z = z + 300;
		placeNodes(parsedData[spouseid].father, parent_middle); // place the child node
		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		if (!parsedData[spouseid].mother.IsEmpty()) {
			location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
		}
		else {
			location2.X = location2.X - 120;
		}
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Black, TEXT("Parent2: middle: ") + FString::FromInt(parent_middle));
	}
	else if (!parsedData[spouseid].mother.IsEmpty()) {
		int parent_id = search_by_id(parsedData, parsedData[spouseid].mother);
		int parent_middle = middle + parsedData[parent_id].pos * 120;
		if (!parsedData[spouseid].mother.IsEmpty()) {
			location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
		}
		else {
			location2.X = location2.X - 120;
		}
		location2.X = parent_middle;
		location2.Z = z + 300;
		placeNodes(parsedData[spouseid].mother, parent_middle); // place the child node
		ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
		location2.X = location2.X;// +120;  // change x to 240 to point to the middle of the couple  // 120 to the child of the parent
		LineBatcher->DrawLine(location, location2, FColor::Red, 0, 10.0f);
	}
}


void Alineage_view_spawner::lineage_view(FString root) {
	int id, spouse_id;
	//Person root_person;
	id = search_by_id(parsedData, root);	// get root person
	parsedData.push_back(parsedData[id]);

	//root_person = parsedData[id];
	if (!parsedData[id].spouse.IsEmpty()) {	// if root has spouse, add spouse
		spouse_id = search_by_id(parsedData, parsedData[id].spouse);
		parsedData.push_back(parsedData[spouse_id]);
	}

	//find_ancestors(parsedData, root, parsedData); // find ancestors

	setLevels(root, 0);

	int maxSpace = setSpace(root);
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("Maxspace: ") + FString::FromInt(maxSpace));
	int middle = maxSpace / 2 * 120;
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("middle: ") + FString::FromInt(middle));
	setPosition(root, 0);

	placeNodes(root, middle);

}


	