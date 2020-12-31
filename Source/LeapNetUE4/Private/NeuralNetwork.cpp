// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetwork.h"
#include "LeapNetUE4.h"



// Sets default values for this component's properties
UNeuralNetwork::UNeuralNetwork()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

#pragma region Setup

// Called when the game starts
void UNeuralNetwork::BeginPlay()
{
	Super::BeginPlay();
	FString test;
	// ...
	if (this->topologies == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Save Game variable is null pointer"));

		if (UGameplayStatics::DoesSaveGameExist(this->slotName, 0)) {

			UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
			this->topologies = Cast<UTopologies>(UGameplayStatics::LoadGameFromSlot(this->slotName, 0));
		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
			this->topologies = Cast<UTopologies>(UGameplayStatics::CreateSaveGameObject(UTopologies::StaticClass()));
		}
	}

	neuralLayers.clear();
	// Set up the neural network
	for (int layer = 0; layer < this->Topology.Num(); layer++) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
		neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->bRandomiseWeights, layer));
	}


}

void UNeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

#pragma endregion

#pragma region Test and Train Network
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

#pragma endregion

#pragma region Saving and Loading

// Loads the topology from a struct that can be saved to a save game object.
void UNeuralNetwork::LoadTopology(FString customTopologyName, FString customSlotName) {
	FString test;
	if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

		
		this->topologies = Cast<UTopologies>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));

		// Test to make sure that the save is working
		test = this->topologies->bPrevSaved ? "True" : "False";
		UE_LOG(LogTemp, Warning, TEXT("Has been saved previously? %s"), *test);
		this->bPreviouslySaved = this->topologies->bPrevSaved;
		

		if (this->topologies->DoesTopologyExist(customTopologyName)) {
			// Retrieves the topology from the save game object
			this->Topology = this->topologies->GetTopologyFromName(customTopologyName).Topology;

			// Retrieves the theta and alpha values for the network
			this->theta = this->topologies->GetTopologyFromName(customTopologyName).theta;
			this->alpha = this->topologies->GetTopologyFromName(customTopologyName).alpha;

			// Clears the neural network ready for repopulation
			UE_LOG(LogTemp, Warning, TEXT("Cleared Topology"));
			neuralLayers.clear();

			// Set up the neural network
			UE_LOG(LogTemp, Warning, TEXT("Setting up loaded Topology"));
			for (int layer = 0; layer < this->Topology.Num(); layer++) {
				test = FString::FromInt(layer);
				UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
				neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->bRandomiseWeights, layer));
			}
		}

		else {
			UE_LOG(LogTemp, Warning, TEXT("Topology Name doesn't exist and couldn't be loaded"));
		}

	}
	else {
		UE_LOG(LogTemp, Warning,TEXT("No save game found in slot"));
	}
}

void UNeuralNetwork::SaveCurrentTopology(FString customTopologyName, FString customSlotName) {
	FString test;
	if (this->topologies == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Topology variable is null pointer"));

		if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

			UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
			this->topologies = Cast<UTopologies>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));
		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
			this->topologies = Cast<UTopologies>(UGameplayStatics::CreateSaveGameObject(UTopologies::StaticClass()));
		}
	}
	if (this->topologies->IsValidLowLevel()) {

		this->topologies->bPrevSaved = true;

		UE_LOG(LogTemp, Warning, TEXT("Topology %s added"), *customTopologyName);

		if (this->topologies->DoesTopologyExist(customTopologyName)) {
			// If the topology name already exists in the save file, overwrite it
			this->topologies->Topologies[this->topologies->GetTopologyIndexFromName(customTopologyName)].Topology = this->Topology;
			UE_LOG(LogTemp, Warning, TEXT("Topology name already exists, overwriting save file"));

			// Saves the theta and alpha values of the network for it to work properly
			this->topologies->Topologies[this->topologies->GetTopologyIndexFromName(customTopologyName)].alpha = this->alpha;
			this->topologies->Topologies[this->topologies->GetTopologyIndexFromName(customTopologyName)].theta = this->theta;
		}
		else {
			// If it is a new topology name, then add it as a new item
			this->topologies->Topologies.Add(FTopology(customTopologyName, this->Topology, this->theta, this->alpha));
			UE_LOG(LogTemp, Warning, TEXT("Topology name is new, adding to save file"));
		}



		// Saves the game to the slot
		UGameplayStatics::SaveGameToSlot(this->topologies, customSlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Game saved"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid Save Game Object"));
	}

}

void UNeuralNetwork::LoadTopologyDefault()
{
	LoadTopology(this->topologyName, this->slotName);
}

void UNeuralNetwork::SaveCurrentTopologyDefault()
{
	SaveCurrentTopology(this->topologyName, this->slotName);
}

#pragma endregion

