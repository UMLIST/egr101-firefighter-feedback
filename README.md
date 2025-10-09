# egr101-firefighter-feedback
repository for egr 101 firefighter feedback

# software
- nRF Connect SDK 2.6.0
- nRF Mesh Mobile App

# hardware
-nRF52840 Developmnent Kit
-nRF52840 Dongle

# subdirectories
- Sender: This code is generating a message every second with the following format: "MSG,<MAC address>,<heart rate>". The heart rate counts up each message iteration beginning at 40 (resets to 40 once it reaches 100). An LED also blinks each time a message is sent as a physical indicator to the user. This should be deployed on the device simulating the device inside of the ship and firefighter pocket that is transmitting a firefighter's current status. 
- Destination: Receieves the message sent by the sender. This should be deployed on the device simulating a receiver at the very end of of the hose line outside of the ship that can connect with the base station computer monitoring the scene
- Relay: This is code deployed on any relay node along the hose line

# how to use
- Once the code is deployed, power on the devices. Right after programming, they will each need to be provisioned via the nRF Mesh App. This step only needs to be done once. 
- After provisioning, monitor the ```Desination``` node via PuTTY, or some other Shell program. The received message will be displayed.
- You can ignore any warning messages-- this is just a simple proof of concept demo. Also, don't worry if messages are recieved out of order or missed. As long as messages are in general being recieved, we can declare it working!

NOTE: The address of the Destination is hardcoded in the sender code (mostly because I couldn't get global messages to work...) SO after provisioning the Desintation node on the nRF Mesh app, you MUST modify the address value (Line 443 in model_handler.c in the ```Sender``` subdirectory). The address is given in the nRF Mesh app after it is provisioned by the same app. 