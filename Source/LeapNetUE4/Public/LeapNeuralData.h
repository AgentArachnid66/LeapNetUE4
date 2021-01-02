// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeapData_Structs.h"
#include "LeapNeuralData.generated.h"

/**
 * 
 */
UCLASS()
class LEAPNETUE4_API ULeapNeuralData : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		// Data to save and will be fed into the neural network
		TArray<FFrameData> frameData;

	UPROPERTY(BlueprintReadWrite)
		// Map to control the conversion of gesture name to the corresponding index
		TMap<FString, int> gestureMap;


	UFUNCTION(BlueprintCallable)
		/**
		* Iterates through the frame data and converts the tags attached to each set of data to an integer that will correspond to the index of the output layer of the neural network. Sets up the data for use in the neural network.
		* @return The number of unique gestures in the data. Useful for setting up the topology correctly
		*/
		int ConvertTagsToInts();

	UFUNCTION(BlueprintCallable)
		/** Removes a gesture's dataset from the save file
		* @param gesture - Gesture to remove from dataset.
		*/
		void RemoveGesture(FString gesture);

	UFUNCTION(BlueprintCallable)
		/** Removes a specific dataset from a gesture's entire dataset from the save file
		* @param gesture - Gesture to remove data from
		* @param dataIndex - Index of the gesture data to remove, relative to the gesture.
		*/
		void RemoveData(FString gesture, int dataIndex);

	UFUNCTION(BlueprintCallable)
		/**
		* Returns the number of unique gestures within the save game
		* @return number of unique gestures that exist 1-based
		*/
		int GetNumberOfUniqueGestures();

	UFUNCTION(BlueprintCallable)
		/**
		* Returns if the gesture exists in the save game
		* @param gesture - The gesture to look for
		* @return Whether or not the gesture exists
		*/
		bool DoesGestureExist(FString gesture);
};
