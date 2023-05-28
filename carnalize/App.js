import { StatusBar } from "expo-status-bar";
import { StyleSheet, Text, View } from "react-native";
import AppScreen from "./src";

export default function App() {
  return <AppScreen />;
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#fff",
    alignItems: "center",
    justifyContent: "center",
  },
});
