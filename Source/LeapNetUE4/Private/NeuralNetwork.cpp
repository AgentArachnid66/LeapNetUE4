// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetwork.h"

// Sets default values for this component's properties
UNeuralNetwork::UNeuralNetwork()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNeuralNetwork::BeginPlay()
{
	Super::BeginPlay();
	FString test;
	// ...
	for (int layer = 0; layer < this->Topology.Num(); layer++) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
		neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->randomiseWeights));
	}

	for (int layer = 0; layer < this->Topology.Num(); layer++) {
		if (layer < this->Topology.Num() - 1) {
			neuralLayers.at(layer).FeedForward(this->neuralLayers.at(layer + 1));
		}
	}
	

}


// Called every frame
void UNeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

