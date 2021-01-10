// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NN_Structs.h"
#include <vector>
#include "Neuron.h"
/**
 * 
 */

using namespace std;
class LEAPNETUE4_API NeuralLayer
{
public:

	/**
	* Constructor for a NeuralLayer Object
	* @param &layerInfo - Reference to the new Layer's data, which contains the new neuron data for each neuron
	* @param bRandomiseWeights - Boolean to control whether or not this layer needs new random weights
	* @param layerNumber - The index of this layer in the Topology
	* @return NeuralLayer
	*/
	NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights, int layerNumber);
	~NeuralLayer();

	/** This Layer's data */
	FNeuralLayer layerData;

	/** The neurons that are contained within this Layer */
	vector<Neuron> neurons;

	/** The index of this Layer in the Topology*/
	int layerNum;


	/** 
	* Feeds the values from this layer into the next one
	* @param &nextLayer - A reference to the next layer in the network so this layer can access those neurons. MUST BE STOPPED BEFORE LAST LAYER TRIES TO FEED TO LAYER THAT DOESN'T EXIST
	* @param theta - Threshold value for the neuron's activation and derivation functions
	* @param &Topology - Reference to the data structure that is used for saving and loading so it's updated when needed
	*/
	void FeedForward(NeuralLayer &nextLayer, float theta, FNeuralLayer &Topology);

	/**
	* Propagates the errors back through the network, providing the back bone for the learning and training of the network
	* @param prevLayer - Reference to the previous layer so this layer can access those neurons. MUST BE STOPPED BEFORE FIRST LAYER TRIES TO ACCESS LAYER THAT DOESN'T EXIST
	* @param alpha - Learning rate of the network
	* @param target - What the target was for this round of training
	* @param &prevLayerData - Reference to the previous layer's data structure so that can be updated when needed
	*/
	void BackPropagate(NeuralLayer &prevLayer, float alpha, float theta, FNeuralLayer &prevLayerData);

	/**
	* Easy way to update the neuron data on this layer when needed
	* @param &layerInfo - Reference to the new layer data, which contains the new neuron data for each neuron
	* @param bRandomiseWeights - Boolean to control whether or not the update needs new random weights
	*/
	void UpdateNeurons(FNeuralLayer &layerInfo, bool randomiseWeights);

	/**
	* Sets the input values on this layer. Advised to only use this on the first layer and then use feedforward to propagate. Also make sure that the length of the input array and the size of the layer match.
	* @param inputs - The new values of the neurons on this layer, with neuron[0] being set to input[0], neuron[n] being set to input[n]
	*/
	void SetInputValues(TArray<float> inputs);
};
