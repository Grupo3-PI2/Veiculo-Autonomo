import React, { useState } from "react";
import {
  SafeAreaView,
  Text,
  TouchableOpacity,
  View,
} from "react-native";
import Ionicons from "@expo/vector-icons/Ionicons";
 
import styles from "./styles";

const Alert = () => {
    return (
      <View style={styles.container}>
        <Ionicons name="warning-outline" size={100} />
        <Text style={[styles.boldText, styles.alertText]}>
          Atenção: O carro encontrou um obstáculo. É necessário remover o
          obstaculo da frente.
        </Text>
      </View>
    );
  };
  
  const HomePageView = ({ ...props }) => {
    const [progress, setProgress] = useState(100);
    const [isCarStop, setIsCarStop] = useState(false);
  
    return (
      <SafeAreaView style={[styles.viewPager, styles.container]}>
  
        {/* create a rectangle with background color */}
        <View style={[styles.header, { flex: 0.3 }]}>
          <Text style={styles.title}>Carnalize</Text>
        </View>
        <View style={styles.topBar} />
  
        {isCarStop ? (
          <Alert />
        ) : (
          <View style={styles.container}>
            <View style={styles.container}>
              <View id="filledBar" style={styles.bar}>
                <View
                  id="filledBar"
                  style={[styles.bar, styles.FilledBar, { width: progress }]}
                >
                  <View style={styles.barIcons}>
                    <View style={styles.barInfo}>
                      <Text style={styles.barInfoText}>50%</Text>
                    </View>
                    <View style={styles.barDot} />
                    <Ionicons name="car-outline" color="black" size={25} />
                  </View>
                </View>
              </View>
            </View>
  
            <View style={styles.container}>
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
                  <Text style={[styles.infoText, styles.boldText]}>1 km/h</Text>
                </Text>
              </View>
            </View>
          </View>
        )}
  
        <View style={[styles.container, { flex: 0.5 }]}>
          <TouchableOpacity style={styles.btn}>
            <Text style={styles.btnText}>Iniciar o movimento</Text>
          </TouchableOpacity>
        </View>
      </SafeAreaView>
    );
  };

  export default HomePageView;