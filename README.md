# LeapNetUE4

Developed with Unreal Engine 4

This project aims to create an easy and quick way to create custom gestures using a LeapMotion Controller. It uses a neural network that can be set to a custom size for optimisation but will come with a topology that works. 

The neural network is an actor component that can be attached to any actor that will feed the input into it. It has a few features that are the ability to save and load a topology from a save file from both the editor and through scripting and the ability to train and test the network through functions that are available in the blueprint as well. I made it a separate class to encapsulate the functionality and have the leap motion data being collected and processed in a separate class so that the neural network can be used for projects that don't require the leap motion.

The leap motion data collector uses the forward vector data to avoid issues with differently sized hands and distance from the controller so these don't affect the training of the neural network.


To use, after adding elements to the base topology array or changing any element of the Topology array, you must call the initialise function. To train and test, you must call them in the parent actor that you choose to attach it to, with the relevant input and target values.

</n>
<b> 13/01/2021 Commit Update </b>



Training is still a work in progress to get the back propagation to work. I'm close, but it's probably the hardest thing to program in this entire project. This is the first neural network I've ever made, and I hope to make more Machine Learning applications like this. I'm planning on adding some more activation functions such as the tanh function and also some more options for machine learning architetures such as K nearest neighbours in a similar way to the neural network. Which is to say an actor component that isn't reliant on anything, apart from the classes made purely for it, and can be used in an application completely different to my own.  

To optimise my application, it might be a good idea to transfer the learning aspect over to a separate c++ program and import in the weights, biases and theta values via a format like JSON. I'll keep the training built in if I go this route to keep everything within UE4 as much as possible, but without a doubt having less code for processors, such as the difference between a separate program and running training in UE4, will have a significant increase on performance, and therefore reduce the amount of time taken for training and  the risk of crashing or freezing. As my computer is not exactly top specs, this may be the route I have to go down in order to train my neural network for the purpose it was originally conceived for.

In addition to the above changes, I would also like to add some additionaly safe guards for users as I'm running into a problem that many developers face which is that I'm getting too used to my plugin and know how to avoid the errors that can cause it to crash, which end users won't immediately know. I'll use MantisBT to help me keep check of the bugs amd to help get them organised. 
