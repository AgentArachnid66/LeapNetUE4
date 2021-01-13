#pragma once
#include "NN_Structs.generated.h"

UENUM(BlueprintType)
enum NeuronType {
	Input,
	Hidden,
	Output
};


USTRUCT(BlueprintType)
struct FSynapse {
	GENERATED_USTRUCT_BODY()

	FSynapse(){}

	FSynapse(float synapseWeight, int outputNeuron) {
		weight = synapseWeight;
		output = outputNeuron;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int output;
};


USTRUCT(BlueprintType)
struct FNeuron {

	GENERATED_USTRUCT_BODY()

	FNeuron() {	}

	FNeuron(TArray<FSynapse> synapses, float neuronValue, float neuronError, float thresholdValue) {
		Connections = synapses;
		value = neuronValue;
		error = neuronError;
		theta = thresholdValue;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSynapse> Connections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float theta;

	NeuronType type;
	float error;
	float sumErrorWeights;
};

USTRUCT(BlueprintType)
struct FNeuralLayer {

	GENERATED_USTRUCT_BODY()

	// Default Constructor
	FNeuralLayer() {
	}

	/**
	* Constructor
	* @param neurons - Neurons in the layer
	*/
	FNeuralLayer(TArray<FNeuron> neurons) {
		NeuronsInLayer = neurons;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNeuron> NeuronsInLayer;
};

USTRUCT(BlueprintType)
struct FTopology {

	GENERATED_USTRUCT_BODY()

	// Got these variables so that I can load in different topologies for different purposes
	// Makes it easier to train and finetune neural networks as all I need to do is add a new topology for 
	// a new purpose without having to retrain a massive neural network.

	FTopology() // Default Constructor.
	{
	}

	/**
	* Constructor of the FTopology struct. Useful for saving Topology data
	* @param n - The name of the Topology
	* @param t - The Topology data
	* @param th - Theta value (Threshold Value)
	* @param a - Alpha value (learning rate)
	* @return FTopology
	*/
	FTopology(FString n, TArray<FNeuralLayer> t, float th, float a) // Overloaded Constructor
	{
		topologyName = n;
		Topology = t;
		theta = th;
		alpha = a;
	}

	UPROPERTY()
		FString topologyName;

	UPROPERTY()
		TArray<FNeuralLayer> Topology;

	UPROPERTY()
		float theta;

	UPROPERTY()
		float alpha;
};