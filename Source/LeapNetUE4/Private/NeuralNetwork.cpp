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
	
}

void UNeuralNetwork::Initialise() {
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

	// If the user wants to make a basic topology, then this is where it happens 
	// It needs to convert the TArray of ints to a TArray of FNeuralLayer structs 
	// Conditions are if the baseTopology has elements and that none of the elements are 0
	if ((this->baseTopology.Num() > 0) & (!this->baseTopology.Contains(0))) {
		this->Topology.Empty();
		// Iterates through all the layers
		for (int layer = 0; layer < this->baseTopology.Num(); layer++) {
			TArray<FNeuron> neuronsInLayer;
			// Iterates through all the neurons in the layer. It makes sure it is positive by getting the absolute
			for (int neuron = 0; neuron < FMath::Abs(this->baseTopology[layer]); neuron++) {
				TArray<FSynapse> connections;
				// Iterates through all the output neurons to get the connections
				if (layer < this->baseTopology.Num() - 1) {

					for (int output = 0; output < this->baseTopology[layer + 1]; output++) {
						connections.Add(FSynapse(0, output));
					}
				}
				neuronsInLayer.Add(FNeuron(connections, 0, 0, this->theta));
			}

			if ((layer < this->baseTopology.Num() - 1)&(bEnableBias)) {

				// Adds Bias neuron separately to hidden layers
				UE_LOG(LogTemp, Warning, TEXT("Added Bias Neuron"));

				neuronsInLayer.Add(FNeuron(neuronsInLayer.Last().Connections, 1, 0, this->theta));
			}
			this->Topology.Add(FNeuralLayer(neuronsInLayer));
		}

	}

	// Either uses the newly generated data or the user defined topology to set up the topology
	for (int layer = 0; layer < this->Topology.Num(); layer++) {
		test = FString::FromInt(layer);
		UE_LOG(LogTemp, Warning, TEXT("Added Layer: %s"), *test);
		neuralLayers.push_back(NeuralLayer(this->Topology[layer], this->bRandomiseWeights, layer));
	}

	UE_LOG(LogTemp, Warning, TEXT("Finished Neural Network Setup"));
}

void UNeuralNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

#pragma endregion

#pragma region Test and Train Network
bool UNeuralNetwork::Train(TArray<float> inputs, TArray<float> targets ) {
	FString test;
	// Start the Feed Forward

	// Made a separate function for when it comes to use the neural network
	// If the feed forward was successful then it can continue the training
	if (FeedForward(inputs)) {


		// Start the Back Propagation

		// Calculate Total Error for the output layer
		// Total Error = sum of 1/2*(target - output)^2

		// Calculate individual error terms
		// Error Terms = derivation of sigmoid * (target - output)
		UE_LOG(LogTemp,Warning,TEXT("############## Output Layer ###########"))

		float errorTotal = 0;
		for (int output = 0; output < this->neuralLayers.back().neurons.size(); output++) {

			test = FString::FromInt(output);
			UE_LOG(LogTemp, Warning, TEXT("############## Neuron %s ###########"), *test);

			if (targets.Num() > this->neuralLayers.back().neurons.size()) {
				UE_LOG(LogTemp, Warning, TEXT("Target array is larger than output layer "));
				return false;
			}
			
			else {
				neuralLayers.back().neurons.at(output).neuronData.error = targets[output] - neuralLayers.back().neurons.at(output).GetActivatedValue();
				neuralLayers.back().neurons.at(output).neuronData.error *= neuralLayers.back().neurons.at(output).GetDerivedValue();
				
				errorTotal += 0.5f * FMath::Pow((targets[output] - neuralLayers.back().neurons.at(output).GetActivatedValue()), 2.f);


				test = FString::SanitizeFloat(neuralLayers.back().neurons.at(output).neuronData.error);
				UE_LOG(LogTemp, Warning, TEXT("Neuron has error of: %s"), *test);

				test = FString::SanitizeFloat(targets[output]);
				UE_LOG(LogTemp, Warning, TEXT("Target for this neuron: %s"), *test);

				test = FString::SanitizeFloat(neuralLayers.back().neurons.at(output).GetActivatedValue());
				UE_LOG(LogTemp, Warning, TEXT("Neuron's Output Value: %s"), *test);


				neuralLayers.back().neurons.at(output).neuronData.sumErrorWeights = neuralLayers.back().neurons.at(output).neuronData.error;
			}
		}

		test = FString::SanitizeFloat(errorTotal);
		UE_LOG(LogTemp, Warning, TEXT("Total Error: %s"), *test);



		// Propagates the error backwards throughout the network
		for (int layer = this->Topology.Num() - 1; layer > 0; layer--) {
			test = FString::FromInt(layer);
			UE_LOG(LogTemp, Warning, TEXT("Layer %s Started Back Propagation"), *test);
			neuralLayers.at(layer).BackPropagate(neuralLayers.at(layer - 1), this->alpha, this->Topology[layer - 1]);
		}
		return true;
	}
	else {
		return false;
	}
}

bool UNeuralNetwork::TrainIndex(TArray<float> inputs, int targetIndex)
{
	// If the target index is valid then it can be used for training
	if (targetIndex < neuralLayers.back().neurons.size()) {
		TArray<float> targets;
		targets.Init(0, neuralLayers.back().neurons.size());
		targets[targetIndex] = 1.f;
		return Train(inputs, targets);
	}
	// Else it just prints an error message to the log
	else {
		UE_LOG(LogTemp, Warning, TEXT("Target Index was Invalid"));
		return false;
	}

}

bool UNeuralNetwork::FeedForward(TArray<float> input)
{
	bool success = false;
	FString test;
	// Feed forward Loop
	for (int layer = 0; layer < this->Topology.Num(); layer++) {

		// Looks for the input layer
		if (layer == 0) {
			// Sets the input values accordingly and if was successful then the feed forward can continue
			success = neuralLayers.at(layer).SetInputValues(input, this->bEnableBias);
		}

		// Checks to make sure that the input layers have the correct values
		if (success) {
			// Looks for input and hidden layers
			if (layer < this->Topology.Num() - 1) {
				test = FString::FromInt(layer);
				UE_LOG(LogTemp, Warning, TEXT("Fed through Layer: %s"), *test);

				// Feeds the input through the network
				success = neuralLayers.at(layer).FeedForward(this->neuralLayers.at(layer + 1), this->Topology[layer]);
			}

			// Looks for the output layer
			else {
				// Iterates through output layer neurons
				for (int outputNeuron = 0; outputNeuron < neuralLayers.at(layer).neurons.size(); outputNeuron++) {
					test = FString::SanitizeFloat(neuralLayers.at(layer).neurons[outputNeuron].GetActivatedValue());
					UE_LOG(LogTemp, Warning, TEXT("Final Value: %s"), *test);
				}
			}
		}

		else {
			UE_LOG(LogTemp, Warning, TEXT("Feed Forward couldn't be performed"));
			return false;
		}
	}

	return true;


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


	if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

		UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
		this->topologies = Cast<UTopologies>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
		this->topologies = Cast<UTopologies>(UGameplayStatics::CreateSaveGameObject(UTopologies::StaticClass()));
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

void UNeuralNetwork::LoadLeapData(FString customSlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

		ULeapNeuralData* LeapData = Cast<ULeapNeuralData>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));

		// Retrieves the topology from the save game object
		this->trainingData = LeapData->frameData;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No save game found in slot"));
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

