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
}

Neuron::Neuron(FNeuron &data, bool randomiseWeights)
{
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

}

Neuron::~Neuron()
{
}

float Neuron::GetActivatedValue(float theta) {
	float x = this->neuronData.value - theta;
	return 1 / (1 + exp(-x));
	//return this->neuronData.value >= theta ? 1 : 0;
}

float Neuron::GetDerivedValue(float theta) {
	float Y = GetActivatedValue(theta);
	return Y * (1 - Y);
}