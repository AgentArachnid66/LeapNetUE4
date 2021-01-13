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

bool NeuralLayer::FeedForward(NeuralLayer &nextLayer, FNeuralLayer &Topology) {
	FString test;
	bool success = true;
	vector<bool> visitedNeurons(nextLayer.neurons.size(), false);

	// Iterates through all neurons in this layer
	for (int neuron = 0; neuron < this->neurons.size(); neuron++) {
		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("Neuron %s started forward propagation"), *test);



		// Iterates through all connections this neuron has with the neurons in the next layer
		for (int synapse = 0; synapse < this->neurons[neuron].neuronData.Connections.Num(); synapse++) {



			// local variable to save me writing this out everytime I want to reference the right thing :)
			int active = this->neurons[neuron].neuronData.Connections[synapse].output;

			// Checks to see if the layer is the input and if so, skip the activation and if it's the hidden layer then it's the activated
			float value = this->layerNum >= 1 ? this->neurons[neuron].GetActivatedValue() : this->neurons[neuron].neuronData.value;

			// Next neuron's value is equal to the sum of all neurons connected to it's value * the weight 
			// of the connection from that neuron.

			// Checks if this is the first time we are looking at this neuron
			if (!visitedNeurons.at(active)) {

				
				// Overwrites the neuron value
				nextLayer.neurons[active].neuronData.value = value
					* this->neurons[neuron].neuronData.Connections[synapse].weight;
				
				test = FString::SanitizeFloat(value);
				UE_LOG(LogTemp, Warning, TEXT("Neuron Value: %s"), *test);

				test = FString::SanitizeFloat(this->neurons[neuron].neuronData.Connections[synapse].weight);
				UE_LOG(LogTemp, Warning, TEXT("Neuron Weight: %s"), *test);

				test = FString::SanitizeFloat(nextLayer.neurons[active].neuronData.value);
				UE_LOG(LogTemp, Warning, TEXT("First input: %s"), *test);

				// Marks neuron as visited
				visitedNeurons.at(active) = true;
			}
			// Otherwise we continue the sum
			else {
				nextLayer.neurons[active].neuronData.value += value
					* this->neurons[neuron].neuronData.Connections[synapse].weight;

				test = FString::SanitizeFloat(value);
				UE_LOG(LogTemp, Warning, TEXT("Neuron Value: %s"), *test);

				test = FString::SanitizeFloat(this->neurons[neuron].neuronData.Connections[synapse].weight);
				UE_LOG(LogTemp, Warning, TEXT("Neuron Weight: %s"), *test);

				test = FString::SanitizeFloat(nextLayer.neurons[active].neuronData.value);
				UE_LOG(LogTemp, Warning, TEXT("Sum so far: %s"), *test);
			}
		}

		
		// This should update the UI to reflect the actual value of the neuron. If this works then I'll do the same for the 
		// back propagation for the weights.
		Topology.NeuronsInLayer[neuron].value = this->neurons.at(neuron).neuronData.value;

	}

	return success;

}

void NeuralLayer::BackPropagate(NeuralLayer &prevLayer, float alpha, FNeuralLayer &prevLayerData) {
	FString test;


	test = FString::FromInt(prevLayer.layerNum);

	UE_LOG(LogTemp, Warning, TEXT("############## Layer %s ###########"), *test);


	vector<bool> visited(neurons.size(), false);

	for (int neuron = 0; neuron < prevLayer.neurons.size(); neuron++) {

		test = FString::FromInt(neuron);
		UE_LOG(LogTemp, Warning, TEXT("############## Neuron %s ###########"), *test);

		float theta = prevLayer.neurons.at(neuron).neuronData.theta;

		prevLayer.neurons.at(neuron).CalculateErrorTerms(this->neurons);

		// Now we know the error term of this neuron we can work out how to change the weights

		// Changes the weights for each neuron
		for (int synapse = 0; synapse < prevLayer.neurons.at(neuron).neuronData.Connections.Num(); synapse++) {

			// Gets the connected neuron
			int errorNeuron = prevLayer.neurons.at(neuron).neuronData.Connections[synapse].output;

			test = FString::FromInt(errorNeuron);

			UE_LOG(LogTemp, Warning, TEXT("############## Connected Neuron Index %s ###########"), *test);


			// Safety check to make sure that the neuron that this is outputting to exists
			if (errorNeuron < this->neurons.size()) {


				// Calculates the connections new weight

					// delta weight = 
					// learning rate * 
					// input * 
					// error term

				float newWeight = alpha *
					prevLayer.neurons.at(neuron).GetActivatedValue() *
					this->neurons.at(errorNeuron).neuronData.sumErrorWeights;

				// Debug Messages
				test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).GetDerivedValue());
				UE_LOG(LogTemp, Warning, TEXT("Derived Value: %s"), *test);

				test = FString::SanitizeFloat(prevLayer.neurons.at(neuron).GetActivatedValue());
				UE_LOG(LogTemp, Warning, TEXT("Activated Value: %s"), *test);

				test = FString::SanitizeFloat(this->neurons.at(errorNeuron).neuronData.sumErrorWeights);
				UE_LOG(LogTemp, Warning, TEXT("Sum of Error and Weights: %s"), *test);

				test = FString::SanitizeFloat(newWeight);
				UE_LOG(LogTemp, Warning, TEXT("Delta Weight: %s"), *test);

				// Apply Delta Weight
				prevLayer.neurons.at(neuron).neuronData.Connections[synapse].weight += newWeight;


				// Only calculates the delta theta once per neuron per layer
				// Better than iterating through the layer again.
				if (!visited.at(errorNeuron)) {
					// Calculate delta theta
					// = alpha * error term of neuron

					// Debug Messages
					test = FString::SanitizeFloat(this->neurons.at(errorNeuron).neuronData.error);
					UE_LOG(LogTemp, Warning, TEXT("Error term for delta theta calculation: %s"), *test);

					test = FString::SanitizeFloat(-1.f*alpha*this->neurons.at(errorNeuron).neuronData.error);
					UE_LOG(LogTemp, Warning, TEXT("Delta Theta: %s"), *test);

					// Apply Delta Theta
					this->neurons.at(errorNeuron).neuronData.theta +=
						-1.f *
						alpha *
						this->neurons.at(errorNeuron).neuronData.sumErrorWeights;

					// Outputs the new theta value
					test = FString::SanitizeFloat(this->neurons.at(errorNeuron).neuronData.theta);
					UE_LOG(LogTemp, Warning, TEXT("New Theta: %s"), *test);



					visited.at(errorNeuron) = true;
				}
			}

			else {
				UE_LOG(LogTemp, Warning, TEXT("Output Neuron doesn't exist"));
			}
		}

		// Updates UI
		prevLayerData.NeuronsInLayer[neuron] = prevLayer.neurons.at(neuron).neuronData;
	}
}

// Mainly used for the input layer
bool NeuralLayer::SetInputValues(TArray<float> inputs, bool bEnableBias) {

	FString test = FString::FromInt(inputs.Num());
	UE_LOG(LogTemp, Warning, TEXT("Number of elements in input: %s"), *test);

	// Checks if the number of inputs matches the number of input neurons minus the bias neuron
	if (inputs.Num() <= this->neurons.size()) {

		// Goes through the layer and sets the neuron values to a new float, 
		// but takes into account the bias neuron if one exists
		for (int neuron = 0; neuron < (this->neurons.size() ); neuron++) {

			// Made the function less reliant on the existence of a bias node
			// by checking if the 
			if (bEnableBias) {
				UE_LOG(LogTemp, Warning, TEXT("Skipping Bias Node"));
			}
			else {
				this->neurons.at(neuron).neuronData.value = inputs[neuron];
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Input layer values were set successfully"));
		return true;
	}
	else {
		test = FString::FromInt(this->neurons.size());
		UE_LOG(LogTemp, Warning, TEXT("Input array is bigger than Input Layer array: %s"), *test);
		return false;
	}

}