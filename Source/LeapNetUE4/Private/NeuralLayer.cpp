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

NeuralLayer::~NeuralLayer()
{
}

void NeuralLayer::FeedForward(NeuralLayer* nextLayer) {
	for(int neuron=0;neuron<this->neurons.size();neurons++){
		for (int synapse = 0; synapse < this->neurons[neuron].neuronData.Connections.Num(); synapse++) {
			int active = this->neurons[neuron].neuronData.Connections[synapse].output;
			nextLayer->neurons[active].neuronData.value = this->neurons[neuron].GetActivatedValue() * this->neurons[neuron].neuronData.Connections[synapse].weight;

			FString test = FString::SanitizeFloat(nextLayer->neurons[active].neuronData.value);
			UE_LOG(LogTemp, Warning, TEXT("Set Neuron Value to: %s"), *test);
		}
	}

}