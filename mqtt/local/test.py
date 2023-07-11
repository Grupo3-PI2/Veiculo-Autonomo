import paho.mqtt.client as mqtt

# Callback function for when the client successfully connects to the broker
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    # Subscribe to a topic after successful connection
    client.subscribe("your/topic")  # Replace with your desired topic

# Callback function for when a message is received on a subscribed topic
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

# Create a client instance
client = mqtt.Client()

# Set up the callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to the local MQTT broker
client.connect("localhost", 1883, 60)

# Run the MQTT client loop to handle network communication, callbacks, and message processing
client.loop_forever()