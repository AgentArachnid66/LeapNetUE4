# LeapNetUE4

Developed with Unreal Engine 4

This project aims to create an easy and quick way to create custom gestures using a LeapMotion Controller. It uses a neural network that can be set to a custom size for optimisation but will come with a topology that works. 

The neural network is an actor component that can be attached to any actor that will feed the input into it. It has a few features that are the ability to save and load a topology from a save file (still a work in progress) from both the editor and through scripting and the ability to train and test the network through functions that are available in the blueprint as well. I made it a separate class to encapsulate the functionality and have the leap motion data being collected and processed in a separate class so that the neural network can be used for projects that don't require the leap motion.
