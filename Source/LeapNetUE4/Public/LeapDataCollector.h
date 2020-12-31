// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Leap/include/LeapC.h"
#include "LeapMotion/Public/LeapMotionData.h"
#include "LeapDataCollector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAPNETUE4_API ULeapDataCollector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeapDataCollector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		/**
		* Given the frame data generated from the leap motion controller, it will calculate the inputs to give to the neural network
		* @param frameData - The frame data to convert to an array of floats
		* @return Inputs to be fed into the neural network. Ensure that the length of this array is equal to the size of the input layer
		*/
	TArray<float> CalculateData(FLeapFrameData frameData);

#pragma region Recording

	UFUNCTION(BlueprintCallable, CallInEditor)
		void Snapshot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/**
		* This is the name of the gesture that is currently active and will be attached to all recorded training data
		*/
		FString gestureName;

#pragma endregion

private:

#pragma region CalculateData

	/**
	* Calculates the bone data as an array of floats. Provides the foundation for the data collection. Did this way to make it easy to change and optimise.
	* @param boneData - The LeapBoneData to calculate the floats for
	* @return - Array of floats that will make up the majority of the inputs for the network
	*/
	TArray<float> CalculateBoneData(FLeapBoneData boneData);

	/**
	* Calculates the Digit Data as an array of floats. Uses the CalculateBoneData function to calculate the data for the digit
	* @param digitData - The LeapDigitData to be used in the float calculation
	* @return - Array of floats to be used for this digit
	*/
	TArray<float> CalculateDigitData(FLeapDigitData digitData);

	/**
	* Calculates the Palm Data as an array of floats. Uses a custom function as it needs to extract different data
	* @param - palmData - The LeapPalmData to be used in the float calculation
	* @return - Array of floats to be used for this palm
	*/
	TArray<float> CalculatePalmData(FLeapPalmData palmData);

	/**
	* Calculates the Hand Data as an array of floats. Uses the CalculateDigitData and CalculatePalmData to get the data for the hand
	* @param handData - The LeapHandData to be used in this calculation
	* @return - Array of floats that are the total result of this hand's digits and it's palm
	*/
	TArray<float> CalculateHandData(FLeapHandData handData);

#pragma endregion

	FString test;


};
