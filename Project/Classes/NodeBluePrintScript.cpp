// Fill out your copyright notice in the Description page of Project Settings.

#include "NodeBluePrintScript.h"


// Sets default values for this component's properties
UNodeBluePrintScript::UNodeBluePrintScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNodeBluePrintScript::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNodeBluePrintScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}