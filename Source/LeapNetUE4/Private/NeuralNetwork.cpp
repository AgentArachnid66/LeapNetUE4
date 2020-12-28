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
		neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->randomiseWeights, layer));
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
		neuralLayers.at(layer).BackPropagate(neuralLayers.at(layer - 1), this->alpha, inputs, this->Topology[layer-1]);

	}
	
}


// Called every frame
void UNeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Loads the topology from a struct that can be saved to a save game object.
void UNeuralNetwork::LoadTopology(FString customTopologyName, FString customSlotName) {
	FString test;

	UE_LOG(LogTemp, Warning, TEXT("Cleared Topology"));
	neuralLayers.clear();
	// Need to load the save game itself before accessing the variables
	if (UTopologies* LoadedGame = Cast<UTopologies>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.

		test = LoadedGame->Topologies.Num() > 0 ? LoadedGame->Topologies[0].topologyName : "No Topologies in Save";
		UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *test);
		this->topologies = LoadedGame;

		
	}
	

	
	// Checks if the topology named exists in the save game
	if (topologies->DoesTopologyExist(customTopologyName)) {
		// Gets the topology data from the save game given the name of the topology
		FTopology active = topologies->GetTopologyFromName(customTopologyName);

		UE_LOG(LogTemp, Warning, TEXT("Loading Topology"));
		this->Topology = active.Topology;

		for (int layer = 0; layer < this->Topology.Num(); layer++) {
			test = FString::FromInt(layer);
			UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
			neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->randomiseWeights, layer));
		}
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Topology does not exist"));
	}
}

void UNeuralNetwork::LoadTopologyDefault()
{
	LoadTopology(this->topologyName, this->slotName);
}

void UNeuralNetwork::SaveCurrentTopologyDefault()
{
	SaveCurrentTopology(this->topologyName);
}

void UNeuralNetwork::SaveCurrentTopology(FString customtopologyName) {
	FString test;

	// Get reference to save game
	if (UTopologies* SaveGameInstance = Cast<UTopologies>(UGameplayStatics::CreateSaveGameObject(UTopologies::StaticClass())))
	{
		// Set data on the savegame object.

		// Check if name exists in topology
		if (SaveGameInstance->DoesTopologyExist(customtopologyName)) {
			UE_LOG(LogTemp, Warning, TEXT("Topology Name exists in save, overwriting"));

			// Overwrite topology if it does
			SaveGameInstance->Topologies[SaveGameInstance->GetTopologyIndexFromName(customtopologyName)].Topology = this->Topology;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Topology Name is new, adding to save"));


			test = FString::FromInt(SaveGameInstance->Topologies.Num());
			UE_LOG(LogTemp, Warning, TEXT("Number of Topologies in Before Save: %s"), *test);

			// Add current topology to array if it doesn't
			SaveGameInstance->Topologies.Add(FTopology(customtopologyName, this->Topology));

			test = FString::FromInt(SaveGameInstance->Topologies.Num());
			UE_LOG(LogTemp, Warning, TEXT("Number of Topologies in Save: %s"), *test);
		}

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, this->slotName, 0))
		{
			UE_LOG(LogTemp,Warning,TEXT("Attempt to Save Topology was successful"))
		}

		test = SaveGameInstance->Topologies.Num() > 0 ? SaveGameInstance->Topologies[0].topologyName : "No Topologies in Save";
		UE_LOG(LogTemp, Warning, TEXT("SAVED: %s"), *test);
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
			neuralLayers.at(layer).FeedForward(this->neuralLayers.at(layer + 1), theta, this->Topology[layer]);
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
	// This work so the variable I need to change to update the UI is this variable (this->Topology)
	this->Topology.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Topology Emptied"));
}
