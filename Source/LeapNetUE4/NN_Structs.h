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