// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralLayer.h"

NeuralLayer::NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights, int layerNumber)
{
	this->layerData = layerInfo;
	this->layerNum = layerNumber;
	FString test;
	for (int neuron = 0; neuron < layerData.NeuronsInLayer.Num(); neuron++) {
		this->neurons.push_back(Neuron(layerData.NeuronsInLayer[neuron], randomiseWeights, neuron));
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Added Neuron %s to layer"), *test);
		
		// Updates the UI
		layerInfo.NeuronsInLayer[neuron] = this->neurons.back().neuronData;
	}

	
}

void NeuralLayer::UpdateNeurons(FNeuralLayer &layerInfo, bool randomiseWeights) {
	FString test;

	// Iterates through all neurons in the layer
	for (int neuron = 0; neuron < layerInfo.NeuronsInLayer.Num(); neuron++) {
		// Checks if neuron exists yet and adds if it isn't
		if (neuron > this->layerData.NeuronsInLayer.Num() - 1) {
			this->neurons.push_back(Neuron(layerInfo.NeuronsInLayer[neuron], randomiseWeights, neuron));
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

void NeuralLayer::FeedForward(NeuralLayer &nextLayer, float theta, FNeuralLayer &Topology) {
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
		}

		
		// This should update the UI to reflect the actual value of the neuron. If this works then I'll do the same for the 
		// back propagation for the weights.
		Topology.NeuronsInLayer[neuron].value = this->neurons.at(neuron).neuronData.value;

	}

}

void NeuralLayer::BackPropagate(NeuralLayer &prevLayer, float alpha, float theta, FNeuralLayer &prevLayerData) {
	FString test;

	test = FString::SanitizeFloat(alpha);
	UE_LOG(LogTemp, Warning, TEXT("Learning rate: %s"), *test);
	for (int neuron = 0; neuron < prevLayer.neurons.size(); neuron++) {
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Neuron %s Started Back Propagation"), *test);
		
		test = FString::FromInt(prevLayer.neurons.at(neuron).neuronData.Connections.Num());
		UE_LOG(LogTemp, Warning, TEXT("Neurons in layer: %s"), *test);

		
		// Calculates error for each neuron in this layer, which we know isn't the output layer
		// Error Term = derivation of sigmoid * sum of (weight to next layer * error term in next layer)
		float errorTerm = prevLayer.neurons.at(neuron).CalculateSumWeightsError(this->neurons) *
			prevLayer.neurons.at(neuron).GetDerivedValue(theta);

		prevLayer.neurons.at(neuron).neuronData.error = errorTerm;
		// Now we know the error term of this neuron we can work out how to change the weights
		
			
		test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).neuronData.error);
		UE_LOG(LogTemp, Warning, TEXT("Neuron has error: %s"), *test);

		test = FString::FromInt(prevLayer.neurons.at(neuron).neuronData.Connections.Num());
		UE_LOG(LogTemp, Warning, TEXT("Neuron has %s connections"), *test);

		// Changes the weights for each neuron

		for (int synapse = 0; synapse < prevLayer.neurons.at(neuron).neuronData.Connections.Num(); synapse++) {
			

			// Gets the connected neuron
			int errorNeuron = prevLayer.neurons.at(neuron).neuronData.Connections[synapse].output;

			// Calculates the connections new weight

				// delta weight = 
				// -learning rate * 
				// Derivative of activation * 
				// input * 
				// sum of (weights * error term of next layer)

			float newWeight = alpha * prevLayer.neurons.at(neuron).GetDerivedValue(theta);
			newWeight *= prevLayer.neurons.at(neuron).neuronData.value;
			newWeight *= this->neurons.at(errorNeuron).neuronData.sumErrorWeights;
			
			// Debug Messages
			test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).GetDerivedValue(theta));
			UE_LOG(LogTemp, Warning, TEXT("Derived Value: %s"), *test);

			test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).neuronData.value);
			UE_LOG(LogTemp, Warning, TEXT("Value: %s"), *test);

			test = FString::SanitizeFloat(this->neurons.at(errorNeuron).neuronData.sumErrorWeights);
			UE_LOG(LogTemp, Warning, TEXT("Sum of Error and Weights: %s"), *test);

			test = FString::SanitizeFloat(newWeight);
			UE_LOG(LogTemp, Warning, TEXT("Delta Weight: %s"), *test);


			prevLayer.neurons.at(neuron).neuronData.Connections[synapse].weight -= newWeight;
		}

		// Updates UI
		prevLayerData.NeuronsInLayer[neuron] = prevLayer.neurons.at(neuron).neuronData;
	}
}

// Mainly used for the input layer
void NeuralLayer::SetInputValues(TArray<float> inputs) {

	// Goes through the layer and sets the neuron values to a new float 
	for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
		this->neurons.at(neuron).neuronData.value = inputs[neuron];
	}

}