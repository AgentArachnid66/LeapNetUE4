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

	if (this->LeapData == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Save Game variable is null pointer"));

		if (UGameplayStatics::DoesSaveGameExist(this->slotName, 0)) {

			UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
			this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::LoadGameFromSlot(this->slotName, 0));
		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
			this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::CreateSaveGameObject(ULeapNeuralData::StaticClass()));
		}
	}
	
	// Makes a timer for the recording functionality and then pauses it
	GetWorld()->GetTimerManager().SetTimer(recorderHandle, this, &ULeapDataCollector::Snapshot, 1.f/this->recordRate, true);
	GetWorld()->GetTimerManager().PauseTimer(recorderHandle);

}


// Called every frame
void ULeapDataCollector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#pragma region Data Collection

void ULeapDataCollector::Snapshot() {

	
	// Adds the data
	trainingData.Add(FFrameData(CalculateData(data), gestureName));
	
	// Recording functionality

	// Checks if the timer is unpaused and exists
	if (GetWorld()->GetTimerManager().IsTimerActive(recorderHandle)) {

		// Increments so that if this data is deleted, it will delete all the relevant data
		recordingIterations++;

		// Debugging 
		test = FString::FromInt(recordingIterations);
		UE_LOG(LogTemp, Warning, TEXT("Number of recording Iterations: %s"), *test);
	}

}

void ULeapDataCollector::RemoveLastSnapshot()
{
	trainingData.RemoveAt(trainingData.Num() - 1);
}

void ULeapDataCollector::RemoveLastNSnapshots(int last)
{
	for (int i = trainingData.Num() - last; i < trainingData.Num(); i++) {
		trainingData.RemoveAt(i);
	}
}

void ULeapDataCollector::StartRecording()
{
	// Unpauses the recording timer
	GetWorld()->GetTimerManager().UnPauseTimer(recorderHandle);
}

void ULeapDataCollector::StopRecording()
{
	// Pauses the timer. 
	GetWorld()->GetTimerManager().PauseTimer(recorderHandle);
}

void ULeapDataCollector::SaveRecording()
{
	SaveLeapData(this->slotName, this->overwriteData);
}

void ULeapDataCollector::DeleteRecording()
{
	// Only deletes the most recent recording from the training data.
	RemoveLastNSnapshots(this->recordingIterations);
	recordingIterations = 0;
}

void ULeapDataCollector::GestureIndicesFromTags()
{
	TMap<FString, int> gestureMap;
	int highest = 0;
	for (int i = 0; i < trainingData.Num(); i++) {
		// If the gesture is unique then it will be set to the highest value so far, 
		// else it will be set to the correct index
		trainingData[i].gestureIndex = gestureMap.FindOrAdd(trainingData[i].gestureTag, highest);

		// If it is equal to the highest value so far, I need to increment the highest value so that the next unique
		// gesture gets the correct index assigned to it
		if (trainingData[i].gestureIndex == highest) {
			highest++;
		}

	}
}

#pragma endregion

#pragma region Saving and Loading

void ULeapDataCollector::SaveLeapData(FString customSlotName, bool overwriteTrainingData) {


	if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

		UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
		this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
		this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::CreateSaveGameObject(ULeapNeuralData::StaticClass()));
	}
	
	if (this->LeapData->IsValidLowLevel()) {

		// Overwrites the frame data on the save
		if (overwriteTrainingData) {
			this->LeapData->frameData = this->trainingData;
			test = FString::FromInt(this->LeapData->ConvertTagsToInts());
			UE_LOG(LogTemp, Warning, TEXT("Number of Unique Gestures in Save: %s"), *test);
		}

		// Adds the data to the save
		else {
			this->LeapData->frameData.Append(this->trainingData);
			test = FString::FromInt(this->LeapData->ConvertTagsToInts());
			UE_LOG(LogTemp, Warning, TEXT("Number of Unique Gestures in Save: %s"), *test);
		}



		// Saves the game to the slot
		UGameplayStatics::SaveGameToSlot(this->LeapData, customSlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Game saved"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid Save Game Object"));

		if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

			UE_LOG(LogTemp, Warning, TEXT("SaveGame Exists, Loading for saving operation"));
			this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));
		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("Save Game doesn't exist, creating new one"));
			this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::CreateSaveGameObject(ULeapNeuralData::StaticClass()));
		}

	}

}

void ULeapDataCollector::LoadLeapData(FString customSlotName) {
	
	
	if (UGameplayStatics::DoesSaveGameExist(customSlotName, 0)) {

		this->LeapData = Cast<ULeapNeuralData>(UGameplayStatics::LoadGameFromSlot(customSlotName, 0));

		// Retrieves the topology from the save game object
		this->trainingData = this->LeapData->frameData;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No save game found in slot"));
	}
}

void ULeapDataCollector::SaveToText(FString fileName, FString content, FString fileExt) {

	FString dir = FPaths::ProjectDir() + fileName + fileExt;
	if (FFileHelper::SaveStringToFile(content, *dir)) {
		UE_LOG(LogTemp, Warning, TEXT("File saved to %s successfully"), *dir);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("File Failed to save to %s"), *dir);
	}

}

#pragma endregion

#pragma region CalculateData

TArray<float> ULeapDataCollector::CalculateData(FLeapFrameData frameData)
{
	TArray<float> returnValue;

	TArray<float> replacement;

	replacement.Init(0, 64);

	test = frameData.NumberOfHandsVisible > 0 ? "At least one hand is visible" : "No hands in frame data";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *test);

	if (!frameData.LeftHandVisible) {
		UE_LOG(LogTemp, Warning, TEXT("Left Hand is not visible in Frame Data"));
		returnValue = replacement;
	}

	for (FLeapHandData &hand : frameData.Hands)
	{

	// Need to check if the hand is visible and has data before we try calculating data for it.

		test = hand.HandType == EHandType::LEAP_HAND_LEFT ? "Left Hand" : "Right Hand";
		UE_LOG(LogTemp, Warning, TEXT("%s being collected"), *test);

		TArray<float> calData = CalculateHandData(hand);

		test = FString::FromInt(calData.Num());
		UE_LOG(LogTemp, Warning, TEXT("Number of elements in dataset: %s"), *test);

		returnValue.Append(calData);
	}

	if (!frameData.RightHandVisible) {
		UE_LOG(LogTemp, Warning, TEXT("Right Hand is not visible in Frame Data"));
		returnValue.Append(replacement);
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

	FVector palmForward = UKismetMathLibrary::GetForwardVector(palmData.Orientation);

	returnValue.Add(palmForward.X);
	returnValue.Add(palmForward.Y);
	returnValue.Add(palmForward.Z);
	returnValue.Add(palmData.Velocity.Size());

	return returnValue;
}

#pragma endregion