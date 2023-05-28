import { StyleSheet } from "react-native";

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

  export default styles;