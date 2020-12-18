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

	// Set up the neural network
	for (int layer = 0; layer < this->Topology.Num(); layer++) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
		neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->randomiseWeights));
	}


}


void UNeuralNetwork::Train(TArray<float> inputs, TArray<float> targets ) {
	FString test;
	// Start the Feed Forward

	// Made a separate function for when it comes to use the neural network
	FeedForward(inputs);

	// Start the Back Propagation

	// Calculate Error for the output layer
	// error = target - output
	for (int output = 0; output < this->neuralLayers.back().neurons.size(); output++) {
		Neuron &active = neuralLayers.back().neurons.at(output);
		active.neuronData.error = targets[output] - active.GetActivatedValue(theta);

		test = FString::SanitizeFloat(neuralLayers.back().neurons.at(output).neuronData.error);
		UE_LOG(LogTemp, Warning, TEXT("Neuron has error of: %s"), *test);


	}
	test = FString::FromInt(this->Topology.Num() - 1);
	UE_LOG(LogTemp, Warning, TEXT("Number of layers: %s"), *test);


	
	// Propagates the error backwards throughout the network
	for (int layer = this->Topology.Num() - 1; layer > 0; layer--) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Layer %s Started Back Propagation"), *test);
		neuralLayers.at(layer).BackPropagate(neuralLayers.at(layer - 1), this->alpha, inputs);

	}
	
}


// Called every frame
void UNeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Loads the topology from a struct that can be saved to a save game object.
void UNeuralNetwork::LoadTopology(TArray<FNeuralLayer> newTopology) {
	FString test;
	// Iterates through the topology
	for (int layer = 0; layer < newTopology.Num(); layer++) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);

		// Checks if the new topology is greater than the current one and if so then adds a new layer
		if (layer > this->Topology.Num() - 1) {
			neuralLayers.push_back(NeuralLayer(newTopology[layer], this->randomiseWeights));
		}

		// Otherwise it will just update the existing layer
		else {
			neuralLayers.at(layer).UpdateNeurons(newTopology[layer], this->randomiseWeights);
		}

		// Need to add a check to see if the new topology is smaller
	}
}


void UNeuralNetwork::FeedForward(TArray<float> input)
{
	FString test;
	// Feed forward Loop
	for (int layer = 0; layer < this->Topology.Num(); layer++) {

		// Looks for the input layer
		if (layer == 0) {
			// Sets the input values accordingly
			neuralLayers.at(layer).SetInputValues(input);
		}

		// Looks for input and hidden layers
		if (layer < this->Topology.Num() - 1) {
			test = FString::FromInt(layer);
			UE_LOG(LogTemp, Warning, TEXT("Fed through Layer: %s"), *test);

			// Feeds the input through the network
			neuralLayers.at(layer).FeedForward(this->neuralLayers.at(layer + 1),theta);
		}

		// Looks for the output layer
		else {
			test = FString::SanitizeFloat(neuralLayers.at(layer).neurons[0].GetActivatedValue(theta));
			UE_LOG(LogTemp, Warning, TEXT("Final Value: %s"), *test);
		}

	}
}

// Will be used for testing and general utility as I want to see the changes in the editor
void UNeuralNetwork::UpdateUI()
{
	this->Topology.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Topology Emptied"));
}
