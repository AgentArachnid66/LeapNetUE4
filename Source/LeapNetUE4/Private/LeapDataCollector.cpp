// Fill out your copyright notice in the Description page of Project Settings.


#include "LeapDataCollector.h"

// Sets default values for this component's properties
ULeapDataCollector::ULeapDataCollector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULeapDataCollector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULeapDataCollector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULeapDataCollector::Snapshot() {

	UE_LOG(LogTemp, Warning, TEXT("Leap Frame Data was recieved"));

}

#pragma region CalculateData

TArray<float> ULeapDataCollector::CalculateData(FLeapFrameData frameData)
{
	TArray<float> returnValue;

	test = frameData.NumberOfHandsVisible > 0 ? "At least one hand is visible" : "No hands in frame data";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *test);



	for (FLeapHandData &hand : frameData.Hands)
	{

	// Need to check if the hand is visible and has data before we try calculating data for it.

	// NOTE - Change to getting empty data if hand isn't visible (all 0s) as this may affect the neural network. The
	// length of the array that will be used will be determined after all other substantial changes and milestones have
	// been reached
		test = hand.HandType == EHandType::LEAP_HAND_LEFT ? "Left Hand" : "Right Hand";
		UE_LOG(LogTemp, Warning, TEXT("%s being collected"), *test);

		returnValue.Append(CalculateHandData(hand));
	}

	
	return returnValue;
}

TArray<float> ULeapDataCollector::CalculateBoneData(FLeapBoneData boneData)
{
	TArray<float> returnValue;

	FVector forward = UKismetMathLibrary::GetForwardVector(boneData.Rotation);
	UE_LOG(LogTemp, Warning, TEXT("Bone's Forward Vector: %s"), *forward.ToString());


	returnValue.Add(forward.X);
	returnValue.Add(forward.Y); 
	returnValue.Add(forward.Z);

	return returnValue;
}

TArray<float> ULeapDataCollector::CalculateDigitData(FLeapDigitData digitData)
{
	
	TArray<float> returnValue;
	for (int bone = 0; bone < digitData.Bones.Num(); bone++) {
		test = FString::FromInt(digitData.FingerId);
		UE_LOG(LogTemp, Warning, TEXT("Finger ID: %s"), *test);

		returnValue.Append(CalculateBoneData(digitData.Bones[bone]));
	}

	return returnValue;
}

TArray<float> ULeapDataCollector::CalculateHandData(FLeapHandData handData)
{
	TArray<float> returnValue;

	for (int digit = 0; digit < handData.Digits.Num(); digit++) {
		returnValue.Append(CalculateDigitData(handData.Digits[digit]));
	}

	returnValue.Append(CalculatePalmData(handData.Palm));

	return returnValue;
}

TArray<float> ULeapDataCollector::CalculatePalmData(FLeapPalmData palmData)
{
	TArray<float> returnValue;
	returnValue.Add(palmData.Orientation.Yaw);
	returnValue.Add(palmData.Orientation.Pitch);
	returnValue.Add(palmData.Orientation.Roll);
	returnValue.Add(palmData.Direction.ForwardVector.X);
	returnValue.Add(palmData.Direction.ForwardVector.Y);
	returnValue.Add(palmData.Direction.ForwardVector.Z);
	returnValue.Add(palmData.Velocity.Size());
	returnValue.Add(palmData.StabilizedPosition.X);
	returnValue.Add(palmData.StabilizedPosition.Y);
	returnValue.Add(palmData.StabilizedPosition.Z);

	return returnValue;
}

#pragma endregion