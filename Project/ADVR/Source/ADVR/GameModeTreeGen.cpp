// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeTreeGen.h"
#include "Engine.h"
#include "NodeBluePrintScript.h"

void AGameModeTreeGen::BeginPlay() 
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("In Game Mode"));
	CreateNode(FVector(1200.f, 5.f, 150.f));
	UWorld* const World = GetWorld();
	if (World) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("In Has World"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = NULL;
		SpawnParams.Instigator = Instigator;
		ANodeBluePrintScript * newNode = World->SpawnActor<ANodeBluePrintScript>(Node, FVector(1200, 5, 100), FRotator::ZeroRotator, SpawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("End Has World"));
	}
}

void AGameModeTreeGen::CreateNode(FVector Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("In CreateNode"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NULL;
	SpawnParams.Instigator = Instigator;
	
	APlayerCameraManager *camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	Location = camManager->GetCameraLocation();
	Location.X += 100;
	

	ANodeBluePrintScript * newNode = GetWorld()->SpawnActor<ANodeBluePrintScript>(Node, Location, FRotator::ZeroRotator, SpawnParams);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("After CreateNode"));
}


