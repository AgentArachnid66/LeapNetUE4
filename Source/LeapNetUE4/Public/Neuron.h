// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "CoreMinimal.h"
#include "NN_Structs.h"


class LEAPNETUE4_API Neuron
{
public:
	Neuron(FNeuron &data, bool randomiseWeights, int neuronNumber);
	~Neuron();

	/** The data that this neuron contains - connections, it's error and it's value */
	FNeuron neuronData;

	/**	The index of the neuron in it's parent layer */
	int neuronNum;


	/**
	* Gets the activated value of the neuron. It's a non-destructive way of achieving this as it doesn't change the value of the neuron
	* @return the activated value. In this case I'm using the Sigmoid function	
	*/
	float GetActivatedValue();

	/**
	* Gets the derivation of the value. Essential for the training of the neural network
	* @return the derived value. Since I'm using Sigmoid to find the activated value, I'll find the derivation of the Sigmoid function at the x value equal to the value of the neuron
	*/
	float GetDerivedValue();

	/**
	* Calculates the sum of the weights * the error. Essential for calculating delta weights, but only needs to be called once per neuron per backpropagation pass
	* @return The sum of the weights multiplied by the error
	*/
	float CalculateSumWeightsError(std::vector<Neuron> nextLayerNeurons);

	/**
	* Calculates the Error Term for this neuron. Essential for calculating delta weights
	*/
	float CalculateErrorTerms(std::vector<Neuron> nextLayerNeurons);

	/**
	* A quick way to update the neuron data without having to reinitialise the neuron
	* @param &data - A reference to the data to pass into the neuron to replace the current data
	* @param bRandomiseWeights - A Boolean to control whether or not the update should come with a new set of weights for the connections
	*/
	void UpdateNeuronData(FNeuron &data, bool randomiseWeights);



};
