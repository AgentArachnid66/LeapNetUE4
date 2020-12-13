// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "CoreMinimal.h"
#include "NN_Structs.h"
#include "NeuralLayer.h"
#include "Components/ActorComponent.h"
#include "NeuralNetwork.generated.h"

using namespace std;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAPNETUE4_API UNeuralNetwork : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNeuralNetwork();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNeuralLayer> Topology;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool randomiseWeights;

	vector<NeuralLayer> neuralLayers;

};
