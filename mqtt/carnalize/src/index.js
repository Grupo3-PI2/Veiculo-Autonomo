import React, { useState } from "react";
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { NavigationContainer } from '@react-navigation/native';
import Ionicons from "@expo/vector-icons/Ionicons";

import HomePageView from "./home";
import NotificationsPageView from "./notification";

import buffer from 'buffer';
global.Buffer = buffer.Buffer
const Tab = createBottomTabNavigator();

const AppScreen = () => {
  const [notifications, setNotifications] = useState([]);

  const addNotification = (notification) => {
    console.log("notification: ", notification);
    
    // add notification to the list
    setNotifications((notifications) => [...notifications, notification]);
  };

  return (
    <NavigationContainer>
      <Tab.Navigator>
        <Tab.Screen
          name="Home"
          options={{
            tabBarIcon: ({ color, size }) => (
              <Ionicons name="home" color={color} size={size} />
            ),
            headerShown: false,
          }}
        >
          {() => <HomePageView addNotification={addNotification} />}
        </Tab.Screen>
        <Tab.Screen
          name="Notifications"
          options={{
            tabBarIcon: ({ color, size }) => (
              <Ionicons name="notifications" color={color} size={size} />
            ),
            headerShown: false,
          }}
        >
          {() => <NotificationsPageView notifications={notifications} />}
        </Tab.Screen>
      </Tab.Navigator>
    </NavigationContainer>
  );
};

export default AppScreen;