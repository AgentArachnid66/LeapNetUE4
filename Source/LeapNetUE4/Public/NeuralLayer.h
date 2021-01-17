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
	NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights, int layerNumber, ActivationFunction active);
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
	* @param bEnableBias - Boolean to check if the functions should take into account the existence of a bias neuron
	* @return Whether the feed forward for this layer was successful
	*/
	bool FeedForward(NeuralLayer &nextLayer, FNeuralLayer &Topology, bool bEnableBias);

	/**
	* Propagates the errors back through the network, providing the back bone for the learning and training of the network
	* @param prevLayer - Reference to the previous layer so this layer can access those neurons. MUST BE STOPPED BEFORE FIRST LAYER TRIES TO ACCESS LAYER THAT DOESN'T EXIST
	* @param alpha - Learning rate of the network
	* @param target - What the target was for this round of training
	* @param &prevLayerData - Reference to the previous layer's data structure so that can be updated when needed
	* @param &currentLayerData - Reference to the current layer's data structure so it can be updated when needed
	* @param bEnableBias - Boolean to check to see if the function should take in the existence of a bias neuron into account
	*/
	void BackPropagate(NeuralLayer &prevLayer, float alpha, FNeuralLayer &prevLayerData,FNeuralLayer &currentLayerData,bool bEnableBias);

	/**
	* Easy way to update the neuron data on this layer when needed
	* @param &layerInfo - Reference to the new layer data, which contains the new neuron data for each neuron
	* @param bRandomiseWeights - Boolean to control whether or not the update needs new random weights
	*/
	void UpdateNeurons(FNeuralLayer &layerInfo, bool randomiseWeights);

	/**
	* Sets the input values on this layer. Advised to only use this on the first layer and then use feedforward to propagate. Also make sure that the length of the input array and the size of the layer match.
	* @param inputs - The new values of the neurons on this layer, with neuron[0] being set to input[0], neuron[n] being set to input[n]
	* @param bEnableBias - Boolean to ensure an easy way to ensure that the bias is excluded or included. Useful for testing. True for the bias to be skipped when setting the values and false for it to be overwritten and treated like a usual neuron 
	* @return Whether the input values were set correctly
	*/
	bool SetInputValues(TArray<float> inputs, bool bEnableBias);

	/**
	* Sets the Activation Function for each neuron in this layer
	* @param active - The new active activation function
	*/
	void SetActivation(ActivationFunction active);
};
