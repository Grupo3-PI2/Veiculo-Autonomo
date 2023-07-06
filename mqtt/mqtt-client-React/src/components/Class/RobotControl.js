import React from 'react';
import mqtt from 'mqtt';

import './RobotControl.css';
import arrowIcon from './arrow.svg';

window.Buffer = window.Buffer || require("buffer").Buffer;


class RobotControl extends React.Component {
  constructor(props) {
    super(props);

    this.mqttClient = null;

    this.state = {
      speed: 0,
    };
  }

  componentDidMount() {
    this.mqttClient = mqtt.connect('ws://192.168.0.101:9333');

    this.mqttClient.subscribe('robot/commands');

    this.mqttClient.on('connect', this.handleConnect);
    this.mqttClient.on('close', this.handleClose);
  }

  componentWillUnmount() {
    if (this.mqttClient) {
      this.mqttClient.end();
    }
  }

  handleConnect = () => {
    console.log('Connected to MQTT broker');
  };

  handleClose = () => {
    console.log('Connection closed');
  };

  handleDirectionButton(command) {
    this.sendDirectionMessage(`${command}`);
  }

  sendDirectionMessage(message) {
    this.mqttClient.publish('command/directions', message);
  }

  sendControlMessage(message) {
    this.mqttClient.publish('command/controls', message);
  }

  handleSpeedChange(speed) {
    if (speed < 0) {
      speed = 0;
    } else if (speed > 10) {
      speed = 10;
    };

    this.setState({ speed });
    this.sendControlMessage(`speed:${speed}`);
  }

  handleStartButton() {
    this.sendControlMessage('control:start');
  }

  handleStopButton() {
    this.sendControlMessage('control:stop');
  }

  render() {
    return (
      <div className="RobotControl">
        <div className="ButtonContainer">
          <button
            className="Button"
            onClick={() => this.handleDirectionButton('forward')}
          >
            <div className="ArrowIcon ArrowUp">
              <img
                src={arrowIcon}
                className="ArrowIcon"
                alt="Arrow"
                onMouseDown={(event) => event.preventDefault()}
              />
            </div>
          </button>
        </div>
        <div className="ButtonContainer">

          <button
            className="Button"
            onClick={() => this.handleDirectionButton('left')}
          >
            <div className="ArrowIcon ArrowLeft">
              <img
                src={arrowIcon}
                className="ArrowIcon"
                alt="Arrow"
                onMouseDown={(event) => event.preventDefault()}
              />
            </div>
          </button>
          <button
            className="Button"
            onClick={() => this.handleDirectionButton('right')}
          >
            <div className="ArrowIcon ArrowRight">
              <img
                src={arrowIcon}
                className="ArrowIcon"
                alt="Arrow"
                onMouseDown={(event) => event.preventDefault()}
              />
            </div>
          </button>
        </div>
        <div className="ButtonContainer Command">
          <button
            className="Button ButtonStart"
            onClick={() => this.handleStartButton()}
          >
            Start
          </button>
          <button
            className="Button ButtonStop"
            onClick={() => this.handleStopButton()}
          >
            Stop
          </button>
        </div>
        <div className="SpeedContainer">
          <div className="CurrentSpeed">Velocidade atual: {this.state.speed}</div>
          <div className="SpeedButtons">
            <button className="SpeedButton" onClick={() => this.handleSpeedChange(this.state.speed + 1)}>+</button>
            <button className="SpeedButton" onClick={() => this.handleSpeedChange(this.state.speed - 1)}>-</button>
          </div>
        </div>
      </div>
    );
  }
}

export default RobotControl;
