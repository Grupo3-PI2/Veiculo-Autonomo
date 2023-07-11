import React, { useState } from "react";
import {
  SafeAreaView,
  Text,
  View,
} from "react-native";
import Ionicons from "@expo/vector-icons/Ionicons";

import styles from "./styles";

const NotificationsPageView = ({ ...props }) => {
    const [isCarStop, setIsCarStop] = useState(false);
    var notifications = props.notifications;

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

  export default NotificationsPageView;