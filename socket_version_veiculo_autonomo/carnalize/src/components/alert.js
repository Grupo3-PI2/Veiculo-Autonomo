import { Text, View} from "react-native";
import Ionicons from "@expo/vector-icons/Ionicons";

import styles from "./styles";

export const Alert = () => {
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