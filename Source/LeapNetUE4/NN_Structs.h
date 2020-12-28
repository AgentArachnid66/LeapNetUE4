#pragma once
#include "NN_Structs.generated.h"

USTRUCT(BlueprintType)
struct FSynapse {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int output;
};


USTRUCT(BlueprintType)
struct FNeuron {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSynapse> Connections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float value;
	float error;
};

USTRUCT(BlueprintType)
struct FNeuralLayer {

	GENERATED_USTRUCT_BODY()


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

	FTopology(FString n, TArray<FNeuralLayer> t) // Overloaded Constructor
	{
		topologyName = n;
		Topology = t;
	}


	FString topologyName;

	TArray<FNeuralLayer> Topology;
};