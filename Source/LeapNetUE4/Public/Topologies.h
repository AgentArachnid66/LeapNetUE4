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

	// Constructor
	UTopologies();

	// Array of Topology Data that means that multiple topologies can be added to this save game, useful for organisation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTopology> Topologies;

	// Test boolean to ensure that the save game object is working
	UPROPERTY()
		bool bPrevSaved;




	UFUNCTION()
		/**
	 * Given a name, return the topology from the save game. Will return the first index if none match so should be used in tandem with DoesTopologyExist in an if statement.
	 * @param topologyName - The name of the Topology to return
	 * @return A Struct that holds both the name of the topology but also all necessary data to reconstruct it 
	    */
		FTopology GetTopologyFromName(FString topologyName);

	UFUNCTION()
	/** Checks if there is a topology given a name. Mostly used to ensure no errors occur in the use of the system
	*   @param topologyName - The name of the Topology to check for
	*   @return Bool that is whether or not that topology exists in this save game
	*/
	bool DoesTopologyExist(FString topologyName);

	UFUNCTION()
	/**
	* Gets the index of a topology given the name
	* @param topologyName - The name of the topology to get the index of
	* @return The index of the topology given the name.
	*/
	int GetTopologyIndexFromName(FString topologyName);
};

