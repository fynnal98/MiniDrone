import 'dart:async';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_joystick/flutter_joystick.dart';
import 'package:udp/udp.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();

  // Querformat erzwingen
  SystemChrome.setPreferredOrientations([
    DeviceOrientation.landscapeLeft,
    DeviceOrientation.landscapeRight,
  ]).then((_) {
    runApp(const MyApp());
  });
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Drone Controller',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const DroneControllerPage(),
    );
  }
}

class DroneControllerPage extends StatefulWidget {
  const DroneControllerPage({Key? key}) : super(key: key);

  @override
  _DroneControllerPageState createState() => _DroneControllerPageState();
}

class _DroneControllerPageState extends State<DroneControllerPage> {
  double throttle = 0.0; // Vertikale Bewegung des linken Joysticks
  double yaw = 0.0;      // Horizontale Bewegung des linken Joysticks
  double pitch = 0.0;    // Vertikale Bewegung des rechten Joysticks
  double roll = 0.0;     // Horizontale Bewegung des rechten Joysticks

  bool isConnected = false; // WLAN-Verbindungsstatus
  late UDP sender;          // UDP-Client
  final String espIp = "192.168.4.1"; // ESP32-Standard-IP
  final int espPort = 4210;           // ESP32-Port

  Timer? _sendTimer;

  @override
  void initState() {
    super.initState();
    _connectToESP();
  }

  // Verbindung mit ESP32 herstellen
  Future<void> _connectToESP() async {
    try {
      sender = await UDP.bind(Endpoint.any());
      setState(() {
        isConnected = true;
      });

      // Timer starten, um kontinuierlich Daten zu senden
      _sendTimer = Timer.periodic(const Duration(milliseconds: 50), (timer) {
        _sendControlData();
      });
    } catch (e) {
      print("Fehler bei der Verbindung: $e");
      setState(() {
        isConnected = false;
      });
    }
  }

  // Daten senden
  Future<void> _sendControlData() async {
    if (isConnected) {
      final String data =
          "Throttle: ${throttle.toStringAsFixed(2)}, Yaw: ${yaw.toStringAsFixed(2)}, Pitch: ${pitch.toStringAsFixed(2)}, Roll: ${roll.toStringAsFixed(2)}";
      await sender.send(data.codeUnits, Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Drone Controller'),
        actions: [
          IconButton(
            icon: Icon(
              isConnected ? Icons.wifi : Icons.wifi_off,
              color: isConnected ? Colors.green : Colors.red,
            ),
            onPressed: () {
              setState(() {
                isConnected = !isConnected; // Testweise Verbindung umschalten
              });
            },
          ),
          IconButton(
            icon: const Icon(Icons.settings),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(
                  builder: (context) => const SettingsPage(),
                ),
              );
            },
          ),
        ],
      ),
      body: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          // Linker Joystick
          Padding(
            padding: const EdgeInsets.only(left: 128.0),
            child: SizedBox(
              width: 150,
              height: 150,
              child: Joystick(
                mode: JoystickMode.all,
                listener: (details) {
                  setState(() {
                    throttle = details.y;
                    yaw = details.x;
                  });
                },
                base: Container(
                  decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    border: Border.all(
                      color: Colors.orange,
                      width: 3.0,
                    ),
                    color: Colors.transparent,
                  ),
                ),
                stick: Container(
                  decoration: const BoxDecoration(
                    shape: BoxShape.circle,
                    color: Colors.black,
                  ),
                  width: 40,
                  height: 40,
                ),
              ),
            ),
          ),
          // Steuerwerte anzeigen
          Expanded(
            child: Center(
              child: Text(
                'Throttle: ${throttle.toStringAsFixed(2)}\n'
                    'Yaw: ${yaw.toStringAsFixed(2)}\n'
                    'Pitch: ${pitch.toStringAsFixed(2)}\n'
                    'Roll: ${roll.toStringAsFixed(2)}',
                style: const TextStyle(fontSize: 16),
                textAlign: TextAlign.center,
              ),
            ),
          ),
          // Rechter Joystick
          Padding(
            padding: const EdgeInsets.only(right: 128.0),
            child: SizedBox(
              width: 150,
              height: 150,
              child: Joystick(
                mode: JoystickMode.all,
                listener: (details) {
                  setState(() {
                    pitch = details.y;
                    roll = details.x;
                  });
                },
                base: Container(
                  decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    border: Border.all(
                      color: Colors.orange,
                      width: 3.0,
                    ),
                    color: Colors.transparent,
                  ),
                ),
                stick: Container(
                  decoration: const BoxDecoration(
                    shape: BoxShape.circle,
                    color: Colors.black,
                  ),
                  width: 40,
                  height: 40,
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  @override
  void dispose() {
    _sendTimer?.cancel();
    sender.close();
    super.dispose();
  }
}

// Einstellungen-Seite
class SettingsPage extends StatelessWidget {
  const SettingsPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Einstellungen'),
      ),
      body: const Center(
        child: Text('Hier kommen die Einstellungen!'),
      ),
    );
  }
}
