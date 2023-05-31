import React, { useEffect, useState, useRef } from "react";
import {
  SafeAreaView,
  Text,
  TouchableOpacity,
  View,
  TextInput,
} from "react-native";
import Ionicons from "@expo/vector-icons/Ionicons";
import io from 'socket.io-client';
 
import styles from "./styles";
import { Alert } from "./components/alert";
  
const HomePageView = ({ addNotification }) => {
  const [objectFound, setObjectFound] = useState(false);
  const [showFirstPage, setShowFirstPage] = useState(true);
  const [routeDistance, setRouteDistance] = useState(0);
  const [errorMessage, setErrorMessage] = useState('');
  const [carProgress, setCarProgress] = useState({
    progress: 0,
    speed: 0,
    isRunning: false,
    distance: 0,
  });

  const socketRef = useRef(null);

  useEffect(() => {
    socketRef.current = io('http://localhost:4000');

    socketRef.current.on('connect', () => {
      console.log('Connected to socket');
      setErrorMessage('');
    });
  
    socketRef.current.on('connect_error', (error) => {
      console.error('Socket connection error:', error);
      if (error.message !== 'Error connecting with the server') {
        addNotification({
          icon: <Ionicons name="alert-circle-outline" color="black" size={20} />,
          text: "Error connecting with the server",
        });
      }
      setErrorMessage('Error connecting to the server');
    });

    socketRef.current.on('carProgress', (progress) => {
      console.log('Car progress:', progress);
      setCarProgress(progress);

      // depending on the car progress, add a notification
      if (!progress.isRunning) {
        addNotification(
          {
            icon: <Ionicons name="car-outline" color="black" size={20} />,
            text: "O carro está parado",
          }
        );
      }
    });

    socketRef.current.on('objectFound', (progress) => {
      setCarProgress(progress);
      setObjectFound(true);
      addNotification(
        {
          icon: <Ionicons name="warning-outline" color="black" size={20} />,
          text: "Um objeto no caminho foi detectado",
        }
      );
    });

    socketRef.current.on('objectRemoved', (progress) => {
      console.log('Object removed:', progress);
      setCarProgress(progress);
      setObjectFound(false);
      addNotification(
        {
          icon: <Ionicons name="checkmark-outline" color="black" size={20} />,
          text: "O objeto foi removido",
        }
      );
    });

    return () => {
      socketRef.current.disconnect();
    };
  }, []);

  const handleStartCar = () => {
    if (objectFound) {
      return;
    }

    socketRef.current.emit('startCar');
  };

  const handleStopCar = () => {
    socketRef.current.emit('stopCar');
  };

  const handleResetCar = () => {
    setShowFirstPage(true);
    socketRef.current.emit('resetCar');
  };

  const getProgress = (carDistance) => {
    const totalDistance = routeDistance;

    if (carDistance > totalDistance) {
      return 100;
    }

    let result = (carDistance / totalDistance) * 100;

    if (isNaN(result)) {
      result = 0;
    }

    return Math.floor(result);
  };

  const handleRouteDistanceChange = (text) => {
    let cleanedText = text.replace(/[^0-9]/g, '');
    let number = parseInt(cleanedText, 10);
  
    if (isNaN(number)) {
      number = 0;
    }

    if (number > 100) {
      number = 100;
      setErrorMessage('O limite máximo é de 100 metros');
    } else {
      setErrorMessage('');
    }
  
    setRouteDistance(number);
  };

  return (
    <SafeAreaView style={[styles.viewPager, styles.container]}>

      {/* create a rectangle with background color */}
      <View style={[styles.header, { flex: 0.3 }]}>
        <Text style={styles.title}>Carnalize</Text>
      </View>
      <View style={styles.topBar} />


      {showFirstPage ? (
        <View style={[styles.container]}>
          <View style={[styles.container, styles.infoContainer]}>
            <Text style={styles.infoText}>
              Informe a distância da pista em metros:
            </Text>
            <TextInput
              style={styles.input}
              onChangeText={handleRouteDistanceChange}
              value={routeDistance}
              keyboardType="numeric"
            />
            {errorMessage ? <Text style={styles.errorText}>{errorMessage}</Text> : null}
          </View>

          {/* only show the Iniciar button if routeDistance != 0 */}
          {routeDistance ? (
            <View style={[styles.container, { flex: 0.5 }]}>
              <TouchableOpacity style={styles.btn} onPress={() => setShowFirstPage(false)}>
                <Text style={styles.btnText}>Iniciar</Text>
              </TouchableOpacity>
            </View>
          ) : null}
        </View>
      ) : null}

      {objectFound && !showFirstPage ? (
        <Alert />
      ) : null}

      {!objectFound && !showFirstPage && (
        <View style={styles.container}>
          <View style={styles.container}>
            <View id="filledBar" style={styles.bar}>
              <View
                id="filledBar"
                style={[styles.bar, styles.FilledBar, { width: getProgress(carProgress.distance) + '%' }]}
              >
                <View style={styles.barIcons}>
                  <View style={styles.barInfo}>
                    <Text style={styles.barInfoText}>{ getProgress(carProgress.distance) }%</Text>
                  </View>
                  <View style={styles.barDot} />
                  <Ionicons name="car-outline" color="black" size={25} />
                </View>
              </View>
            </View>
          </View>

          <View style={styles.container}>
            {errorMessage ? <Text style={styles.errorText}>{errorMessage}</Text> : null}

            <View style={[styles.container, styles.infoContainer]}>
              <Text style={styles.infoText}>
                Peso atual da carga:{" "}
                <Text style={[styles.infoText, styles.boldText]}>1.75kg</Text>
              </Text>
              <Text style={styles.infoText}>
                Limite de carga:
                <Text style={[styles.infoText, styles.boldText]}>2kg</Text>
              </Text>
              <Text style={styles.infoText}>
                Velocidade do carrinho:
                <Text style={[styles.infoText, styles.boldText]}>{carProgress.speed} km/h</Text>
              </Text>
              <Text style={styles.infoText}>
                Distância percorrida:
                <Text style={[styles.infoText, styles.boldText]}>{carProgress.distance} metros</Text>
              </Text>
            </View>
          </View>
        </View>
      )}

      {!showFirstPage ? (
        <View style={[styles.container, { flex: 0.5 }]}>
          {carProgress.isRunning ? (
            <TouchableOpacity style={styles.btn} onPress={handleStopCar}>
              <Text style={styles.btnText}>Parar o movimento</Text>
            </TouchableOpacity>
          ) : (
            <TouchableOpacity style={styles.btn} onPress={handleStartCar}>
              <Text style={styles.btnText}>Iniciar o movimento</Text>
            </TouchableOpacity>
          )}
          <TouchableOpacity style={[styles.btn, styles.resetBtn]} onPress={handleResetCar}>
            <Text style={styles.btnText}>Resetar o progresso</Text>
          </TouchableOpacity>
        </View>
      ) : null}
    </SafeAreaView>
  );
};

export default HomePageView;