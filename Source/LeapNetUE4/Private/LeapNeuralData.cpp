// Fill out your copyright notice in the Description page of Project Settings.


#include "LeapNeuralData.h"
#include "LeapNetUE4.h"

int ULeapNeuralData::ConvertTagsToInts() {
	int highest = 0;

	for (int data = 0; data < frameData.Num(); data++) {
		// If the gesture is unique then it will be set to the highest value so far, 
		// else it will be set to the correct index
		frameData[data].gestureIndex = gestureMap.FindOrAdd(frameData[data].gestureTag, highest);

		// If it is equal to the highest value so far, I need to increment the highest value so that the next unique
		// gesture gets the correct index assigned to it
		if (frameData[data].gestureIndex == highest) {
			highest++;
		}

	}
	// Returns the number of unique gestures, 0-based
	return highest;
}

void ULeapNeuralData::RemoveGesture(FString gesture)
{
	for (int i = 0; i < frameData.Num(); i++) {
		if (frameData[i].gestureTag == gesture) {
			frameData.RemoveAt(i);
		}
	}
}

void ULeapNeuralData::RemoveData(FString gesture, int dataIndex)
{
	int index = 0;
	for (int i = 0; i < frameData.Num(); i++) {
		if (frameData[i].gestureTag == gesture) {
			if (i == index) {
				frameData.RemoveAt(i);
			}
			index++;
		}
	}
}

int ULeapNeuralData::GetNumberOfUniqueGestures()
{

	return gestureMap.Num();
}

bool ULeapNeuralData::DoesGestureExist(FString gesture)
{
	return gestureMap.Contains(gesture);
}
