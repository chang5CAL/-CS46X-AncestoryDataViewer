// Fill out your copyright notice in the Description page of Project Settings.

#include "FileReader.h"
#include "PlatformFileManager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "UnrealString.h"

// Sets default values for this component's properties
UFileReader::UFileReader()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFileReader::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFileReader::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFileReader::checkFile(FString filePath) {
	FString file = filePath;

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*file)) {
		return false;
	}
	return true;
}