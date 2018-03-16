// Fill out your copyright notice in the Description page of Project Settings.

#include "NodeScript.h"


// Sets default values for this component's properties
UNodeScript::UNodeScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNodeScript::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNodeScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNodeScript::changeTextRender(FName t) {
	self->TextRenderComponent->SetText(t);
}