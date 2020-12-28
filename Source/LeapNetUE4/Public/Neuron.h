// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NN_Structs.h"

class LEAPNETUE4_API Neuron
{
public:
	Neuron(FNeuron &data, bool randomiseWeights, int neuronNumber);
	~Neuron();

	FNeuron neuronData;
	int neuronNum;

	float GetActivatedValue(float theta);

	float GetDerivedValue(float theta);

	void UpdateNeuronData(FNeuron &data, bool randomiseWeights);



};
