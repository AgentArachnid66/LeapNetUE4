// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "CoreMinimal.h"
#include "NN_Structs.h"
#include "NeuralLayer.h"
#include "Topologies.h"
#include "LeapNeuralData.h"
#include "Kismet/GameplayStatics.h"
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
	
#pragma region UFUNCTIONS


	UFUNCTION(BlueprintCallable, CallInEditor)
		/**
		* Initialises the topology, separating it from BeginPlay to give more control over the initialisation itself.
		*/
		void Initialise();

		/**
		* Loads a topology. This is a more advanced way to save a topology as you have more options for slots and don't have to change the classes variable
		* @param topologyName - The name of the Topology to load in
		* @param slotName - The name of the slot to load in from 
		 */
	UFUNCTION(BlueprintCallable)
		void LoadTopology(FString topologyName, FString slotName);

	UFUNCTION(BlueprintCallable, CallInEditor)
		/**
		*Loads a topology from the save game object given the default variables that can be changed in the editor or blueprints. 
		*/
		void LoadTopologyDefault();

	UFUNCTION(BlueprintCallable, CallInEditor)
		/**
		*Saves the current topology with the default variables. Suitable for basic and advanced uses and easy to use, plus it can be used in the editor for quick use.
		*/
		void SaveCurrentTopologyDefault();

	UFUNCTION(BlueprintCallable)
		/**
		* Saves the current topology. This is a more advanced way to save the topology as you have more customisable options
		* @param topologyName - The name to save the topology as
		* @param customSlotName - The name of the slot to save the topology into. If it exists already then it will overwrite it, if not then it will be added
		*/
		void SaveCurrentTopology(FString topologyName, FString customSlotName);

	UFUNCTION(BlueprintCallable)
		/**
		* Loads in the leap data from a save game object. Used for training and testing, instead of run time use
		* @param customSlotName - The name of the slot to load the data from
		*/
		void LoadLeapData(FString customSlotName);

	UFUNCTION(BlueprintCallable)
		/**
		* Trains the network
		* @param inputs - The inputs for the network to feed forward through the network
		* @param targets - The target values that the network is trying to get to, will be used for back propagation
		* @return - Whether or not the training operation ran successfully
		*/
		bool Train(TArray<float> inputs, TArray<float> targets);

	UFUNCTION(BlueprintCallable)
		/**
		* Trains the network
		* @param inputs - The inputs to the network
		* @param targetIndex - The index of the neuron on the output layer
		*/
		bool TrainIndex(TArray<float> inputs, int targetIndex);

	UFUNCTION(BlueprintCallable)
		/**
		* Feeds the inputs given through the network
		* @param input - The input for the network that will be fed forward.
		* @return Whether the feed forward was carried out successfully
		*/
		bool FeedForward(TArray<float> input);

	

#pragma endregion

#pragma region UPROPERTY Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Easy way to set up the topology */
		TArray<int> baseTopology;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Lightweight way to display and save the topology */
		TArray<FNeuralLayer> Topology;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Used for initialisation */
		bool bRandomiseWeights = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Quick way to disable the bias for development purposes */
		bool bEnableBias = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOverwriteTheta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Threshold value for activation function
		float theta = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Activation Function for all neurons (uninitialised and initialised)
		TEnumAsByte<ActivationFunction> activationFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Learning rate of the network
		float alpha = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Name of the Current topology. Change before saving and loading to get the appropriate results */
		FString topologyName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Name of the slot to save and load from. Can be changed here instead of in script */
		FString slotName = "Test";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Test variable to make sure that the save and loading system was working */
		bool bPreviouslySaved;


#pragma endregion

#pragma region Variables

	/**
	* Container for the Neural Layers that is essentially the neural network topology. Using a vector as it should be
	* kept in C++
	*/
	vector<NeuralLayer> neuralLayers;

	/** Save game to save topologies */
	class UTopologies* topologies;

	TArray<FFrameData> trainingData;

#pragma endregion
};
