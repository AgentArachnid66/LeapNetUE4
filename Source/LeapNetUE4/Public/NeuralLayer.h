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
	NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights, int layerNumber);
	~NeuralLayer();

	FNeuralLayer layerData;
	vector<Neuron> neurons;
	int layerNum;

	void FeedForward(NeuralLayer &nextLayer, float theta, FNeuralLayer &Topology);

	void BackPropagate(NeuralLayer &prevLayer, float alpha, TArray<float> target, FNeuralLayer &prevLayerData);

	void UpdateNeurons(FNeuralLayer &layerInfo, bool randomiseWeights);

	void SetInputValues(TArray<float> inputs);
};
