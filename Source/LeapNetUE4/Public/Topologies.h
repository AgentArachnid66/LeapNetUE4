// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NN_Structs.h"
#include "Topologies.generated.h"

/**
 * 
 */



UCLASS()
class LEAPNETUE4_API UTopologies : public USaveGame
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTopology> Topologies;

	UFUNCTION()
		FTopology GetTopologyFromName(FString topologyName);

	bool DoesTopologyExist(FString topologyName);

	int GetTopologyIndexFromName(FString topologyName);
};

