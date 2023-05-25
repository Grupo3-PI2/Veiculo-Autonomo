import React, { useEffect, useState } from "react";
import {
  SafeAreaView,
  StyleSheet,
  Text,
  TouchableOpacity,
  View,
} from "react-native";
import Ionicons from "@expo/vector-icons/Ionicons";

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

export default HomePageView = ({ ...props }) => {
  const [progress, setProgress] = useState(100);
  const [isCarStop, setIsCarStop] = useState(false);

  return (
    <SafeAreaView style={[styles.viewPager, styles.container]}>
      <View style={[styles.container, { flex: 0.5 }]}>
        <Text style={styles.title}>Caranalizer</Text>
      </View>

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

const styles = StyleSheet.create({
  viewPager: {
    flex: 1,
    backgroundColor: "#E0E0E0",
  },
  page: {
    justifyContent: "center",
    alignItems: "center",
  },
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
  },
  title: {
    fontSize: 40,
    fontWeight: 600,
  },
  bar: {
    width: 350,
    position: "relative",
    backgroundColor: "white",
    height: 10,
    borderRadius: 5,
  },
  FilledBar: {
    width: 100,
    borderBottomRightRadius: 0,
    borderTopRightRadius: 0,
    backgroundColor: "#2196f3",
  },
  barIcons: {
    position: "absolute",
    top: -33,
    right: -15,
    height: 80,
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
  },
  barInfo: {
    backgroundColor: "#0f3e8d",
    padding: 3,
    borderRadius: 5,
    display: "flex",
    justifyContent: "center",
    alignItems: "center",
  },
  barInfoText: {
    fontSize: 12,
    color: "white",
  },
  barDot: {
    position: "relative",
    width: 20,
    height: 20,
    borderRadius: 10,
    backgroundColor: "white",
    borderColor: "blue",
    borderWidth: 2,
    borderStyle: "solid",
  },
  infoContainer: {
    width: 350,
    flex: 1,
    alignItems: "flex-start",
  },
  infoText: {
    fontSize: 18,
  },
  boldText: {
    fontWeight: "600",
  },
  btn: {
    width: 250,
    height: 50,
    alignItems: "center",
    backgroundColor: "#2196f3",
    justifyContent: "center",

    borderRadius: 10,
  },
  btnText: {
    color: "white",
    fontSize: 16,
    fontWeight: 600,
  },
  alertText: {
    fontSize: 16,
    textAlign: "center",
    paddingHorizontal: 50,
  },
});
