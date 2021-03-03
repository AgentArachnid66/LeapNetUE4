# LeapNetUE4

Developed with Unreal Engine 4

This project aims to create an easy and quick way to create custom gestures using a LeapMotion Controller. It uses a neural network that can be set to a custom size for optimisation but will come with a topology that works. 

The neural network is an actor component that can be attached to any actor that will feed the input into it. It has a few features that are the ability to save and load a topology from a save file from both the editor and through scripting and the ability to train and test the network through functions that are available in the blueprint as well. I made it a separate class to encapsulate the functionality and have the leap motion data being collected and processed in a separate class so that the neural network can be used for projects that don't require the leap motion.

The leap motion data collector uses the forward vector data to avoid issues with differently sized hands and distance from the controller so these don't affect the training of the neural network.

<b> User Manual </b>

To use, after adding elements to the base topology array, changing any element of the Topology array or load in a new topology, you must call the initialise function. To train and test, you must call them in the parent actor that you choose to attach it to, with the relevant input and target values.

The initialise function must be called to transform the Topology variable in the editor into a neural network object to be used.

For more information on how to use this repository please visit here (docs are a work in progress):
https://www.notion.so/Leap-Net-UE4-79b423b5958c45b79a836b83b6ed8ea7

</n>
<b> 13/01/2021 Commit Update </b>



Training is still a work in progress to get the back propagation to work. I'm close, but it's probably the hardest thing to program in this entire project. This is the first neural network I've ever made, and I hope to make more Machine Learning applications like this. I'm planning on adding some more activation functions such as the tanh function and also some more options for machine learning architetures such as K nearest neighbours in a similar way to the neural network. Which is to say an actor component that isn't reliant on anything, apart from the classes made purely for it, and can be used in an application completely different to my own.  

To optimise my application, it might be a good idea to transfer the learning aspect over to a separate c++ program and import in the weights, biases and theta values via a format like JSON. I'll keep the training built in if I go this route to keep everything within UE4 as much as possible, but without a doubt having less code for processors, such as the difference between a separate program and running training in UE4, will have a significant increase on performance, and therefore reduce the amount of time taken for training and  the risk of crashing or freezing. As my computer is not exactly top specs, this may be the route I have to go down in order to train my neural network for the purpose it was originally conceived for.

Another optimisation method that I could still do is convert my functions into async functions and implement multi threading, therefore it's removed from the main thread and won't cause as bad as a drag on the performance. Given I'm going to optimise for VR anyway, this seems like a good idea anyway. I'll work on this after I have the back propagation. 

In addition to the above changes, I would also like to add some additionaly safe guards for users as I'm running into a problem that many developers face which is that I'm getting too used to my plugin and know how to avoid the errors that can cause it to crash, which end users won't immediately know. I'll use MantisBT to help me keep check of the bugs and to help get them organised. 

</n>
<b> 14/01/2021 Commit Update </b>


Back propagation is still a work in progress, but it's so nearly there. I've made it adjust the delta weights appropriately and apply them, and also calculate the correct delta theta (threshold value) for each neuron. My next step is getting the delta theta applied to the right neurons. After that, see what I can do to optimise the network and make it as easy as possible for users to train the network. I'm thinking of a main function that handles the iteration in a separate thread, with a single input of a data set to perform the training with. I'll implement the different activation functions after this step.

One thing I haven't tested yet is how it handles the bias neurons. This is obviously something I need to check works, but this will come after I have the delta Theta working.

</n>
<b> 15/01/2021 Commit Update </b>

Delta weights and theta values are adjusting according to what previous calculations for one data set, but after setting it to train over the course of several hundread epochs, it still isn't learning and is instead converging to 0.5 regardless of input. I'll do some more calculations for the other 3 data sets to see what results the network should give me for those and compare them. I hope it is a quick and simple fix, though when is it that easy.

In brighter news, I've added in the enum to select other activation functions and although I haven't implemented the function itself, each neuron can have a different activation function. 

</n>
<b> 16/01/2021 Commit Update </b>

Training still isn't working, despite continuous efforts to figure out what's going wrong. I quickly implemented some more groundwork for the multiple activation functions, all I need to do now is to actually implement them. 

</n>
<b> 03/03/2021 Commit Update </b>

So I decided to develop a separate application in Python that will handle the training and all this project needs to do is be able to read in the correct weights and it should work appropriately. That Python project is not yet ready to be released, as I still need to implement a few more features. I will not just abandon this project when I'm so close.
