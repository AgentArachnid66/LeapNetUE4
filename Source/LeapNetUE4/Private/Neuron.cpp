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

	if (this->neuronData.activationFunction == ActivationFunction::TanhActivation) {
		UE_LOG(LogTemp, Warning, TEXT("Tanh Activation Function not implemented yet. Defaulting to Sigmoid"));
	}

	float x = this->neuronData.value - this->neuronData.theta;
	return 1 / (1 + exp(-x));
}

float Neuron::GetDerivedValue() {
	float Y = GetActivatedValue();

	// Debug Message to double check the maths
	FString test = FString::SanitizeFloat(Y);
	UE_LOG(LogTemp, Warning, TEXT("Activated Value: %s"), *test);

	return Y * (1 - Y);
}

float Neuron::CalculateSumWeightsError(std::vector<Neuron> nextLayerNeurons) {
	float returnValue = 0;
	FString test;
	for (int j = 0; j < this->neuronData.Connections.Num(); j++) {

		// Debug Messages

		test = FString::FromInt(this->neuronData.Connections[j].output);
		UE_LOG(LogTemp, Warning, TEXT("###### Output Connected Neuron %s ######"), *test);

		// Error term of the connected neuron in the next layer
		test = FString::SanitizeFloat(nextLayerNeurons.at(this->neuronData.Connections[j].output).neuronData.error);
		UE_LOG(LogTemp, Warning, TEXT("###### Error Term: %s ######"), *test);
	
		// Weight of the connection to that layer
		test = FString::SanitizeFloat(this->neuronData.Connections[j].weight);
		UE_LOG(LogTemp, Warning, TEXT("###### Weight of Connection: %s ######"), *test);

		// Adds onto sum
		returnValue +=
			nextLayerNeurons.at(this->neuronData.Connections[j].output).neuronData.error*
			this->neuronData.Connections[j].weight;
	}
	test = FString::SanitizeFloat(returnValue);
	UE_LOG(LogTemp, Warning, TEXT("Sum of Weights * Error from Calculation: %s"), *test);


	this->neuronData.sumErrorWeights = returnValue;
	return returnValue;
}

float Neuron::CalculateErrorTerms(std::vector<Neuron> nextLayerNeurons) {
	FString test = FString::FromInt(neuronNum);
	UE_LOG(LogTemp, Warning, TEXT("#### Calculating Error Term for Neuron %s ####"), *test);
	
	float errorTerm = CalculateSumWeightsError(nextLayerNeurons);
	switch (neuronData.type) {

		// Only needs this for Hidden Layer as Input doens't have an error gradient and Output has it's own calculation
	case NeuronType::Hidden:

		test = FString::SanitizeFloat(GetDerivedValue());
		UE_LOG(LogTemp, Warning, TEXT("Neuron is in a Hidden Layer. Derived Value is: %s"), *test);
	
		// Error Term = derivation of sigmoid * sum of (weight to next layer * error term in next layer)
		errorTerm *= GetDerivedValue();

		test = FString::SanitizeFloat(errorTerm);
		UE_LOG(LogTemp, Warning, TEXT("Error Term is: %s"), *test);
		break;

	}

	neuronData.error = errorTerm;
	return neuronData.error;
}