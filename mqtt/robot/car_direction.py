import paho.mqtt.client as mqtt

broker_address = "192.168.0.18"
broker_port = 1883
topic = "command/directions"

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with result code: " + str(rc))
    client.subscribe(topic)

def on_message(client, userdata, msg):
    message = msg.payload.decode("utf-8")
    print("")
    print("Received message: " + message)

    if message == "forward":
        print("Turning forward")
    elif message == "left":
        print("Turning left")
    elif message == "right":
        print("Turning right")
    else:
        print("Invalid command")

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.connect(broker_address, broker_port, 60)

client.loop_forever()
