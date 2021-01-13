// Fill out your copyright notice in the Description page of Project Settings.


#include "Neuron.h"

void Neuron::UpdateNeuronData(FNeuron &data, bool randomiseWeights) {
	this->neuronData = data;
	FString test;

	if (randomiseWeights) {
		for (int synapse = 0; synapse < data.Connections.Num(); synapse++) {
			float LO = 0;
			float HI = 1;
			float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			neuronData.Connections[synapse].weight = r3;
		}

	}
	for (int synapse = 0; synapse < data.Connections.Num(); synapse++) {

		float r3 = neuronData.Connections[synapse].weight;

		test = FString::SanitizeFloat(r3);
		UE_LOG(LogTemp, Warning, TEXT("Set Neuron to weight: %s"), *test);
	}
	// Updates the UI
	data = this->neuronData;
}

Neuron::Neuron(FNeuron &data, bool randomiseWeights, int neuronNumber)
{
	this->neuronData = data;
	this->neuronNum = neuronNumber;
	FString test;

	if (randomiseWeights) {
		for (int synapse = 0; synapse < data.Connections.Num(); synapse++) {
			float LO = 0;
			float HI = 1;
			float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			neuronData.Connections[synapse].weight = r3;
		}

	}
	for (int synapse = 0; synapse < data.Connections.Num(); synapse++) {

		float r3 = neuronData.Connections[synapse].weight;		
		test = FString::SanitizeFloat(r3);
		UE_LOG(LogTemp, Warning, TEXT("Set Neuron to weight: %s"), *test);

	}
	
}

Neuron::~Neuron()
{
}

float Neuron::GetActivatedValue() {
	float x = this->neuronData.value - this->neuronData.theta;
	return 1 / (1 + exp(-x));
}

float Neuron::GetDerivedValue() {
	float Y = GetActivatedValue();
	return Y * (1 - Y);
}

float Neuron::CalculateSumWeightsError(std::vector<Neuron> nextLayerNeurons) {
	float returnValue = 0;
	for (int j = 0; j < this->neuronData.Connections.Num(); j++) {
		returnValue +=
			nextLayerNeurons.at(this->neuronData.Connections[j].output).neuronData.error*
			this->neuronData.Connections[j].weight;
	}
	FString test = FString::SanitizeFloat(returnValue);
	UE_LOG(LogTemp, Warning, TEXT("Sum of weights * error: %s"), *test);
	this->neuronData.sumErrorWeights = returnValue;
	return returnValue;
}