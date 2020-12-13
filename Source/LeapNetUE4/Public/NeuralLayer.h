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
	NeuralLayer(FNeuralLayer &layerInfo, bool randomiseWeights);
	~NeuralLayer();

	FNeuralLayer layerData;
	vector<Neuron> neurons;

	void FeedForward(NeuralLayer* nextLayer);

};
