import React, { useEffect, useState } from "react";
import {
  SafeAreaView,
  StyleSheet,
  Text,
  TouchableOpacity,
  View,
} from "react-native";
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { NavigationContainer } from '@react-navigation/native';
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

const NotificationsPageView = () => {
  const [isCarStop, setIsCarStop] = useState(false);
  var notifications = [];
  notifications.push(
    {
      icon: null,
      text: "O carro iniciou o movimento",
    },
    {
      icon: null,
      text: "O carro está parado",
    },
    {
      icon: <Ionicons name="warning-outline" color="black" size={20} />,
      text: "Um objeto no caminho foi detectado",
    },
    )

  return (
    <SafeAreaView style={[styles.viewPager, styles.container]}>

      {/* create a rectangle with background color */}
      <View style={[styles.header, { flex: 0.3 }]}>
        <Text style={styles.title}>Notificações</Text>
      </View>
      <View style={styles.topBar} />

      <View style={styles.infoContainer}>
        <View style={styles.line} />

        <View style={styles.notification}>
          {notifications.map((notification, index) => (
            <View key={index} style={styles.notificationItem}>
              <View style={styles.notificationIcon}>{notification.icon}</View>
              <Text style={styles.infoText}>{notification.text}</Text>
            </View>
          ))}
        </View>
      </View>

    </SafeAreaView>
  );
};

const Tab = createBottomTabNavigator();

const App = () => {
  return (
    <NavigationContainer>
      <Tab.Navigator>
        <Tab.Screen
          name="Home"
          component={HomePageView}
          options={{
            tabBarIcon: ({ color, size }) => (
              <Ionicons name="home" color={color} size={size} />
            ),
            headerShown: false,
          }}
        />
        <Tab.Screen
          name="Notifications"
          component={NotificationsPageView}
          options={{
            tabBarIcon: ({ color, size }) => (
              <Ionicons name="notifications" color={color} size={size} />
            ),
            headerShown: false,
          }}
        />
      </Tab.Navigator>
    </NavigationContainer>
  );
};

const styles = StyleSheet.create({
  viewPager: {
    flex: 0,
    backgroundImage: "linear-gradient(189.63deg, #FFFFFF 45.05%, #E4EBF5 100%)",
    height: "100vh",
    justifyContent: "flex-start",
  },
  page: {
    justifyContent: "center",
    alignItems: "center",
  },
  container: {
    flex: 1,
    width: "100%",
    justifyContent: "center",
    alignItems: "center",
  },
  title: {
    fontSize: 40,
    fontWeight: 849,
    color: "white",
    top: "0px",
    position: "absolute",
    marginTop: "3rem",
  },
  bar: {
    width: 350,
    position: "relative",
    backgroundColor: "#C7D3EB",
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
  header: {
    position: "absolute",
    width: "100%",
    height: "142px",
    left: "0px",
    top: "0px",
    alignItems: "center",
    
    backgroundColor: "#4FA6FF",
  },
  topBar: {
    width: "100%",
    height: "142px",
    left: "0px",
    top: "0px",
  },
  line: {
    height: 1,
    width: "90%",
    backgroundColor: "black",
    marginTop: "2rem",
    marginRight: "1rem",
    marginLeft: "1rem",
  },
  notification: {
    display: "flex",
    flexWrap: "wrap",
    marginRight: "2rem",
  },
  notificationItem: {
    display: "flex", 
    flexDirection: "row",
    gap: "1.5rem",
    borderBottomWidth: 1,
    marginLeft: "1rem",
    width: "100%",
    minHeight: "3rem",
    alignItems: "center",
    paddingTop: "10px",
    paddingBottom: "10px"
  },
  notificationIcon: {
    width: "10px",
    height: "10px",
    justifyContent: "center"
  }
});

export default App;