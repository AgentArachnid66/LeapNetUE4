// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralLayer.h"

NeuralLayer::NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights)
{
	this->layerData = layerInfo;
	FString test;
	for (int neuron = 0; neuron < layerData.NeuronsInLayer.Num(); neuron++) {
		this->neurons.push_back(Neuron(layerData.NeuronsInLayer[neuron], randomiseWeights));
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Added Neuron %s to layer"), *test);
	}
}

void NeuralLayer::UpdateNeurons(FNeuralLayer &layerInfo, bool randomiseWeights) {
	FString test;

	// Iterates through all neurons in the layer
	for (int neuron = 0; neuron < layerInfo.NeuronsInLayer.Num(); neuron++) {
		// Checks if neuron exists yet and adds if it isn't
		if (neuron > this->layerData.NeuronsInLayer.Num() - 1) {
			this->neurons.push_back(Neuron(layerInfo.NeuronsInLayer[neuron], randomiseWeights));
		}
		// Otherwise updates existing neuron
		else {
			neurons.at(neuron).UpdateNeuronData(neurons.at(neuron).neuronData, randomiseWeights);

		}
		// Need to add functionality to remove neurons when topology shrinks
	}

}

NeuralLayer::~NeuralLayer()
{
}

void NeuralLayer::FeedForward(NeuralLayer &nextLayer, float theta) {
	FString test;

	// Iterates through all neurons in this layer
	for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Neuron %s started forward propagation"), *test);

		// Iterates through all connections this neuron has with the neurons in the next layer
		for (int synapse = 0; synapse < this->neurons[neuron].neuronData.Connections.Num(); synapse++) {

			// local variable to save me writing this out everytime I want to reference the right thing :)
			int active = this->neurons[neuron].neuronData.Connections[synapse].output;

			// Next neuron's value is equal to the sum of all neurons connected to it's value * the weight 
			// of the connection from that neuron.
			if (neuron == 0) {
				nextLayer.neurons[active].neuronData.value = this->neurons[neuron].neuronData.value
					* this->neurons[neuron].neuronData.Connections[synapse].weight;
			}
			else {
				nextLayer.neurons[active].neuronData.value += this->neurons[neuron].neuronData.value
					* this->neurons[neuron].neuronData.Connections[synapse].weight;

			}
			// Test to see if the algorithm worked as expected until I get the UI working
			test = FString::SanitizeFloat(nextLayer.neurons[active].neuronData.value);
			UE_LOG(LogTemp, Warning, TEXT("X = %s"), *test);
			test = FString::SanitizeFloat(nextLayer.neurons[active].GetActivatedValue(theta));
			UE_LOG(LogTemp, Warning, TEXT("Y = %s"), *test);
		}

	}

}

void NeuralLayer::BackPropagate(NeuralLayer &prevLayer, float alpha, TArray<float> target) {
	FString test;

	test = FString::SanitizeFloat(alpha);
	UE_LOG(LogTemp, Warning, TEXT("Learning rate: %s"), *test);
	for (int neuron = 0; neuron < prevLayer.neurons.size(); neuron++) {
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Neuron %s Started Back Propagation"), *test);
		
		test = FString::FromInt(prevLayer.neurons.at(neuron).neuronData.Connections.Num());
		UE_LOG(LogTemp, Warning, TEXT("Neurons in layer: %s"), *test);

		
		// Calculates error for each neuron
		for (int synapse = 0; synapse < prevLayer.neurons.at(neuron).neuronData.Connections.Num(); synapse++) {

			int errorNeuron = prevLayer.neurons.at(neuron).neuronData.Connections[synapse].output;

			// If it is the first connection to check for this neuron then it is overwritten
			if (synapse == 0) {
				prevLayer.neurons.at(neuron).neuronData.error = this->neurons.at(errorNeuron).neuronData.error;
			}
			// else it adds to the current error
			else {
				prevLayer.neurons.at(neuron).neuronData.error += this->neurons.at(errorNeuron).neuronData.error;
			}
		}
		
		test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).neuronData.error);
		UE_LOG(LogTemp, Warning, TEXT("Neuron has error: %s"), *test);


		test = FString::FromInt(prevLayer.neurons.at(neuron).neuronData.Connections.Num());
		UE_LOG(LogTemp, Warning, TEXT("Neuron has %s connections"), *test);

				// Sets the new weights given the newly calculated error on a connection basis
		for (int synapse = 0; synapse < prevLayer.neurons.at(neuron).neuronData.Connections.Num(); synapse++) {

			// Gets output neuron index in an easier to read and write format
			int output = prevLayer.neurons.at(neuron).neuronData.Connections[synapse].output;


			// Gets the new weight
			float newWeight = prevLayer.neurons.at(neuron).neuronData.error * alpha * target[neuron];


			test = FString::FromInt(output);
			UE_LOG(LogTemp, Warning, TEXT("Current output neuron: %s"), *test);

			test = FString::SanitizeFloat(target[neuron]);
			UE_LOG(LogTemp, Warning, TEXT("Target value: %s"), *test);

			test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).neuronData.error);
			UE_LOG(LogTemp, Warning, TEXT("Output Neuron Error: %s"), *test);

			test = FString::SanitizeFloat(newWeight);
			UE_LOG(LogTemp, Warning, TEXT("Neuron's weight offset: %s"), *test);


			// Sets the new weight for the connection
			prevLayer.neurons[neuron].neuronData.Connections[synapse].weight += newWeight;

			test = FString::SanitizeFloat(prevLayer.neurons[neuron].neuronData.Connections[synapse].weight);
			UE_LOG(LogTemp, Warning, TEXT("Neuron's new Weight: %s"), *test);
		}
		
	}
}

// Mainly used for the input layer
void NeuralLayer::SetInputValues(TArray<float> inputs) {

	// Goes through the layer and sets the neuron values to a new float 
	for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
		this->neurons.at(neuron).neuronData.value = inputs[neuron];
	}

}